// Autor: Rodrigo Mendes Leme                    Numero USP: 3151151
// Curso: computacao                             Data: 3/06/2001
// Professor: Paulo Feofiloff
// Exercicio 7
// Compilador usado: GCC-Linux
// Descricao: verifica se existe um par disjunto de caminhos entre dois dados
//            vertices.

#include <stdio.h>
#include "gb_graph.h"
#include "gb_rand.h"
#include "gb_econ.h"
#include "gb_games.h"
#include "gb_miles.h"
#include "gb_basic.h"

#define pred x.V
#define flx a.V
#define caminho w.I
#define apred b.V
#define sep z.I
#define cam_1 u.V
#define cam_2 v.V

int acha_primeiro_caminho(Vertex *v, Vertex *s);
int acha_segundo_caminho(Vertex *v, Vertex *s);
void separa_caminhos(Vertex *r, Vertex *s);
int caminhos_disjuntos(Graph *g, Vertex *r, Vertex *s);
void imprime_primeiro_caminho(Vertex *r, Vertex *s);
void imprime_segundo_caminho(Vertex *r, Vertex *s);
void imprime_separador(Graph *g);

// Funcao: main

int main(int argc, char *argv[])
{
  Graph *g;
  Vertex *r,
         *s;
  long p,
       q,
       r_i,
       s_i;

  if (argc != 6)
  {
    fprintf(stderr, "Erro: numero de argumentos invalido.\n");
    return -1;
  }
  if ((strcmp(argv[1], "rand") != 0) && (strcmp(argv[1], "econ") != 0) &&
      (strcmp(argv[1], "games") != 0) && (strcmp(argv[1], "miles") != 0) &&
      (strcmp(argv[1], "board") != 0) && (strcmp(argv[1], "subsets") != 0))
  {
    fprintf(stderr, "Erro: segundo argumento invalido.\n");
    return -2;
  }
  p = atol(argv[2]);
  q = atol(argv[3]);

  // Descricao dos grafos:
  //
  // - rand: p = numero de vertices e q = numero de arcos.
  // - econ: p = numero de vertices e q = numero de vertices especiais a serem
  //         omitidos.
  // - games: p = numero de times e q = semente.
  // - miles: p = numero de cidades e q = numero de ligacoes entre as cidades.
  // - board: p = numero de direcoes na direcao horizontal e q = numero
  //              de direcoes na direcao vertical.
  // - subsets: p = conjunto e q = cardinalidade do conjunto.

  if (strcmp(argv[1], "rand") == 0)
    g = random_graph(p, q, 1, 1, 1, NULL, NULL, 0, 0, 0);
  if (strcmp(argv[1], "econ") == 0)
    g = econ(p, 2, q, 0);
  if (strcmp(argv[1], "games") == 0)
    g = games(p, 0, 0, 0, 0, 0, 0, q);
  if (strcmp(argv[1], "miles") == 0)
    g = miles(p, 0, 0, 1, 0, q, 0);
  if (strcmp(argv[1], "board") == 0)
    g = gunion(board(p, q, 0, 0, -1, 0, 0), board(p, q, 0, 0, -2, 0, 0));
  if (strcmp(argv[1], "subsets") == 0)
    g = subsets(q, 1, 1- p, 0, 0, 0, 1, 0);

  r_i = atol(argv[4]);
  if ((r_i < 0) || (r_i >= g->n))
  {
    fprintf(stderr, "Erro: terceiro argumento invalido.\n");
    return -3;
  }
  s_i = atol(argv[5]);
  if ((s_i < 0) || (s_i >= g->n))
  {
    fprintf(stderr, "Erro: quarto argumento invalido.\n");
    return -4;
  }
  r = g->vertices + r_i;
  s = g->vertices + s_i;
 
  fprintf(stdout, "=======================================================\n");
  fprintf(stdout, "Caminhos disjuntos de um grafo do tipo %s:\n", argv[1]);
  fprintf(stdout, "-------------------------------------------------------\n");
  fprintf(stdout, "Nome: %s\n", g->id);
  fprintf(stdout, "Numero de vertices: %ld\n", g->n);
  fprintf(stdout, "Numero de arcos: %ld\n", g->m);
  fprintf(stdout, "-------------------------------------------------------\n");
  if (caminhos_disjuntos(g, r, s))
  {
    fprintf(stdout, "Existe um par disjunto de caminhos.\n\n");
    fprintf(stdout, "Primeiro caminho:\n");
    imprime_primeiro_caminho(r, s);
    fprintf(stdout, "-----------------------------------------------------\n");
    fprintf(stdout, "Segundo caminho:\n");
    imprime_segundo_caminho(r, s);
  }
  else
  {
    fprintf(stdout, "Nao existe um par disjunto de caminhos.\n\n");
    fprintf(stdout, "Vertices do separador:\n");
    imprime_separador(g);
  }
  fprintf(stdout, "=======================================================\n");
  return 0;
}

// Funcao: acha_primeiro_caminho
// Descricao: encontra um caminho entre dois dados vertices v e s, retornando 1
//            se o caminho existe e 0 caso contrario.

int acha_primeiro_caminho(Vertex *v, Vertex *s)
{
  Vertex *w;
  Arc *a;
  
  for (a = v->arcs; a != NULL; a = a->next)
  {
    w = a->tip;
    if (w->sep != 0)
      continue;
    w->sep = 1;
    if (w == s)
    {
      w->pred = a->flx = v;
      w->sep  = 0;
      return 1;
    }
    if (acha_primeiro_caminho(w, s))
    {
      w->pred = a->flx = v;
      return 1;
    }
  }
  return 0;
}

// Funcao: acha_segundo_caminho
// Descricao: encontra um segundo caminho entre dois dados vertices v e s, re-
//            tornando 1 se o caminho existe e 0 caso contrario.

int acha_segundo_caminho(Vertex *v, Vertex *s)
{
  Vertex *w;
  Arc *a,
      *intersecao;

  // Primeiro procura todos os arcos no sentido convencional
  for (a = v->arcs; a != NULL; a = a->next)
  {  
    w = a->tip;
    if (w->sep == 1 && w->pred == NULL)
      continue;
    if (a->flx != NULL)
      continue;
    w->sep = 2;
    if (w == s)
    {
      a->apred = v;
      w->sep   = 0;
      return 1;
    }    
    if (acha_segundo_caminho(w, s))
    {
      a->apred = v;
      return 1;
    }
  }
  
  // Nao encontrou um caminho convencional; procura uma sequencia de aumento
  if (a == NULL)
    if (v->pred != NULL)
      if (acha_segundo_caminho(v->pred, s))
      {
        intersecao = v->pred->arcs;
        while (intersecao->tip != v && intersecao->flx != v->pred)
          intersecao = intersecao->next;
        intersecao->apred = v;	    
        return 1;
      }
  return 0;
}

// Funcao: separa_caminhos
// Descricao: dados dois vertices r e s, devolve um par de caminhos disjuntos
//            entre r e s.

void separa_caminhos(Vertex *r, Vertex *s)
{
  Vertex *v,
         *w;
  Arc *a; 
 
  // Marca os vertices do primeiro caminho 
  v = r;
  while (v != s)
  {
    a = v->arcs;
    while(a != NULL)
    {
      w = a->tip;
      if (w->pred == NULL && a->apred == NULL) 
        a = a->next;
      else
        if (a->flx != NULL && a->apred != NULL && w != s)    // Intersecao
          a = a->next;                                       // de caminhos
        else
          break;
    }
    if (a != NULL)      //E um vertice do primeiro caminho
    {
      a->apred = a->flx;
      v->cam_1 = v = w;
    }
  }
  
  // Marca os vertices do segundo caminho
  v = r;
  while (v != s)
  {
    a = v->arcs;
    while (a != NULL)
    {
      w = a->tip;
      if (w->pred == NULL && a->apred == NULL) 
        a = a->next;
      else
        if (a->flx != NULL && a->apred != NULL && w != s)     // Intersecao
          a = a->next;                                        // de caminhos
        else
          break;
    }
    if (a != NULL)          // E um vertice do segundo caminho
      v->cam_2 = v = w;
  }
}

// Funcao: caminhos_disjuntos
// Descricao: dado um grafo g e dois vertices r e s, devolve 1 se existe um par
//            de caminhos disjuntos de r ate s ou 0 caso contrario (nesse caso,
//            obtem um separador de rs).

int caminhos_disjuntos(Graph *g, Vertex *r, Vertex *s)
{
  Vertex *v;
  Arc *a;

  for (v = g->vertices; v < g->vertices+g->n; v++)
  {
    v->pred = v->cam_1 = v->cam_2 = NULL;
    v->sep  = v->caminho = 0;
    for (a = v->arcs; a != NULL; a = a->next)
      a->apred = a->flx = NULL;
  }
  
  r->pred = r;
  r->sep  = 1;
  if (acha_primeiro_caminho(r, s))
  { 
    if (acha_segundo_caminho(r, s))
    {
      separa_caminhos(r, s);
      return 1;
    }
    else
      return 0;
  }
  else
    return 0;
}

// Funcao: imprime_primeiro_caminho
// Descricao: imprime os vertices de um dos caminhos disjuntos de r ate s.

void imprime_primeiro_caminho(Vertex *r, Vertex *s)
{
  Vertex *v;
 
  fprintf(stdout, "(");
  for (v = r; v != s; v = v->cam_1)
    fprintf(stdout, "%s->", v->name);
  fprintf(stdout, "%s)\n", s->name);
}

// Funcao: imprime_segundo_caminho
// Descricao: imprime os vertices do outro caminho disjunto de r ate s.

void imprime_segundo_caminho(Vertex *r, Vertex *s)
{
  Vertex *v;
 
  fprintf(stdout, "(");
  for (v = r; v != s; v = v->cam_2)
    fprintf(stdout, "%s->", v->name);
  fprintf(stdout, "%s)\n", s->name);
}

// Funcao: imprime_separador
// Descricao: imprime os vertices de um separador de rs.

void imprime_separador(Graph *g)
{
  Vertex *v;

  for (v = g->vertices; v < g->vertices + g->n; v++)
    if (v->sep != 0)
      fprintf(stdout, "%s\n", v->name);
}

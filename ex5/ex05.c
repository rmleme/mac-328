// Autor: Rodrigo Mendes Leme                    Numero USP: 3151151
// Curso: computacao                             Data: 20/03/2001
// Professor: Paulo Feofiloff
// Exercicio 5
// Compilador usado: GCC-Linux
// Descricao: analisa alguns tipos de grafos gerados pelo Stanford Graph Base.

#include "gb_graph.h"
#include "gb_rand.h"
#include "gb_basic.h"
#include "gb_games.h"
#include "gb_miles.h"
#include "gb_econ.h"
#include "gb_basic.h"
#include <stdio.h>
#include <stdlib.h>

#define GRAU_SAIDA u.I
#define GRAU_ENTRADA v.I
#define ROTULO w.V
#define JA_MARCOU a.I

int confere_num_arcos(Graph *g);
long conta_lacos(Graph *g);
long conta_arc_paral(Graph *g);
long conta_arc_antiparal(Graph *g);
void conta_graus_saida(Graph *g);
void conta_graus_entrada(Graph *g);
long conta_grau_saida_conj(Graph *g);
long conta_grau_entrada_conj(Graph *g);
void rotula_vertices(Graph *g);
void obtem_mestres(Graph *g, Vertex **mestres, long *qtos_componentes);
void tamanho_componentes(Graph *g, Vertex **mestres, long *tamanho_comp,
                         long qtos_componentes);
void agrega_componentes(Graph *g, Vertex **mestres, long *tamanho_comp,
                        Vertex **vertices_comp, long qtos_componentes);
void ordena_vertices(long *tamanho_comp, Vertex **vertices_comp,
                     long qtos_componentes);
long **monta_matriz_adj(Graph *g);

// Funcao: main

int main(int argc, char *argv[])
{
  Graph *g,
        *g1,
        *g2;
  Vertex *v,
         **mestres,
         **vertices_comp;
  long i,
       j,
       p,
       q,
       num_lacos,
       arcos_antiparal,
       grau_saida_conj,
       grau_entrada_conj,
       qtos_componentes,
       *tamanho_comp,
       **matriz_adj;

  if (argc != 4)
  {
    fprintf(stderr,"Erro: numero de argumentos invalido.\n");
    return -1;
  }
  if ((strcmp(argv[1], "rand") != 0) && (strcmp(argv[1], "subsets") != 0) &&
      (strcmp(argv[1], "games") != 0) && (strcmp(argv[1], "miles") != 0) &&
      (strcmp(argv[1], "dama") != 0) && (strcmp(argv[1], "pessoal") != 0))
  {
    fprintf(stderr,"Erro: segundo argumento invalido.\n");
    return -2;
  }
  p = atol(argv[2]);
  q = atol(argv[3]);
  
  // Descricao dos grafos:
  //
  // - rand: p = numero de vertices e q = numero de arcos.
  // - subsets: p = conjunto e q = cardinalidade do conjunto.
  // - games: p = numero de times e q = semente.
  // - miles: p = numero de cidades e q = numero de ligacoes entre as cidades
  // - board (dama): p = numero de direcoes na direcao horizontal e q = numero
  //                 de direcoes na direcao vertical.
  // - econ: p = numero de vertices e q = numero de vertices especiais a serem
  //         omitidos.

  if (strcmp(argv[1], "rand") == 0)
    g = random_graph(p, q, 1, 1, 1, NULL, NULL, 0, 0, 0);
  if (strcmp(argv[1], "subsets") == 0)
    g = subsets(q, 1, 1 - p, 0, 0, 0, 1, 0);
  if (strcmp(argv[1], "games") == 0)
    g = games(p, 0, 0, 0, 0, 0, 0, q);
  if (strcmp(argv[1], "miles") == 0)
    g = miles(p, 0, 0, 1, 0, q, 0);
  if (strcmp(argv[1], "dama") == 0)
  {
    g1 = board(p, q, 0, 0, -1, 0, 0);
    g2 = board(p, q, 0, 0, -2, 0, 0);
    g = gunion(g1, g2, 0, 0);
  }
  if (strcmp(argv[1], "econ") == 0)
    g = econ(p, q, 0, 0);
  v = g->vertices;

  fprintf(stdout, "=======================================================\n");
  fprintf(stdout, "Analise de um grafo do tipo %s:\n", argv[1]);
  fprintf(stdout, "-------------------------------------------------------\n");
  fprintf(stdout, "Nome: %s\n", g->id);
  fprintf(stdout, "Numero de vertices: %ld\n", g->n);

  if (confere_num_arcos(g))
    fprintf(stdout, "Numero de arcos: %ld\n", g->m);
  else
  {
    fprintf(stderr, "Erro: conflito no numero de arcos do grafo.\n");
    return -3;
  }

  num_lacos = conta_lacos(g);
  fprintf(stdout, "Numero de lacos: %ld\n", num_lacos);
  fprintf(stdout, "Numero de arcos paralelos: %ld\n", conta_arc_paral(g));
  arcos_antiparal = conta_arc_antiparal(g);
  fprintf(stdout, "Numero de arcos anti-paralelos: %ld\n", arcos_antiparal);

  if (g->m - num_lacos == arcos_antiparal)
    fprintf(stdout, "O grafo e simetrico.\n");
  else
    fprintf(stdout, "O grafo nao e simetrico.\n");
  fprintf(stdout, "-------------------------------------------------------\n");

  conta_graus_saida(g);
  conta_graus_entrada(g);
  fprintf(stdout, "Vertices:\n");
  fprintf(stdout, "Gs Ge Nome\n");
  for (i = 0; i < g->n; i++)
    fprintf(stdout, "%ld  %ld  %s\n", v[i].GRAU_SAIDA, v[i].GRAU_ENTRADA,
            v[i].name);
  fprintf(stdout, "-------------------------------------------------------\n");

  grau_saida_conj = conta_grau_saida_conj(g);
  if (grau_saida_conj != -1)
  {
    fprintf(stdout, "Grau de saida do conjunto {v[0], ... ,v[4]}: %ld\n",
            grau_saida_conj);
    grau_entrada_conj = conta_grau_entrada_conj(g);
    if (grau_entrada_conj != -1)
      fprintf(stdout, "Grau de entrada do conjunto {v[0], ... ,v[4]}: %ld\n",
              grau_entrada_conj);
  }
  else
  {
    fprintf(stdout, "O grafo possui menos de 5 vertices.\n");
    fprintf(stdout, "Nao e existe o conjunto {v[0], ... ,v[4]}.\n");
  }
  fprintf(stdout, "-------------------------------------------------------\n");

  rotula_vertices(g);
  obtem_mestres(g, mestres, &qtos_componentes);
  fprintf(stdout, "Numero de componentes: %ld\n\n", qtos_componentes);
  tamanho_comp = (long *) malloc(qtos_componentes * sizeof(long));
  tamanho_componentes(g, mestres, tamanho_comp, qtos_componentes);
  vertices_comp = (Vertex **) malloc(qtos_componentes * sizeof(Vertex *));
  for (i = 0; i < qtos_componentes; i++)
    vertices_comp[i] = (Vertex *) malloc(tamanho_comp[i] * sizeof(Vertex));
  agrega_componentes(g, mestres, tamanho_comp, vertices_comp,
                     qtos_componentes);
  ordena_vertices(tamanho_comp, vertices_comp, qtos_componentes);
  for (i = 0; i < qtos_componentes; i++)
  {
    fprintf(stdout, "Componente %ld: %ld vertices\n", i + 1, tamanho_comp[i]);
    for (j = 0; j < tamanho_comp[i]; j++)
      fprintf(stdout, "   (Gs = %ld, Ge = %ld, Vertice = %s\n",
              vertices_comp[i][j].GRAU_SAIDA,
              vertices_comp[i][j].GRAU_ENTRADA,
              vertices_comp[i][j].name);
  }
  fprintf(stdout, "-------------------------------------------------------\n");

  matriz_adj = monta_matriz_adj(g);
  if (matriz_adj == NULL)
  {
    fprintf(stdout, "O grafo possui mais de 60 vertices.\n");
    fprintf(stdout, "Nao e possivel imprimir a matriz de adjacencias.\n");
  }
  else
  {  
    fprintf(stdout, "Matriz de adjacencias:\n");
    fprintf(stdout, " |");
    for (j = 0; j < g->n; j++)
      fprintf(stdout, "%ld ", j);
    fprintf(stdout, "\n--");
    for (j = 0; j < g->n; j++)
      fprintf(stdout, "--");
    fprintf(stdout, "\n");
    for (i = 0; i < g->n; i++)
    {
      fprintf(stdout, "%ld|", i);
      for (j = 0; j < g->n; j++)
        fprintf(stdout, "%ld ", matriz_adj[i][j]);
      fprintf(stdout, "\n");
    }
  }
  fprintf(stdout, "=======================================================\n");
  return 0;
}

// Funcao: confere_num_arcos
// Descricao: confere o numero de arcos de um grafo g, retornando 1 se ele for
//            igual g->m e 0 caso contrario.

int confere_num_arcos(Graph *g)
{
  long i,
       cont = 0;
  Arc *atu;
  Vertex *v = g->vertices;

  for (i = 0; i < g->n; i++)
  {
    atu = v[i].arcs;
    while (atu != NULL)
    {
      cont++;
      atu = atu->next;
    }
  }
  if (cont == g->m)
    return 1;
  else
    return 0;
}

// Funcao: conta_lacos
// Descricao: conta o numero de lacos de um grafo g.

long conta_lacos(Graph *g)
{
  long i,
       cont = 0;
  Arc *atu;
  Vertex *v = g->vertices;

  for (i = 0; i <= g->n; i++)
  {
    atu = v[i].arcs;
    while (atu != NULL)
    {
      if (atu->tip == &v[i])
        cont++;
      atu = atu->next;
    }
  }
  return cont;
}

// Funcao: conta_arc_paral
// Descricao: conta o numero de arcos paralelos de um grafo g.

long conta_arc_paral(Graph *g)
{
  long i,
       cont = 0;  
  int achou;
  Arc *lento,
      *rapido;
  Vertex *v = g->vertices;

  for (i = 0; i < g-> n; i++)
    for (lento = v[i].arcs; lento != NULL; lento = lento->next)
    {
      achou = 0;
      for (rapido = v[i].arcs; rapido != NULL; rapido = rapido->next)
        if ((lento != rapido) && (!rapido->JA_MARCOU) &&
            (rapido->tip == lento->tip))
	{
          cont++;
          achou = 1;
          lento->JA_MARCOU = 1;
          rapido->JA_MARCOU = 1;
        }
      if (achou)
        cont++;
    }
  return cont;
}

// Funcao: conta_arc_antiparal
// Descricao: conta o numero de arcos anti-paralelos de um grafo g.

long conta_arc_antiparal(Graph *g)
{
  long i,
       cont = 0;
  Arc *atu,
      *atu_aux;
  Vertex *v = g->vertices;

  for (i = 0; i < g->n; i++)
  {
    atu = v[i].arcs;
    while (atu != NULL)
    {
      atu_aux = atu->tip->arcs;
      while (atu_aux != NULL)
      {
        if (atu_aux->tip == &v[i])
          cont++;
        atu_aux = atu_aux->next;
      }
      atu = atu->next;
    }
  }
  return cont;
}

// Funcao: conta_graus_saida
// Descricao: conta o grau de saida de todos os vertices de um grafo g.

void conta_graus_saida(Graph *g)
{
  long i,
       cont;
  Arc *atu;
  Vertex *v = g->vertices;

  for (i = 0; i < g->n; i++)
  {
    cont = 0;
    atu = v[i].arcs;
    while (atu != NULL)
    {
      cont++;
      atu = atu->next;
    }
    v[i].GRAU_SAIDA = cont;
  }
}

// Funcao: conta_graus_entrada
// Descricao: contra o grau de entrada de todos os vertices de um grafo g.

void conta_graus_entrada(Graph *g)
{
  long i,
       j,
       cont;
  Arc *atu;
  Vertex *v = g->vertices;

  for (i = 0; i < g->n; i++)
  {
    cont = 0;
    for (j = 0; j < g->n; j++)
    {
      atu = v[j].arcs;
      while (atu != NULL)
      {
        if (atu->tip == &v[i])
          cont++;
        atu = atu->next;
      }
      v[j].GRAU_ENTRADA = cont;
    }
  }
}

// Funcao: conta_grau_saida_conj
// Descricao: se um dado grafo g tiver pelo menos 5 vertices, conta o grau de
//            saida do conjunto de vertices {v[0], ... ,v[4]} de g.

long conta_grau_saida_conj(Graph *g)
{
  int i;
  long cont = 0;
  Arc *atu;
  Vertex *v = g->vertices;

  if (g->n < 5)
    return -1;
  else
  {
    for (i = 0; i < 5; i++)
    {
      atu = v[i].arcs;
      while (atu != NULL)
      {
        if ((atu->tip != &v[0]) && (atu->tip != &v[1]) &&
            (atu->tip != &v[2]) && (atu->tip != &v[3]) &&
            (atu->tip != &v[4]))
          cont++;
        atu = atu->next;
      }
    }
    return cont;
  }
}

// Funcao: conta_grau_entrada_conj
// Descricao: se um dado grafo g tiver pelo menos 5 vertices, conta o grau de
//            entrada do conjunto de vertices {v[0], ... ,v[4]} de g.

long conta_grau_entrada_conj(Graph *g)
{
  long i,
       cont = 0;
  Arc *atu;
  Vertex *v = g->vertices;

  if (g->n < 5)
    return -1;
  else
  {
    for (i = 5; i < g->n; i++)
    {
      atu = v[i].arcs;
      while (atu != NULL)
      {
        if ((atu->tip == &v[0]) || (atu->tip == &v[1]) ||
            (atu->tip == &v[2]) || (atu->tip == &v[3]) ||
            (atu->tip == &v[4]))
          cont++;
        atu = atu->next;
      }
    }
    return cont;
  }
}

// Funcao: rotula_vertices
// Descricao: atribui a cada vertice dos componentes de um grafo g um rotulo,
//            usando "vertices-mestres" no papel destes.

void rotula_vertices(Graph *g)
{
  long i,
       x;
  Arc *atu;
  Vertex *alfa,
         *beta,
         *v = g->vertices;

  for (i = 0; i < g->n; i++)
    v[i].ROTULO = &v[i];

  for (i = 0; i < g->n; i++)
  {
    atu = v[i].arcs;
    while (atu != NULL)
    {
      alfa = v[i].ROTULO;
      beta = atu->tip->ROTULO;
      if (alfa != beta);
        for (x = 0; x < g->n; x++)
          if (v[x].ROTULO == beta)
            v[x].ROTULO = alfa;
      atu = atu->next;
    }
  }
}

// Funcao: obtem_mestres
// Descricao: dado um grafo g, obtem os "vertices-mestres" do mesmo, bem como
//            seu numero de componentes.

void obtem_mestres(Graph *g, Vertex **mestres, long *qtos_componentes)
{
  long i;
  Vertex *v = g->vertices;

  *qtos_componentes = 0;
  *mestres = (Vertex *) malloc(g->n * sizeof(Vertex *));
  for (i = 0; i < g->n; i++)
    if (v[i].ROTULO == &v[i])
      mestres[(*qtos_componentes)++] = &v[i];
}

// Funcao: tamanho_componentes
// Descricao: conta o tamanho de cada componente de um grafo g.

void tamanho_componentes(Graph *g, Vertex **mestres, long *tamanho_comp,
                         long qtos_componentes)
{
  long i,
       j;
  Vertex *v = g->vertices;

  for (i = 0; i < qtos_componentes; i++)
    tamanho_comp[i] = 0;
  for (i = 0; i < qtos_componentes; i++)
    for (j = 0; j < g->n; j++)
      if (v[j].ROTULO == mestres[i])
        tamanho_comp[i]++;
}

// Funcao: agrega_componentes
// Descricao: agrega todos os vertices dos componentes de um grafo g, criando
//            um vetor de vertices por componente.

void agrega_componentes(Graph *g, Vertex **mestres, long *tamanho_comp,
                        Vertex **vertices_comp, long qtos_componentes)
{
  long i,
       j,
       k;
  Vertex *v = g->vertices;

  for (i = 0; i < qtos_componentes; i++)
  {
    k = 0;
    for (j = 0; (j < g->n) && (k < tamanho_comp[i]); j++)
      if (v[j].ROTULO == mestres[i])
        vertices_comp[i][k++] = v[j];
  }
}

// Funcao: ordena_vertices
// Descricao: ordena em ordem descrescente de grau de saida os vertices de cada
//            componente de um grafo g (atraves do algoritmo selection sort).

void ordena_vertices(long *tamanho_comp, Vertex **vertices_comp,
                     long qtos_componentes)
{
  long i,
       maior,
       lento,
       rapido;
  Vertex aux;

  for (i = 0; i < qtos_componentes; i++)
    for (lento = 0; lento < tamanho_comp[i] - 1; lento++)
    {
      maior = lento;
      for (rapido = lento + 1; rapido < tamanho_comp[i]; rapido++)
 	if (vertices_comp[i][rapido].GRAU_SAIDA >
            vertices_comp[i][maior].GRAU_SAIDA)
          maior = rapido;
      aux = vertices_comp[i][lento];
      vertices_comp[i][lento] = vertices_comp[i][maior];
      vertices_comp[i][maior] = aux;
    }
}

// Funcao: matriz_adj
// Descricao: aloca e monta a matriz de adjacencias de um grafo g, retornando
//            NULL se nao foi possivel faze-lo ou a matriz caso contrario.

long **monta_matriz_adj(Graph *g)
{
  long i,
       j,
       **matriz;
  Arc *atu;
  Vertex *v = g->vertices;

  if (g->n > 60)
    return NULL;
  else
  {
    matriz = (long **) malloc(g->n * sizeof(long *));
    for (i = 0; i < g->n; i++)
      matriz[i] = (long *) malloc(g->n * sizeof(long));
  
    for (i = 0; i < g->n; i++)
      for (j = 0; j < g->n; j++)
        matriz[i][j] = 0;

    for (i = 0; i < g->n; i++)
    {
      atu = v[i].arcs;
      while (atu != NULL)
      {
        matriz[i][atu->tip - g->vertices]++;
        atu = atu->next;
      }
    }
    return matriz;
  }
}

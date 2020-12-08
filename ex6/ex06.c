// Autor: Rodrigo Mendes Leme                    Numero USP: 3151151
// Curso: computacao                             Data: 14/04/2001
// Professor: Paulo Feofiloff
// Exercicio 6
// Compilador usado: GCC-Linux
// Descricao: obtem o caminho minimo entre dois vertices de um grafo dado.

#include "gb_graph.h"

#define dist z.I
#define backlink y.V
#define hh_val x.I
#define llink v.V
#define rlink w.V

static Vertex head[128];

// Funcao: init_dlist
// Descricao: cria uma lista duplamente encadeada vazia.

void init_dlist(long d)
{
  head->llink = head->rlink = head;
  head->dist = d - 1;
}

// Funcao: enlist
// Descricao: inclui um novo elemento na lista duplamente encadeada.

void enlist(Vertex *v, long d)
{
  register Vertex *t = head->llink;

  v->dist = d;
  while (d < t->dist)
    t = t->llink;
  v->llink = t;
  (v->rlink = t->rlink)->llink = v;
  t->rlink = v;
}

// Funcao: reenlist
// Descricao: decrementa o valor da chave de um elemento da lista duplamente
//            encadeada.

void reenlist(Vertex *v, long d)
{
  register Vertex *t = v->llink;

  (t->rlink = v->rlink)->llink = v->llink;
  v->dist = d;
  while (d < t->dist)
    t = t->llink;
  v->llink = t;
  (v->rlink = t->rlink)->llink = v;
  t->rlink = v;
}

// Funcao: del_first
// Descricao: exclui o elemento de menor chave da lista duplamente encadeada.

Vertex *del_first()
{
  Vertex *t;

  t = head->rlink;
  if (t == head)
    return NULL;
  (head->rlink = t->rlink)->llink = head;
  return t;
}

void (*init_queue)() = init_dlist;
void (*enqueue)() = enlist;
void (*requeue)() = reenlist;
Vertex *(*del_min)() = del_first;

// Funcao: swap_path
// Descricao: inverte os passos do menor caminho achado, para que o mesmo possa
//            ser percorrido na direcao correta.

Vertex *swap_path(Vertex *vv)
{
  register Vertex *t = NULL,
                  *p = vv,
                  *q;

  if(!p->backlink)
    return NULL;
  do
  {
    q = p->backlink;
    p->backlink = t;
    t = p;
    p = q;
  } while (t != p);
  return t;
}

// Funcao: caminho_minimo
// Descricao: dados dois vertices r e s de um grafo g, encontra e retorna o ca-
//            minho minimo entre r e s; se o mesmo nao existir, retorna NULL.

Vertex *caminho_minimo(Vertex *r, Vertex *s, Graph *g)
{
  register Vertex *t;

  for (t = g->vertices + g->n - 1; t >= g->vertices; t--)
    t->backlink = NULL;
  r->backlink = r;
  r->dist = 0;
  r->hh_val = 0;
  (*init_queue)(0L);

  t = r;
  while(t != s)
  {
    {
      register Arc *a;
      register long d = t->dist - t->hh_val;

      for(a = t->arcs; a; a = a->next)
      {
        register Vertex *v = a->tip;
        if (v->backlink)
        {
          register long dd = d + a->len + v->hh_val;
          if (dd < v->dist)
          {
            v->backlink = t;
            (*requeue)(v, dd);
          }
        }
        else
        {
          v->hh_val = 0;
          v->backlink = t;
          (*enqueue)(v, d + a->len + v->hh_val);
        }
      }
    }
    t = (*del_min)();
    if(t == NULL)
      return NULL;
  }
  return swap_path(s);
}

extern Vertex *caminho_minimo()
extern Vertex *swap_path()

#define dist z.I
#define backlink y.V

extern void (*init_queue)();
extern void (*enqueue)();
extern void (*requeue)();
extern Vertex *(*del_min)();

#define hh_val x.I

extern void init_dlist();
extern void enlist();
extern void reenlist();
extern Vertex *del_first();

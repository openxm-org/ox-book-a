#define NIL 0
#define NUMBER 1
#define LIST 2
#define ID  3

union any {
  int mint;
  struct list *list;
  void *op;
};

struct object {
  int tag;
  union any body;
};

struct list {
  struct object *left;
  struct list *right;
};

struct object * new_object();
struct object * new_object_mint(int x);
struct object * new_object_list(struct list *lst);
struct list * new_list(struct object *op);
struct list * append(struct list *lst,struct object *op);
struct list * concat_list(struct list *lst,struct list *lst2);
void print_list(struct list *lst);
void print_object(struct object *op);

/*
Note.
new_object_list((struct list *)NULL) returns empty list object.
*/

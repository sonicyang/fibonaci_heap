#ifndef __FB_H__
#define __FB_H__

#include <stdlib.h>

struct fib_heap_t{
    struct node_t* head;
};

struct node_t{
    struct node_t* p;
    struct node_t* child;
    struct node_t* sibling;
    int key;
    int degree;
};

struct fib_heap_t* make_fib_heap(void);
int fib_heap_minimum(struct fib_heap_t*);
int fib_heap_union(struct fib_heap_t*);
int fib_heap_insert(struct fib_heap_t*, struct node_t);
int fib_heap_extract_min(struct fib_heap_t*);
void fib_heap_decrease_key(struct fib_heap_t*, struct node_t, int);
void fib_heap_delete(struct fib_heap_t*, struct node_t);
void delete_fib_heap(struct fib_heap_t*);

#endif//__FB_H__

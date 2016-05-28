#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#include "fb.h"

static void fib_heap_link(struct fib_heap_t* h, struct node_t* n1, struct node_t* n2){
    n1->left->right = n1->right;
    n1->right->left = n1->left;

    if(n2->child){
        n1->right = n2->child;
        n1->left = n2->child->left;
        n2->child->left->right = n1;
        n2->child->left = n1;
    }else{
        n2->child = n1;
        n1->left = n1;
        n1->right = n1;
    }
    n2->degree++;
    n1->mark = 0;
}

static void consolidate(struct fib_heap_t* h){
    int i, d, D;
    struct node_t* x;
    struct node_t* w;
    struct node_t* v;
    struct node_t* y;
    struct node_t* tmp;
    struct node_t** A;

    D = ceil(log2(h->n));

    A = malloc(D * sizeof(struct node_t*));
    memset(A, 0, D * sizeof(struct node_t*));

    v = h->min;
    w = h->min;
    do{
        x = w;
        w = w->right;
        d = x->degree;
        while(A[d] != NULL){
            y = A[d];
            if(x->key > y->key){
                tmp = x;
                x = y;
                y = tmp;
            }
            fib_heap_link(h, y, x);
            A[d] = NULL;
            d++;
        }
        A[d] = x;
    }while(w != v);

    h->min = NULL;
    for(i = 0; i < D; i++){
        if(A[i] != NULL){
            if(h->min == NULL){
                h->min = A[i];
                h->min->left = h->min;
                h->min->right = h->min;
            }else{
                A[i]->right = h->min;
                A[i]->left = h->min->left;
                h->min->left->right = A[i];
                h->min->left = A[i];

                if(A[i]->key < h->min->key)
                    h->min = A[i];
            }
        }
    }

    free(A);
    return;
}

static void cut(struct fib_heap_t* h, struct node_t* x, struct node_t* y){
    y->degree--;

    if(x->right == x){
        y->child = NULL;
    }else{
        y->child = x->right;
        x->right->left = x->left;
        x->left->right = x->right;
    }
    x->right = h->min;
    x->left = h->min->left;
    h->min->left->right = x;
    h->min->left = x;

    x->p = NULL;
    x->mark = 0;

    return;
}

static void cascade_cut(struct fib_heap_t* h, struct node_t* y){
    struct node_t* z = y->p;

    if(z != NULL){
        if(y->mark == 0){
            y->mark = 1;
        }else{
            cut(h, y, z);
            cascade_cut(h, z);
        }
    }

    return;
}

struct fib_heap_t* make_fib_heap(void){
    struct fib_heap_t* ret = malloc(sizeof(struct fib_heap_t));
    ret->min = NULL;
    ret->n = 0;
    return ret;
}

int fib_heap_minimum(struct fib_heap_t* h){
     return h->min->key;
}

struct fib_heap_t* fib_heap_union(struct fib_heap_t* h1, struct fib_heap_t* h2){
    if(!h1)
        return h2;
    if(!h2)
        return h1;

    h2->min->left->right = h1->min->right;
    h1->min->right->left = h2->min->left;
    h1->min->right = h2->min;
    h2->min->left = h1->min;

    if(h2->min > h1->min)
        h1->min = h2->min;

    h1->n += h2->n;

    free(h2);

    return h1;
}

void fib_heap_insert(struct fib_heap_t* h, int x){
    struct node_t* n = malloc(sizeof(struct node_t));

    n->key = x;
    n->p = NULL;
    n->degree = 0;
    n->mark=0;
    n->child=NULL;

    if(!h->min){
        h->min = n;
        n->right = n;
        n->left = n;
    }else{
        n->right = h->min;
        n->left = h->min->left;
        h->min->left->right = n;
        h->min->left = n;
    }

    if(n->key < h->min->key)
        h->min = n;

    h->n++;

    return;
}

int fib_heap_extract_min(struct fib_heap_t* h){
    int ret;
    int i;
    struct node_t* ptr;
    struct node_t* ptr2;

    ret = h->min->key;
    ptr = h->min->child;
    for(i = 0; i < h->min->degree; i++){
        ptr2 = ptr->right;
        ptr->right = h->min;
        ptr->left = h->min->left;
        h->min->left->right = ptr;
        h->min->left = ptr;
        ptr = ptr2;
    }

    ptr = h->min;
    ptr->right->left = ptr->left;
    ptr->left->right = ptr->right;

    if(ptr == ptr->right){
        h->min=NULL;
    }else{
        h->min = ptr->right;
        consolidate(h);
    }
    h->n = h->n - 1;

    free(ptr);
    return ret;
}

void fib_heap_decrease_key(struct fib_heap_t* h, struct node_t* x, int k){
    struct node_t* y;

    if(k > x->key){
        puts("Error! Increasing key");
        return;
    }

    x->key = k;
    y = x->p;
    if(y != NULL && x->key < y->key){
        cut(h, x, y);
        cascade_cut(h, y);
    }

    if(x->key < h->min->key)
        h->min = x;
}

void fib_heap_delete(struct fib_heap_t* h, struct node_t* x){
    fib_heap_decrease_key(h, x, INT_MIN);
    fib_heap_extract_min(h);
}

static void free_node(struct node_t* x){
    int i;
    struct node_t* ptr;
    struct node_t* ptr2;

    ptr = x->child;
    for(i = 0; i < x->degree; i++){
        ptr2 = ptr->right;
        free_node(ptr);
        ptr = ptr2;
    }
    printf("Freeing: %d\n", x->key);
    free(x);
}

void delete_fib_heap(struct fib_heap_t* h){
    struct node_t* w;
    struct node_t* v;
    struct node_t* tmp;

    if(h->min != NULL){
        v = h->min;
        w = h->min;
        do{
            tmp = w;
            w = w->right;
            if(tmp != NULL)
                free_node(tmp);
        }while(w != v);
    }

    free(h);
}

#include <stdio.h>
#include <stdlib.h>
#include "fb.h"

void print_node(struct node_t* n, int x){
    struct node_t* end;
    int i;

    if(n != NULL){
        end = n;
        do{
            printf("(%d", n->key);
            if(n->degree)
                print_node(n->child, 1);
            printf(")");
            if(!x)
                printf("\n");
            n = n->right;
        }while(n != end);
    }

}

void print_fib_heap(struct fib_heap_t* h){
    print_node(h->min, 0);
}

int main(int argc, char* argv[]){
    struct fib_heap_t* h;

    //Init a Fib Heap
    puts("Fib Heap created");
    h = make_fib_heap();

    puts("Insert 10 elements");
    fib_heap_insert(h, 10);
    fib_heap_insert(h, 20);
    fib_heap_insert(h, 30);
    fib_heap_insert(h, 40);
    print_fib_heap(h);

    printf("Extract min: %d\n", fib_heap_extract_min(h));

    puts("After extract min");
    print_fib_heap(h);

    //free the heap
    puts("Freeing the fib heap");
    delete_fib_heap(h);
    return 0;
}


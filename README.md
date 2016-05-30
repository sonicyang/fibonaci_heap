Fibonacci Heap
==============

This is an implementation of fibonacci heap of integer type.

Usage
-----

To use this library, you should include fb.h.

#include "fb.h"

This library support 8 functions
 - make_fib_heap
 - fib_heap_minimum
 - fib_heap_union
 - fib_heap_insert
 - fib_heap_extract_min
 - fib_heap_decrease_key
 - fib_heap_delete
 - delete_fib_heap

The type of dynamic table is 
 - sturct fib_heap_t

The type of nodes 
 - sturct node_t

You must first declare and create a heap

`struct fib_heap_t* h = make_fib_heap()`

Then you can insert a integer x

`n = fib_heap_insert(h, x)`

Or you can decrease the key of a node 

`fib_heap_decrease_key(h, n, 0)`

Deleting a node

`fib_heap_delete(h, n)`

Extracting the minimum of the heap, this will trigger consolidate

`min = fib_heap_extract_minimum(h)`

Union 2 Fibonacci Heaps

`h = fib_heap_union(h1, h2)`

Don't forget to free the heap after use.

`delete_fib_heap(h)`

Example
-------
A example program is included as main.c. 

Please compile it with fb.c.

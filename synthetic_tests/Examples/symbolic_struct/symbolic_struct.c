#include "../../../../analyzer/include/klee/klee.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

// taken from https://gist.github.com/tovain10071991/8275da696c4e03a8a078
// klee_make_symbolic_range taken from:
// https://keeda.stanford.edu/pipermail/klee-dev/2011-July/000685.html
// This function marks a part of memory as symbolic by creating a
// new allocation and copying it over an existing memory area. 
// The existing area will then become symbolic.
void klee_make_symbolic_range(void* addr, size_t offset, 
                              size_t nbytes, const char* name) {
# ifdef KLEE
        assert(addr != NULL && "Must pass a valid addr");
        assert(name != NULL && "Must pass a valid name");

        if(nbytes == 0)
                return;

        void* start = addr + offset;
        klee_check_memory_access(start, nbytes);

        void* symbolic_data = malloc(nbytes);
        klee_make_symbolic(symbolic_data, nbytes, name);
        memcpy(start, symbolic_data, nbytes);
        free(symbolic_data);
# endif
}

typedef struct _my_type {
    char c;
    int x;
} my_type;

void main(void){
    my_type my;
    my.c = 'a';
    my.x = 1;

    # ifdef KLEE
    //klee_make_symbolic(&(my.x), sizeof my.x, "my.x");
    klee_make_symbolic_range(&my.x, 0, sizeof my.x, "my.x");
    # endif

    # ifdef KLEE
    // make sure that my.c is not symbolic
    if(my.c == 'a'){
        printf("my.c == 'a'\n");
    }else{
        printf("my.c != 'a'\n");
    }
    # endif

    if(my.x == 3){
        # ifdef KLEE
        printf("my.x == 3\n");
        #endif
    }else{
        # ifdef KLEE
        printf("my.x != 3\n");
        #endif
    }
}


#include "stdio.h"
#include "stdlib.h"
#include <stddef.h>

#define MAX_HEAPS 4096

typedef long Align;

union header {
	struct {
		union header *ptr;
		unsigned int size;
	} s;
	Align x;
};

typedef union header Header;

static unsigned char heaps[MAX_HEAPS];
static unsigned char *program_break = heaps;

static Header base; /* empty list to get started */
static Header *my_freep = NULL; /* start of my_free list */

void *_sbrk(unsigned int nbytes);

void my_free(void *ap);

void *my_malloc(unsigned int nbytes)
{
	Header *p, *prevp;
	unsigned int nunits;
	void *cp;

	nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;

	if ((prevp = my_freep) == NULL) {
		base.s.ptr = my_freep = prevp = &base;
		base.s.size = 0;
	}

	for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
		if (p->s.size >= nunits) {
			if (p->s.size == nunits) {
				prevp->s.ptr = p->s.ptr;
			} else {
				p->s.size -= nunits;
				p += p->s.size;
				p->s.size = nunits;
			}
			my_freep = prevp;
                        #ifdef KLEE
                        printf("my_malloc allocating %d bytes at %p\n",nbytes,p+1);
                        #endif
			return (void *)(p + 1);
		}

		if (p == my_freep) {
			cp = _sbrk(nunits * sizeof(Header));
			if (cp == (void *) -1) {
				return NULL;
			} else {
				p = (Header *) cp;
				p->s.size = nunits;
				my_free((void *) (p + 1));
				p = my_freep;
			}
		}
	}
}

void my_free(void *ap) {
	Header *bp, *p;
	bp = (Header *) ap - 1;

	for (p = my_freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr) {
		if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
			break;
	}

	if (bp + bp->s.size == p->s.ptr) {
		bp->s.size += p->s.ptr->s.size;
		bp->s.ptr = p->s.ptr->s.ptr;
	} else {
		bp->s.ptr = p->s.ptr;
	}

	if (p + p->s.size == bp) {
		p->s.size += bp->s.size;
		p->s.ptr = bp->s.ptr;
	} else {
		p->s.ptr = bp;
	}

	my_freep = p;
}

void main() {
  char *array = my_malloc(sizeof(char)*3);
  array += 4;
  *array = 3;
}

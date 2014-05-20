
/*
 * implement a private memory manage allocater
 */

#define STDIO_H_H

#ifdef STDIO_H_H

#include <stdio.h>	// for NULL, size_t
#include <unistd.h>	// for sbrk

#else

#define NULL	0
typedef int	size_t;

#endif

typedef long Align;

union Header {
	struct {
		union Header *ptr;
		unsigned size;
	} s;
	Align x;	// never used, but for align
};

union Header base;	// why do add union base ???? for usage
union Header *freep = NULL;

#define NALLOC	1024
void priv_free(void *addr);

static union Header *morecore(size_t nunits)
{
	union Header *p = NULL;
	void *ap = NULL;

	if (nunits < NALLOC)
		nunits = NALLOC;

	ap = sbrk(nunits * sizeof(*p));
	if (ap ==  (char *)-1) {
		return NULL;
	}

	p = (union Header *)ap;
	p->s.size = nunits;
	priv_free((void *)(p + 1));
	return freep;
}

void *priv_malloc(size_t nbytes)
{
	union Header *prevp;
	union Header *p;
	unsigned nunits;

	nunits = (nbytes + sizeof(*p) - 1) / (sizeof(*p)) + 1;
	if ((prevp = freep) == NULL) {
		base.s.ptr = freep = prevp = &base;	// &base for zero bytes alloc ?
		base.s.size = 0;
	}

	for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
		if (p->s.size >= nunits) {
			if (p->s.size == nunits) {
				prevp->s.ptr = p->s.ptr;
			} else {
				p->s.size -= nunits;
				p += p->s.size;
				p->s.size =nunits;
			}
			freep = prevp;	// next time, will search from the last searche item to get a block mem
			return (void *)(p + 1);
		}
		if (p == freep) {
			if ((p = morecore(nunits)) == NULL)
				return ;
		}
	}
	return (void *) p;
}

void priv_free(void *addr)
{
	union Header *ap = NULL;
	union Header *p  = NULL;

	ap = (union Header *)addr - 1;
	for (p = freep; !((p < ap) && (ap < p->s.ptr)); p = p->s.ptr) {
		if ((p >= p->s.ptr) && ((ap > p) || (ap < p->s.ptr)))
			break;
	}

	if (ap + ap->s.size == p->s.ptr) {
		ap->s.size += p->s.ptr->s.size;
		ap->s.ptr = p->s.ptr->s.ptr;
	} else {
		ap->s.ptr = p->s.ptr;
	}

	if (p + p->s.size == ap) {
		p->s.size += ap->s.size;
		p->s.ptr = ap->s.ptr;
	} else {
		p->s.ptr = ap;
	}

}

void print_item(void)
{
	union Header *p = NULL;

	for (p = freep->s.ptr; p != freep; p = p->s.ptr) {
		fprintf(stdout, "ptr: %p, size: %d, next: %p\n", p, p->s.size, p->s.ptr);
	}
}


/*
 * test
 */
/*
#include <stdlib.h>
int main(int argc, char *argv[])
{
	void *p = NULL;
	void *p2 = NULL;
	void *p3 = NULL;
	void *p4 = NULL;

	p = priv_malloc(1024);
	print_item();
	fprintf(stdout, "alloc %d bytes, %p\n", 1024, p);
	p2 = priv_malloc(1024);
	print_item();
	fprintf(stdout, "alloc %d bytes, %p\n", 1024, p2);
	p3 = priv_malloc(1024);
	print_item();
	fprintf(stdout, "alloc %d bytes, %p\n", 1024, p3);
	p4 = priv_malloc(1024);
	print_item();
	fprintf(stdout, "alloc %d bytes, %p\n", 1024, p4);

	fprintf(stdout, "free %p\n", p);
	priv_free(p);
	print_item();
	fprintf(stdout, "free %p\n", p2);
	priv_free(p2);
	print_item();
	fprintf(stdout, "free %p\n", p3);
	priv_free(p3);
	print_item();
	fprintf(stdout, "free %p\n", p4);
	priv_free(p4);
	print_item();
	exit(0);
}
*/


#if 0
#ifndef __TRANSFER_H__
#define __TRANSFER_H__

#include "setjmp.h"

typedef struct {
	jmp_buf env;
} context_t;

#define ACCEPT(entry)	(setjmp(entry))
#define REJOIN(entry)	(longjmp(entry))

/*
 * change from @src proc to @dst proc
 */
#define TRANSFER(src, dst)	{ if (!ACCEPT(src)) REJOIN(dst); }

#endif	// end of __TRANSFER_H__
#endif

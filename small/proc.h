
#ifndef __INIT_H
#define __INIT_H

/*
 * ucontext utils
 */
/*
typedef jmp_buf ucontext_t;
#define swapcontext(x, y)	{ if (!setjmp(x)) longjmp(y); }
#define getcontext(x)		{ setjmp(x); }
#define makecontext(x, func, unused) { }	// we could not access jmp_buf, so what to do ??
*/
/*
 * TODO can use this for example
#define getcontext(x)	priv_setjmp(x)
#define makecontext(x, fun, unused) {(x)[14] = (unsigned long)(fun); }
#define swapcontext(x, y) {	if(priv_setjmp((x))) ;	\
				else 			\
					priv_longjmp((y), 1);	}
*/

#if 0
#include	"setjmp.h"
//#include	"transfer.h"
#define NULL	0
#else
#include	<stdio.h>
#include 	<ucontext.h>
#include	<setjmp.h>	// for longjmp setjmp
#endif


/*
 * need to implement 
 * longjmp 
 * setjmp
 * printf
 * swapcontext
 * getcontext
 * makecontext
 */

typedef struct proc_t	proc_t;
struct proc_t
{
	ucontext_t	context;
	proc_t		*p_next;
	char 	*(*func)(void *arg);
};

void init_os(void);
void start_os(void);
int _spawn(char  *name, char  *(*func)(void *), 
		void  *arg, void *addr, unsigned wspsiz, unsigned rflags);
int yield(void);

#endif

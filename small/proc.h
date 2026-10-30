
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

int init(void);
#endif

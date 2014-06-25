
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

int init(void);
#endif

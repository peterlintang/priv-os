
#ifndef __K9INIT_H
#define __K9INIT_H

/*
 * ucontext utils
 */
/*
typedef jmp_buf ucontext_t;
#define swapcontext(x, y)	{ if (!setjmp(x)) longjmp(y); }
#define getcontext(x)		{ setjmp(x); }
#define makecontext(x, func, unused) { }	// we could not access jmp_buf, so what to do ??
*/

int K9init(void);
#endif

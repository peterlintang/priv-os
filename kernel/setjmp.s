
#include "setjmp.h"
/*
 * int setjmp(jmp_buf buffer)
 */
.global setjmp
.ent	setjmp
setjmp:
	.end setjmp

/*
 * void longjmp(jmp_buf buffer)
 */
.global longjmp
.ent	longjmp
longjmp:
	.end longjmp

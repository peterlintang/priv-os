#ifndef __MC_LIBC_PRINT_H
#define __MC_LIBC_PRINT_H

#include <stdarg.h>

#define IBASE	10
#define OBASE	8
#define XBASE	16
#define ILETTER	"0123456789"
#define OLETTER "01234567"
#define XLETTER "0123456789abcdef"
#define NULL	0

#define PUTC(c)	putchar(c)

int myprintf(const char *fmt, ...);

#endif	// end of __MC_LIBC_PRINT_H

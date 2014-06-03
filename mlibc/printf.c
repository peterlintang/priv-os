#include "../include/print.h"

char *ntoa(char *string, int value, 
		int len, int base, char *letter)
{
	int left;
	int last;
	int sign = 0;

	/*
	if (base != IBASE || base != OBASE || base != XBASE)
		return NULL;

	if ((base == IBASE) && (value < 0)) {
		sign = 1;
		value = -value;
	}
	*/

	string[--len] = '\0';
	do {
		left = value / base;
		last = value % base;
		string[--len] = letter[last];
	} while ((value = left) && len > 0);

	if (sign && (len > 0)) {
		string[--len] = '-';
	}

	return string + len;
}
char *itoa(char *string, int value, int len)
{
	return ntoa(string, value, len, IBASE, ILETTER);
}
char *otoa(char *string, int value, int len)
{
	return ntoa(string, value, len, OBASE, OLETTER);
}
char *xtoa(char *string, int value, int len)
{
	return ntoa(string, value, len, XBASE, XLETTER);
}

int doprint(const char *fmt, va_list ap)
{
	int ret = 0;
	int code;
	int value = 0;
	char *p = NULL;
	char string[24] = { 0 };

	for (; ; ) {
		while (((code = *fmt) != '\0') && (code != '%')) {
			PUTC(code);
			fmt++;
			ret++;
		}
		if (code == '\0')
			goto out;

		switch (code = *++fmt) {
			case 'd':
				value = va_arg(ap, int);
				p = itoa(string, value, 24);
				break;
			case 'o':
				value = va_arg(ap, int);
				p = otoa(string, value, 24);
				break;
			case 'x':
				value = va_arg(ap, int);
				p = xtoa(string, value, 24);
				break;
			case 's':
				p = va_arg(ap, char *);
				break;
			case 'c':
				value = va_arg(ap, int);
				string[0] = (char )value;
				string[1] = '\0';
				p = string;
				break;
			default:
				p = NULL;
				break;
		}
		while (p && (*p != '\0')) {
			PUTC(*p);
			p++, ret++;
		}
		fmt++;
	}

out:
	return ret;
}

/*
int sprintf(char *string, char *fmt, ...)
{
	int ret = 0;
	return ret;
}
*/

int myprintf(const char *fmt, ...)
{
	va_list ap;
	int ret = 0;

	puts("in printf\n");
	va_start(ap, fmt);
	ret = doprint(fmt, ap);
	va_end(ap);

	return ret;
}

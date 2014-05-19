
/*
 * minscanf(char *fmt, ...)
 * a minimun implement of scanf
 */

#include "print.h"
#include <stdio.h>
#include <stdlib.h>	// calloc
#define GETCH() getchar()
#define UNGETCH(c) ungetc(c, stdin)

char *get_dchars(char *s, int lim)
{
	int i;
	int c;

	s[--lim] = '\0';
	for (i = 0; i < lim; i++) {
		if (isdigit(c = GETCH())) {
			s[i] = c;
		} else {
			UNGETCH(c);
			break;
		}
	}
	if (i < lim)
		s[i] = '\0';
	return s;
}

int convert_d(char *s, int *p)
{
	if (!s || !p)
		return -1;
	for (*p = 0; *s != '\0'; s++)
		*p = *p * 10 + (*s - '0');

	return *p;
}

char *get_string(char **sp, int lim)
{
	int i;
	char *s = calloc(1, sizeof(char) * lim);
	if (s) {
		--lim;
		for (i = 0; i < lim; i++) {
			s[i] = GETCH();
			if (isspace(s[i]))
				break;
		}
	}
	if (i < lim)
		s = realloc(s, i + 1);
	s[i] = '\0';
	*sp = s;
	return s;
}


int doscanf(const char *fmt, va_list ap)
{
	int ret = 0;
	int code;
	int *pvalue = NULL;
	char **sp = NULL;
	char *pc = NULL;
	char string[24] = { 0 };

	for (; ; ) {
		while (((code = *fmt) != '\0') && (code != '%')) {
			fmt++;
			ret++;
		}
		if (code == '\0')
			goto out;

		switch (code = *++fmt) {
			case 'd':
				pvalue = va_arg(ap, int*);
				get_dchars(string, 24);
				convert_d(string, pvalue);
				ret++;
				break;
				/*
			case 'o':
				value = va_arg(ap, int);
				p = otoa(string, value, 24);
				break;
			case 'x':
				value = va_arg(ap, int);
				p = xtoa(string, value, 24);
				break;
				*/
			case 's':
				sp = va_arg(ap, char **);
				get_string(sp, 1024);
				ret++;
				break;
			case 'c':
				pc = va_arg(ap, char *);
				*pc = GETCH();
				break;
			default:
				break;
		}
		fmt++;
	}

out:
	return ret;
}

int minscanf(const char *fmt, ...)
{
	va_list ap;
	int ret = 0;

	va_start(ap, fmt);
	ret = doscanf(fmt, ap);
	va_end(ap);

	return ret;
}

/*
 * test
 */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int d = 0;
	char *s = NULL;
	char c = '\0';

	minscanf("%s %d %c", &s, &d, &c);
	fprintf(stdout, "%d %c\n", d, c);
	if (s)
		fprintf(stdout, "%s\n", s);
	else
		fprintf(stderr, "error geting str\n");
	exit(0);
}


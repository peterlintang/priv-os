


#if 0
#include	"setjmp.h"
//#include	"transfer.h"
#define NULL	0
#else
#include	<stdio.h>
#include 	<ucontext.h>
#include	<setjmp.h>	// for longjmp setjmp
#endif

#include "ring_buf.h"

Ringbuf_T ring_buf;
void *priv_malloc(size_t nbytes);
void *priv_calloc(int num, int size);
void priv_free(void *addr);


/*
 * need to implement 
 * longjmp 
 * setjmp
 * free
 * calloc
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
	char	*wsplim;
	char	*wsplow;
};

#define MAXPROCESS	16
static proc_t		procv[MAXPROCESS];
static proc_t		*firstfree;
static proc_t		*lastfree;
static proc_t		*thisp;	// current process ?
static proc_t		*prevp;
static proc_t		*lastp;
#define	FIRSTP		(&procv[0])	/* the first process will always be there */


static void _process(void)
{
	char	*ret_str = "default";
	proc_t	*p = thisp;

	ret_str = p->func(NULL);
	printf("%s\n", ret_str);

	p = thisp;

	if (prevp->p_next = thisp->p_next) {
		thisp = thisp->p_next;
	} else {
		thisp = FIRSTP;
		lastp = prevp;
	}
	if (firstfree) {
		lastfree->p_next = p;
		lastfree = p;
	} else {
		firstfree = p;
		lastfree  = p;
	}

	p->p_next = NULL;
	priv_free(p->wsplow);	// from stack memory
				
	if (p == thisp)
	{
		thisp = NULL;
		printf("exit last task\n");
	}
	else
	{
		swapcontext(&p->context, &thisp->context);
	}

	return ;
}


int
_spawn(char  *name, 
		char  *(*func)(void *), 
		void  *arg, 
		unsigned wspsiz, 
		unsigned rflags)
{
	int	j;
	proc_t		*p;

	if (!(p = firstfree)) {
		return -1;	/* no empty element in array */
	}
	firstfree = p->p_next;
	if (p->p_next == NULL) {
		lastfree = NULL;
	}

	if (thisp) {
		lastp->p_next = p;
		lastp = p;
	} else {
		thisp = lastp = p;
	}

	p->p_next = NULL;
	p->func = func;
	p->wsplow = (char *)priv_calloc(wspsiz, 1);
	p->wsplim = p->wsplow + wspsiz;	// stack from high to low
	getcontext(&p->context);
	p->context.uc_stack.ss_sp = p->wsplow;
	p->context.uc_stack.ss_size = wspsiz;
	makecontext(&p->context, _process, 0);
//	setjmp(p->context);
//	p->context[0].__jmpbuf[4] = (int)((((long)p->wsplim - 6 * 8) / 8) * 8);
//	p->context[0].__jmpbuf[5] = (int)_process;
	printf("spawing %s, fun: %p, stack: %p, %p\n", name, func, p->wsplow, p->wsplim);

	return 0;
}


static int yield()
{
	proc_t		*p;

	prevp = thisp;
	if (thisp->p_next) {
		p = thisp;
		thisp = thisp->p_next;
	} else {
		p = thisp;
		thisp = FIRSTP;
	}

	/*
	 * only the one process running !!!
	 */
	if (p == thisp) {
		return 0;
	}

	swapcontext(&p->context, &thisp->context);
	/*
	if (setjmp(p->context)) {
		return 0;
	}
	longjmp(thisp->context, 1);
	*/

	return 0;
}

char *producer(void *priv)
{
	for (unsigned char i = 0; ; i++)
	{
		printf("%s running, i: %u\n", __func__, i);
		Ringbuf_put(ring_buf, &i, 1);
		//sleep(1);
		yield();
	}
	return "producer exit";
}

char *consumer(void *priv)
{
	unsigned char data = 0;
	for (unsigned char i = 0; ; i++)
	{
		while (Ringbuf_get(ring_buf, &data, 1) > 0)
		{
			printf("%s running, data: %u\n", __func__, data);
		}
		//sleep(1);
		yield();
	}
	return "consumer exit";
}

char *hello(void *priv)
{
	return "hello exit";
}


int init(void)
{
	char i;
	char total = MAXPROCESS - 1;
	ucontext_t	init_context;

	thisp = NULL;
	lastp = NULL;
	prevp = NULL;
	for (i = 0; i < total; i++) {
		 procv[i].p_next = &procv[i + 1];
	}
	procv[i].p_next = NULL;
	firstfree = &procv[0];
	lastfree  = &procv[i];
	_spawn("producer", producer, NULL, 8 * 1024, 0);
	_spawn("producer", producer, NULL, 8 * 1024, 0);
	_spawn("consumer", consumer, NULL, 8 * 1024, 0);
	_spawn("hello", hello, NULL, 2 * 1024, 0);
	thisp->context.uc_link = &init_context;
	printf("call first process, proc_t: %d, jmp_buf: %d, ucontext_t: %d\n", (int)sizeof(struct proc_t), (int)sizeof(jmp_buf), (int)sizeof(ucontext_t));

	ring_buf = Ringbuf_create(1024);

	swapcontext(&init_context, &thisp->context);

//	longjmp(thisp->context, 1);	/* run the first process ( here is 0) */
	printf("should not be here\n");
	return 0;
}




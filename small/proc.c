

#include "proc.h"


#define MAXPROCESS	16
static proc_t		procv[MAXPROCESS];
static proc_t		*firstfree;
static proc_t		*lastfree;
static proc_t		*thisp;	// current process ?
static proc_t		*prevp;
static proc_t		*lastp;
#define	FIRSTP		(&procv[0])	/* the first process will always be there */

static ucontext_t	init_context;

static void _process(void)
{
	proc_t	*p = thisp;

	p->func();

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
//	priv_free(p->wsplow);	// from stack memory
				
	if (p == thisp)
	{
		//thisp = NULL;
		printf("exit last task\n");
	}
	else
	{
		swapcontext(&p->context, &thisp->context);
	}
	swapcontext(&thisp->context, &init_context);

	return ;
}


int
_spawn(char  *name, 
		void (*func)(void), 
		void *staddr,
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
//	p->wsplow = (char *)priv_calloc(wspsiz, 1);
	getcontext(&p->context);
	p->context.uc_stack.ss_sp = (char *)staddr; //p->wsplow;
	p->context.uc_stack.ss_size = wspsiz;
	makecontext(&p->context, _process, 0);
//	setjmp(p->context);
//	p->context[0].__jmpbuf[4] = (int)((((long)p->wsplim - 6 * 8) / 8) * 8);
//	p->context[0].__jmpbuf[5] = (int)_process;
	printf("spawing %s, fun: %p, stack: %p, %p\n", name, func, staddr, ((char *)staddr) + wspsiz);

	return 0;
}


int yield(void)
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


void init_os(void)
{
	char i;
	char total = MAXPROCESS - 1;

	thisp = NULL;
	lastp = NULL;
	prevp = NULL;
	for (i = 0; i < total; i++) {
		 procv[i].p_next = &procv[i + 1];
	}
	procv[i].p_next = NULL;
	firstfree = &procv[0];
	lastfree  = &procv[i];
}

void start_os(void)
{

	thisp->context.uc_link = &init_context;
	swapcontext(&init_context, &thisp->context);
//	longjmp(thisp->context, 1);	/* run the first process ( here is 0) */
	printf("should not be here\n");
}

int task_count(void)
{
	int count = 0;
	proc_t *p = FIRSTP;
	for (; p; p = p->p_next)
		count++;
	return count;
}



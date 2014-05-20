

#if 0
#include	"setjmp.h"
//#include	"transfer.h"
#define NULL	0
#else
#include	<stdio.h>
#include 	<stdlib.h>
#include 	<ucontext.h>
#include	<setjmp.h>	// for longjmp setjmp
#endif
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
	jmp_buf		env;
//	jmp_buf		context;
	ucontext_t	context;
	proc_t		*p_next;
	int		pid;
	int		p_state;
	char 	*(*func)(void *arg);
	void	*arg;
	char	*wsplim;
	char	*wsplow;
#define	DEAD		0/* => defunct			*/
#define	ZOMBIE		1/* => needs K9wait by parent	*/
#define	WAITING		2/* => blocked in K9request	*/
#define	RUNNING		3/* => normal operation		*/
#define	STIRRED		4/* => signalled while WAITING	*/
#define	INTERRUPTED	5/* => signalled while RUNNING	*/
};

#define MAXPROCESS	16
static proc_t		procv[MAXPROCESS];
static proc_t		*firstfree;
static proc_t		*lastfree;
static proc_t		*thisp;	// current process ?
static proc_t		*prevp;
static proc_t		*lastp;
#define	FIRSTP		(&procv[0])	/* the first process will always be there */

static int K9exit(void)
{
	longjmp(thisp->env, 1);

	/*
	 * will never get here
	 */
	return -1;
}

static void _process(void)
{
	char	*ret_str = "default";
	proc_t	*p = thisp;

		/* free task */
	if (setjmp(thisp->env)) {
		p = thisp;
		myprintf("%s\n", ret_str);
		if (prevp->p_next = thisp->p_next) {
			thisp = thisp->p_next;
		} else {
			thisp = FIRSTP;	/* at least one runable proc in array */
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
		free(p->wsplow);	// from stack memory
		swapcontext(&p->context, &thisp->context);

		/* never get here */
		return ;
	}
	ret_str = p->func(p->arg);
	longjmp(thisp->env, 1);
	/* never get here */
	return ;
//		longjmp(thisp->context, 1);	// call next process
}

static void	process(void);

int
_spawn(char  *name, 
		char  *(*func)(void *), 
		void  *arg, 
		unsigned wspsiz, 
		unsigned rflags)
{
	static int i = 0;
	int	j;
	proc_t		*p;

	i++;
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
		printf("set thisp to be : %p\n", p);
		thisp = lastp = p;
	}

	myprintf("spawinging...\n");
	p->p_next = NULL;
	p->arg = arg;
	p->func = func;
	p->wsplow = calloc(wspsiz, 1);
	p->wsplim = p->wsplow + wspsiz;	// stack from high to low
	printf("low: %p, lim: %p\n", p->wsplow, p->wsplim);
	getcontext(&p->context);
	p->context.uc_stack.ss_sp = p->wsplow;
	p->context.uc_stack.ss_size = wspsiz;
	makecontext(&p->context, _process, 0);
//	setjmp(p->context);
//	p->context[0].__jmpbuf[4] = (int)((((long)p->wsplim - 6 * 8) / 8) * 8);
//	p->context[0].__jmpbuf[5] = (int)_process;
	printf("spawing finish, fun: %p, i: %d\n", func, i);

	return 0;
}



//static int give_up_cpu()
static int K9yield()
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
	 * only the first process running !!!
	 * there should be at least process 0 
	 * and 1 in the runnable list
	 */
	if (p == thisp) {
	}

	swapcontext(&p->context, &thisp->context);
	/*
	if (setjmp(p->context)) {
		return 0;
	}
	longjmp(thisp->context, 1);
	*/

	/*
	 * will never get here
	 */
	return -1;
}

char *swapper(void *priv)
{
	myprintf("hello from swapper\n");
	while (1) {
		myprintf("yield from swapper\n");
		K9yield();
	}
	return "exit";
}

char *hello_k9(void *arg)
{
	myprintf("%s, %d\n", __func__, (int)arg);
	K9exit();
	return "good by, k9";
}

char *my_init(void *priv)
{
	myprintf("hello from my_init\n");
	_spawn("hello", hello_k9, 4, 1024, 0);
	while (1) {
		myprintf("yield from init\n");
		K9yield();
	}
	return "exit";
}

int K9init(void)
{
	int	i;
	int	total = MAXPROCESS - 1;
	ucontext_t	init_context;

	thisp = NULL;
	lastp = NULL;
	prevp = NULL;
	for (i = 0; i < total; i++) {
		 procv[i].p_next = &procv[i + 1];
		 procv[i].pid = i;
	}
	procv[i].p_next = NULL;
	procv[i].pid = i;
	firstfree = &procv[0];
	lastfree  = &procv[i];
	myprintf("spawn swapper\n");
	_spawn("swapper", swapper, NULL, 64 * 1024, 0);
	myprintf("spawn init\n");
	_spawn("init", my_init, NULL, 64 * 1024, 0);
	thisp->context.uc_link = &init_context;
	myprintf("call first process\n");
	swapcontext(&init_context, &thisp->context);

//	longjmp(thisp->context, 1);	/* run the first process ( here is 0) */
	myprintf("should not be here\n");
	return 0;
}




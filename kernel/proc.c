#include	"setjmp.h"
#include	"transfer.h"

#define NULL	0

typedef struct proc_t	proc_t;
struct proc_t
{
	jmp_buf		env;
	jmp_buf		context;
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

#define MAXPROCESS	4
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
	char	*ret_str = "call K9exit";
	proc_t	*p = thisp;

	if (setjmp(p->env)) {
out:
		/* free task */
		print("%s\n", ret_str);
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
		free(p->wsplim);	// from stack memory
		longjmp(thisp->context, 1);	// call next process
	}
	ret_str = p->func(p->arg);
	goto out;
}

static void	process(void);

int
_spawn(char  *name, 
		char  *(*func)(void *), 
		void  *arg, 
		unsigned wspsiz, 
		unsigned rflags)
{
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
	p->arg = arg;
	p->func = func;
	p->wsplow = malloc(wspsiz);
	p->wsplim = p->wsplow + 1;	// stack from high to low
	setjmp(p->context);
	p->context[14] = _process;
	p->context[13] = p->wsplim;

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

	if (setjmp(p->context)) {
		return 0;
	}
	longjmp(thisp->context, 1);

	/*
	 * will never get here
	 */
	return -1;
}

char *swapper(void *priv)
{
	while (1) {
		K9yield();
	}
	return "exit";
}

char *my_init(void *priv)
{
	return "exit";
}

int K9init(void)
{
	int	i;
	int	total = MAXPROCESS - 1;

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
	_spawn("swapper", swapper, NULL, 64, 0);
	_spawn("init", my_init, NULL, 64, 0);

	longjmp(thisp->context, 1);	/* run the first process ( here is 0) */
	return 0;
}

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
#define	DEAD		0/* => defunct			*/
#define	ZOMBIE		1/* => needs K9wait by parent	*/
#define	WAITING		2/* => blocked in K9request	*/
#define	RUNNING		3/* => normal operation		*/
#define	STIRRED		4/* => signalled while WAITING	*/
#define	INTERRUPTED	5/* => signalled while RUNNING	*/
};

static proc_t		procv[4];
static proc_t		*firstfree;
static proc_t		*lastfree;
static proc_t		*thisp;	// current process ?
static proc_t		*prevp;
static proc_t		*lastp;
#define	FIRSTP		(&procv[0])

static void _process(void)
{
	char	*ret_str = NULL;
	proc_t	*p = thisp;

	if (setjmp(p->env)) {
out:
		/* free task */
		print("%s\n", ret_str);
		thisp = thisp->p_next;
		lastfree->p_next = p;
		lastfree = p;
		p->p_next = NULL;
		free(p->context[13]);	// from stack memory
		longjmp(thisp->env);	// call next process
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
		return -1;
	}
	firstfree = p->p_next;

	lastp->p_next = p;
	lastp = p;
	p->p_next = NULL;
	p->arg = arg;
	p->func = func;
	p->context[14] = _process;

	return 0;
}



static int give_up_cpu()
{
}

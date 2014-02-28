#include	"setjmp.h"
#include	"transfer.h"

typedef struct proc_t	proc_t;
struct proc_t
{
	context_t	context;
	proc_t		*p_next;
	short		p_id;
	short		p_parent;
	short		p_pgrp;
	char		p_pri;
	char		p_tmp_pri;
	char		p_passed;
	int			p_tmp_pri_up;
	unsigned char	p_state;
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
static proc_t		*thisp;
static proc_t		*prevp;
static proc_t		*lastp;
#define	FIRSTP		(&procv[0])


static void	process(void);
static void	ready(proc_t *);
static void	unready(void);

int
_spawn(char  *name, char  *(*fn)(void *), void  *arg, unsigned wspsiz, unsigned rflags)
{
	register int		i;
	register proc_t		*p;

	return 0;
}

void __exit(char *result)
{
}

void _yield()
{
	register context_t	*from;
	register context_t	*to;

	from 	= &thisp->context;
	to 		= &thisp->p_next->context;
	thisp	= thisp->p_next;
	TRANSFER(from, to);
}

static void _process(void)
{
}

/*
**	ready		-- put a process onto the runnable list
*/
static void _ready(proc_t *p)
{
}

/*
**	unready		-- unlink current process from the runnable list
*/
static void unready(void)
{
}


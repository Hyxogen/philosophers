#include "philo.h"

#include <sys/errno.h>
#include <sysexits.h>
#include <stdlib.h>
#include <signal.h>

void
	*ph_safe_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		exit(EX_UNAVAILABLE);
	return (ptr);
}

void
	ph_process_exit(t_app *app, int status)
{
	(void) app;
	exit(status);
}

void
	ph_app_quit(t_app *app, int status)
{
	(void) status;
	ph_stop_philos(app);
}

void
	ph_stop_philos(t_app *app)
{
	pid_t	*childs;

	childs = app->childs;
	while (*childs)
	{
		if (kill(*childs, SIGTERM) < 0)
			if (errno != ESRCH)
				exit(EX_OSERR);
		childs++;
	}
}

#include "philo.h"

#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sysexits.h>
#include <unistd.h>

void
	*ph_safe_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		kill(0, SIGKILL);
	return (ptr);
}

void
	ph_sem_wait(t_app *app, sem_t *sem)
{
	int rc;

	while (1)
	{
		rc = sem_wait(sem);
		if (rc < 0)
		{
			if (errno != EINTR)
				ph_quit(app, EX_OSERR);
			continue;
		}
		break;
	}
}	

void
	ph_sem_post(t_app *app, sem_t *sem)
{
	int rc;

	rc = sem_post(sem);
	if (rc < 0)
		ph_quit(app, EX_OSERR);
}

void
	ph_usleep(t_app *app, useconds_t microseconds)
{
	int	rc;

	while (1)
	{
		rc = usleep(microseconds);
		if (rc < 0)
		{
			if (rc != EINTR)
				ph_quit(app, EX_OSERR);
			continue ;
		}
		break ;
	}
}

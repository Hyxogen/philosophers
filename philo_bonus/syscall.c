#include "philo.h"

#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#ifdef PH_LINUX
# include <semaphore.h>
#else
# include <sys/semaphore.h>
#endif
#include <sysexits.h>
#include <unistd.h>

int
	ph_sem_unlink(const char *name)
{
	int	rc;

	rc = sem_unlink(name);
	if (rc < 0)
	{
		if (errno != ENOENT && errno != EINVAL)
			exit(EX_OSERR);
	}
	return (rc);
}

sem_t
	*ph_sem_open(const char *name, int oflag, mode_t mode, int num)
{
	sem_t	*sem;

	sem = sem_open(name, oflag, mode, num);
	if (sem == SEM_FAILED)
		exit(EX_OSERR);
	return (sem);
}

void
	ph_sem_wait(t_app *app, sem_t *sem, t_exception_proc exception_proc)
{
	int	rc;

	while (1)
	{
		rc = sem_wait(sem);
		if (rc < 0)
		{
			if (errno != EINTR)
				exception_proc(app, EX_OSERR);
			continue ;
		}
		break ;
	}
}	

void
	ph_sem_post(t_app *app, sem_t *sem, t_exception_proc exception_proc)
{
	int	rc;

	rc = sem_post(sem);
	if (rc < 0)
		exception_proc(app, EX_OSERR);
}

void
	ph_usleep(t_app *app, useconds_t microseconds,
			t_exception_proc exception_proc)
{
	int	rc;

	while (1)
	{
		rc = usleep(microseconds);
		if (rc < 0)
		{
			if (rc != EINTR)
				exception_proc(app, EX_OSERR);
			continue ;
		}
		break ;
	}
}

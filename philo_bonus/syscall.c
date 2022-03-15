#include "philo.h"

#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/semaphore.h>
#include <sysexits.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>

int
	ph_sem_unlink(const char *name)
{
	int	rc;

	rc = sem_unlink(name);
	if (rc < 0)
	{
		if (errno != ENOENT)
		{
			fprintf(stderr, "Failed to sem_unlink(%d): %s\n", errno, strerror(errno));
			fprintf(stderr, "Argument was \"%s\"\n", name);
			exit(EX_OSERR);
		}
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
	ph_sem_wait(t_app *app, sem_t *sem, exception_proc_t exception_proc)
{
	int rc;

	while (1)
	{
		rc = sem_wait(sem);
		if (rc < 0)
		{
			if (errno != EINTR)
				exception_proc(app, EX_OSERR);
			continue;
		}
		break;
	}
}	

void
	ph_sem_post(t_app *app, sem_t *sem, exception_proc_t exception_proc)
{
	int rc;

	rc = sem_post(sem);
	if (rc < 0)
		exception_proc(app, EX_OSERR);
}

void
	ph_usleep(t_app *app, useconds_t microseconds, exception_proc_t exception_proc)
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

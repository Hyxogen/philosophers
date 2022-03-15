#include "philo.h"

#include <errno.h>
#include <sys/wait.h>
#include <sysexits.h>

int
	ph_waitpid(t_app *app, pid_t pid, int *stat_loc, int options)
{
	int	rc;

	while (1)
	{
		rc = waitpid(pid, stat_loc, options);
		if (rc < 0)
		{
			if (errno == EINTR)
			   continue;
			else
				ph_app_quit(app, EX_OSERR);
		}
		return (rc);
	}
}

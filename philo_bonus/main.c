#include "philo.h"

#include <stdlib.h>
#include <sys/wait.h>
#include <sysexits.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>

#include <stdio.h>
#include <semaphore.h>

void
	*ph_monitor(void *app_ptr)
{
	t_app	*app;
	int		index;

	app = app_ptr;
	index = app->attr->count;
	ph_usleep(app, app->attr->eat_time, ph_app_quit);
	while (index)
	{
		ph_sem_wait(app, app->eat_sem, ph_app_quit);
		index--;
	}
	ph_sem_wait(app, app->bin_sem, ph_app_quit);
	app->done = 1;
	ph_sem_post(app, app->bin_sem, ph_app_quit);
	ph_stop_philos(app);
	return (NULL);
}

int
	ph_wait_philos_exit(t_app *app)
{
	int	stat;

	if (ph_waitpid(app, 0, &stat, WUNTRACED) < 0)
		return (-1);
	ph_stop_philos(app);
	while (ph_waitpid(app, 0, NULL, WUNTRACED) > 0)
		continue ;
	return (WIFSIGNALED(stat) * EX_SOFTWARE);
}

int
	ph_wait_philos(t_app *app)
{
	int	rc;

	if (pthread_create(&app->monitor_thread, NULL, ph_monitor, app))
	{
		ph_stop_philos(app);
		return (EX_OSERR);
	}
	pthread_detach(app->monitor_thread);
	rc = ph_wait_philos_exit(app);
	ph_sem_wait(app, app->bin_sem, ph_app_quit);
	rc *= !app->done;
	ph_sem_post(app, app->bin_sem, ph_app_quit);
	return (rc);
}

int
	ph_run(t_app *app)
{
	unsigned int 	index;
	t_philo			philo;

	index = 0;
	app->childs = ph_safe_malloc(sizeof(*app->childs) * (app->attr->count + 1));
	memset(app->childs, 0, sizeof(*app->childs) * (app->attr->count + 1));
	ph_sem_wait(app, app->bin_sem, ph_process_exit);
	app->start = ph_get_now(app, ph_app_quit);
	while (index < app->attr->count)
	{
		ph_philo_new(&philo, index, app);
		app->childs[index] = ph_philo_start(&philo);
		if (app->childs[index] < 0)
		{
			ph_stop_philos(app);
			return (EX_OSERR);
		}
		index++;
	}
	ph_sem_post(app, app->bin_sem, ph_app_quit);
	return (ph_wait_philos(app));
}

int
	main(int argc, char **argv)
{
	int				rc;
	t_app 			app;
	t_philo_attr	attr;

	if (argc != 5 && argc != 6)
		return (EX_USAGE);
	if (ph_attr_setup(&attr, argc - 1, argv + 1))
		return (EX_USAGE);
	if (attr.min_eat == 0)
		return (EX_OK);
	ph_app_new(&app, &attr);
	rc = ph_run(&app);
	free(app.childs);
	return (rc);
}

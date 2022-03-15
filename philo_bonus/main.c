#include "philo.h"

#include <stdlib.h>
#include <sys/wait.h>
#include <sysexits.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>

void
	*ph_monitor(void *app_ptr)
{
	t_app	*app;
	int		index;

	app = app_ptr;
	index = app->attr.count;
	while (index)
	{
		ph_sem_wait(app, app->eat_sem, ph_app_quit);
		index--;
	}
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
	while (!ph_waitpid(app, 0, NULL, WUNTRACED))
		continue ;
	return (-WIFSIGNALED(stat));
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
	rc = ph_wait_philos_exit(app);
	pthread_join(app->monitor_thread, NULL);
	return (rc);
}

int
	ph_run(t_app *app)
{
	unsigned int 	index;
	t_philo			philo;

	index = 0;
	app->childs = ph_safe_malloc(sizeof(*app->childs) * (app->attr.count + 1));
	memset(app->childs, 0, sizeof(*app->childs) * (app->attr.count + 1));
	ph_sem_wait(app, app->global_sem, ph_app_quit); /* Will not properly close on error */
	while (index < app->attr.count)
	{
		ph_philo_new(&philo, index, app);
		app->childs[index] = ph_philo_start(&philo);
		if (app->childs[index] < 0)
		{
			ph_stop_philos(app);
			return (EX_OSERR);
		}
		index--;
	}
	app->start = ph_get_now(app, ph_app_quit);
	ph_sem_post(app, app->global_sem, ph_app_quit);
	return (ph_wait_philos(app));
}

int
	main(int argc, char **argv)
{
	int		rc;
	t_app 	app;

	if (argc != 5 && argc != 6)
		return (EX_USAGE);
	printf("%d\n", PSEMNAMLEN);
	ph_app_new(&app);
	if (ph_attr_setup(&app.attr, argc - 1, argv + 1))
		return (EX_USAGE);
	rc = ph_run(&app);
	free(app.childs);
	return (rc);
}

#include <stdlib.h>
#include "philo.h"
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <sys/time.h>

/* Always returns zero for norminette reasons */
int
	destroy_philos(t_philo *philos, size_t count)
{
	size_t	index;

	index = 0;
	while (index < count)
	{
		fork_destroy(&philos[index].lfork);
		index++;
	}
	return (0);
}

size_t
	setup_philos(t_philo **out, t_philo_attribs *attribs, t_app *app)
{
	size_t	index;

	*out = NULL;
	*out = malloc(sizeof(*(*out)) * attribs->count);
	if (!*out)
		return (0);
	index = 0;
	while (index < attribs->count)
	{
		if (!philo_new(&(*out)[index], index, attribs,
			&(*out)[(index + 1) % attribs->count]))
		{
			printf("Failed to create a new philosopher\n");
			destroy_philos(*out, index - 1);
			free(*out);
			return (0);
		}
		if (&(*out)[index].lfork == (*out)[index].rfork)
			(*out)[index].rfork = NULL;
		(*out)[index].app = app;
		index++;
	}
	return (index);
}

void
	wait_stop(const t_philo *philos, size_t count)
{
	size_t	index;

	index = 0;
	while (index < count)
	{
		pthread_join(philos[index].thread, NULL);
		index++;
	}
}

t_bool
	run(t_app *app, t_philo_attribs *attrib)
{
	t_philo	*philos;
	size_t	index;

	if (setup_philos(&philos, attrib, app) == 0)
		return (FALSE);
	index = 0;
	app->start = 0;
	pthread_mutex_lock(&app->global_mtx);
	while (index < attrib->count)
	{
		if (!philo_start(&philos[index]))
		{
			printf("Could not create all philosopher threads\n");
			return (destroy_philos(philos, attrib->count));
		}
		index += 1;
	}
	pthread_mutex_unlock(&app->global_mtx);
	wait_stop(philos, attrib->count);
	free(philos);
	return (TRUE);
}

int
	main(void)
{
	t_philo_attribs	attribs;
	t_app			app;

	attribs.count = 200;
	attribs.death_time = 180 * 1000;
	attribs.eat_time = 60 * 1000;
	attribs.sleep_time = 60 * 1000;
	attribs.min_eat = INT_MAX;
	app.should_stop = 0;
	if (!pthread_mutex_init(&app.global_mtx, NULL))
	{
		if (!run(&app, &attribs))
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	printf("Could not initialize the global mutex\n");
	return (EXIT_FAILURE);
}

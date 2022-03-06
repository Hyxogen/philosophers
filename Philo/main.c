#include "philo.h"
#include <stdlib.h>

int
	ph_attr_setup(t_philo_attr *attr, int optc, char **vals)
{
	int	success;

	success = 1;
	success *= ft_checked_atoi(&attr->count, vals[0]);
	success *= ft_checked_atoi(&attr->death_time, vals[1]);
	success *= ft_checked_atoi(&attr->eat_time, vals[2]);
	success *= ft_checked_atoi(&attr->sleep_time, vals[3]);
	attr->min_eat = -1;
	if (optc == 5)
		success *= ft_checked_atoi(&attr->min_eat, vals[4]);
	return (success);
}

int
	ph_destroy_philos(t_philo *philos, size_t count)
{
	size_t	index;

	index = 0;
	while (index < count)
	{
		ph_fork_destroy(&philos[index].lfork);
		index++;
	}
	return (0);
}

int
	ph_setup_philos(t_philo **out, t_philo_attr *attribs, t_app *app)
{
	size_t	index;

	*out = NULL;
	*out = malloc(sizeof(*(*out)) * attribs->count);
	if (!*out)
		return (-1);
	index = 0;
	while (index < attribs->count)
	{
		if (ph_philo_new(&(*out)[index], index, attribs,
			&(*out)[(index + 1) % attribs->count]) == -1)
		{
			destroy_philos(*out, index - 1);
			free(*out);
			return (-1);
		}
		if (&(*out)[index].lfork == (*out)[index].rfork)
			(*out)[index].rfork = NULL;
		(*out)[index].app = app;
		index++;
	}
	return (0);
}

void
	ph_wait_stop(const t_philo *philos, size_t count)
{
	size_t	index;

	index = 0;
	while (index < count)
	{
		pthread_join(philos[index].thread, NULL);
		index++;
	}
}

int
	ph_run(t_app *app, t_philo_attr *attrib)
{
	t_philo	*philos;
	size_t	index;

	if (ph_setup_philos(&philos, attrib, app) == -1)
		return (-1);
	index = 0;
	app->start = ph_get_now();
	while (index < attrib->count)
	{
		if (!ph_philo_start(&philos[index]))
		{
			ph_destroy_philos(philos, attrib->count);
			return (-1);
		}
		index += 1;
	}
	ph_wait_stop(philos, attrib->count);
	free(philos);
	return (0);
}

int
	main(int argc, char **argv)
{
	t_philo_attr	attr;
	t_app			app;

	if (argc != 5 && argc != 6)
		return (EXIT_FAILURE);
	if (!ph_attr_setup(&attr, argc - 1, argv + 1) || !ph_app_new(&app))
		return (EXIT_FAILURE);
	if (attr.min_eat == 0)
		return (EXIT_SUCCESS);
	ph_run(&app, &attr);
	return (EXIT_SUCCESS);
}

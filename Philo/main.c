#include "philo.h"
#include <stdlib.h>
#include <string.h>

int
	ph_attr_setup(t_philo_attr *attr, int optc, char **vals)
{
	int	success;

	success = 1;
	success *= ft_atoiu((unsigned int *) &attr->count, vals[0]);
	success *= ft_atoiu((unsigned int *) &attr->death_time, vals[1]);
	success *= ft_atoiu((unsigned int *) &attr->eat_time, vals[2]);
	success *= ft_atoiu((unsigned int *) &attr->sleep_time, vals[3]);
	attr->min_eat = -1;
	if (optc == 5)
		success *= ft_atoiu((unsigned int *) &attr->min_eat, vals[4]);
	attr->death_time *= 1000;
	attr->eat_time *= 1000;
	attr->sleep_time *= 1000;
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
	ph_setup_philos(t_philo **out, t_philo_attr *attr, t_app *app)
{
	size_t	index;

	*out = NULL;
	*out = malloc(sizeof(*(*out)) * attr->count);
	if (!*out)
		return (-1);
	index = 0;
	while (index < attr->count)
	{
		if (ph_philo_new(&(*out)[index], index, attr,
			&(*out)[(index + 1) % attr->count]) == -1)
		{
			ph_destroy_philos(*out, index - 1);
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
	ph_run(t_app *app, t_philo_attr *attr)
{
	t_philo	*philos;
	size_t	index;

	if (ph_setup_philos(&philos, attr, app) == -1)
		return (-1);
	index = 0;
	pthread_mutex_lock(&app->global_mtx);
	while (index < attr->count)
	{
		if (ph_philo_start(&philos[index]))
		{
			ph_destroy_philos(philos, attr->count);
			return (-1);
		}
		index += 1;
	}
	app->start = ph_get_now();
	pthread_mutex_unlock(&app->global_mtx);
	ph_wait_stop(philos, attr->count);
	ph_destroy_philos(philos, attr->count);
	free(philos);
	return (0);
}

int
	main(int argc, char **argv)
{
	t_philo_attr	attr;
	t_app			app;

	memset(&attr, 0, sizeof(attr));
	if (argc != 5 && argc != 6)
		return (EXIT_FAILURE);
	if (ph_attr_setup(&attr, argc - 1, argv + 1) || ph_app_new(&app, &attr))
		return (EXIT_FAILURE);
	if (attr.min_eat == 0)
		return (EXIT_SUCCESS);
	if (ph_run(&app, &attr))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

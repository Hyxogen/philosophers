#include "philo.h"
#include <string.h>

int
	ph_fork_new(t_fork *fork)
{
	if (pthread_mutex_init(&fork->mtx, NULL))
		return (-1);
	fork->user = NULL;
	return (0);
}

int
	ph_philo_new(t_philo *philo, int id, t_philo_attr *attr, t_philo *right)
{
	memset(philo, 0, sizeof(*philo));
	philo->id = id;
	philo->attr = attr;
	ph_fork_new(&philo->lfork);
	if (right)
			philo->rfork = &right->lfork;
	return (0);
}

int
	ph_app_new(t_app *app, t_philo_attr *attr)
{
	if (pthread_mutex_init(&app->global_mtx, NULL))
		return (-1);
	app->state = 0;
	app->start = 0;
	app->attr = attr;
	return (0);
}

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
	return (!success);
}

int
	ph_philo_start(t_philo *philo)
{
	if (pthread_create(&philo->thread, NULL, ph_philo_run, philo))
		return (-1);
	return (0);
}

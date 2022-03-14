#include "philo.h"
#include <unistd.h>

void
	ph_philo_drop(t_philo *philo, t_fork *fork)
{
	(void) philo;
	pthread_mutex_lock(&fork->mtx);
	fork->user = NULL;
	pthread_mutex_unlock(&fork->mtx);
}

int
	ph_philo_take(t_philo *philo, t_fork *fork)
{
	int	success;

	success = 0;
	if (!fork)
		return (0);
	pthread_mutex_lock(&fork->mtx);
	if (fork->user == NULL)
	{
		fork->user = philo;
		success = 1;
	}
	pthread_mutex_unlock(&fork->mtx);
	if (success)
		ph_inform(philo, ac_take_fork);
	return (success);
}

int
	ph_philo_wait(t_philo *philo)
{
	int	forks;

	forks = 0;
	ph_inform(philo, ac_start_think);
	while (forks < 2)
	{
		if (ph_philo_is_dead(philo))
			return (1);
		if (ph_philo_take(philo, &philo->lfork))
			forks++;
		if (ph_philo_take(philo, philo->rfork))
			forks++;
		usleep(200);
	}
	return (ph_philo_is_dead(philo));
}

/* Perhaps better to drop forks here */
int
	ph_philo_eat(t_philo *philo)
{
	long	now;

	ph_inform(philo, ac_start_eat);
	now = ph_get_now();
	philo->eat_count += 1;
	if (philo->attr->min_eat >= 0
		&& (long) philo->eat_count == philo->attr->min_eat)
	{
		pthread_mutex_lock(&philo->app->global_mtx);
		philo->app->state += 1;
		if (philo->app->state > 0
			&& (size_t) philo->app->state == philo->app->attr->count)
			philo->app->state = -1;
		pthread_mutex_unlock(&philo->app->global_mtx);
	}
	if (now < 0)
		return (-1);
	philo->last_eat = now;
	if (ph_philo_usleep(philo, philo->attr->eat_time))
		return (1);
	return (ph_philo_is_dead(philo));
}

int
	ph_philo_sleep(t_philo *philo)
{
	ph_inform(philo, ac_start_sleep);
	ph_philo_drop(philo, &philo->lfork);
	ph_philo_drop(philo, philo->rfork);
	if (ph_philo_usleep(philo, philo->attr->sleep_time))
		return (1);
	return (ph_philo_is_dead(philo));
}

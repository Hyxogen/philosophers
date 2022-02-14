#include "philo.h"

#define PHILO_INTER 50

int
	philo_sleep(t_philo *philo)
{
	suseconds_t	now;
	suseconds_t	death_time;
	suseconds_t	sleep_time;

	now = philo_get_timestamp();
	death_time = now + (philo->death_time - now + philo->last_eat);
	sleep_time = now + philo->sleep_time;
	while (now < sleep_time)
	{
		if (philo_is_dead(philo))
			break ;
		if (now + PHILO_INTER >= sleep_time)
		{
			usleep(sleep_time - now);
			break ;
		}
		usleep(PHILO_INTER);
		now = philo_get_timestamp();
	}
	if (philo_is_dead(philo))
	{
		philo_inform(philo, PHILO_DEATH);
		return (0);
	}
	return (1);
}

void
	philo_drop(t_philo *philo, t_fork *fork)
{
	pthread_mutex_lock(fork->mtx);
	fork->owner = NULL;
	pthread_mutex_unlock(fork->mtx);
	philo_inform(philo, PHILO_DROP_FORK);
}

void
	philo_eat(t_philo *philo)
{
	philo->state = PHILO_EATING;
	philo_inform(philo, PHILO_EATING);
	usleep(1000 * philo->eat_time);
	philo->last_eat = philo_get_timestamp();
	philo_drop(philo, philo->lfork);
	philo_drop(philo, philo->rfork);
}

int
	philo_try(t_philo *philo, t_fork *fork)
{
	int success;

	success = 0;
	pthread_mutex_lock(fork->mtx);
	if (fork->owner == NULL || fork->owner == philo)
	{
		fork->owner = philo;
		success = 1;
	}
	pthread_mutex_unlock(fork->mtx);
	if (success)
		philo_inform(philo, PHILO_TAKE_FORK);
	return (success);

int
	philo_try_eat(t_philo *philo)
{
	if (philo_try(philo, philo->lfork) && philo_try(philo, philo->rfork))
	{
		philo_eat(philo);
		return (1);
	}
	return (0);
}

void
	philo_think(t_philo *philo)
{
	if (philo->state == PHILO_THINKING)
		return ;
	philo->state = PHILO_THINKING;
	philo_inform(philo, PHILO_THINKING);
}

void
	philo_should_stop(t_philo *philo)
{
	return (philo_is_dead(philo) || philo->state == PHILO_ERR);
}

void
	philo_run(t_philo *philo)
{
	while (!philo_should_stop(philo))
	{
		if (philo_should_sleep(philo))
		{
			if (!philo_sleep(philo))
				return ;
		}
		if (!philo_try_eat(philo))
			philo_think(philo);
	}
}

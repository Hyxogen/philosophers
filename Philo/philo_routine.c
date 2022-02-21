/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_routine.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/21 14:27:20 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/21 14:39:13 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <sys/time.h>

#define PHILO_INTER 50

int
	philo_sleep(t_philo *philo)
{
	suseconds_t	now;
	suseconds_t	death_time;
	suseconds_t	sleep_time;

	now = philo_get_timestamp();
	death_time = now + (philo->attrib->death_time - now + philo->last_eat);
	sleep_time = now + philo->attrib->sleep_time;
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
		philo_inform(philo, philo_die);
		return (0);
	}
	return (1);
}

void
	philo_drop(t_philo *philo, t_fork *fork)
{
	pthread_mutex_lock(fork->mtx);
	fork->user = NULL;
	pthread_mutex_unlock(fork->mtx);
	philo_inform(philo, philo_drop_fork);
}

void
	philo_eat(t_philo *philo)
{
	philo->state = philo_eating;
	philo_inform(philo, philo_start_eat);
	usleep(1000 * philo->attrib->eat_time);
	philo->last_eat = philo->last_eat + philo->attrib->eat_time;
	philo_drop(philo, philo->lfork);
	philo_drop(philo, philo->rfork);
}

int
	philo_try(t_philo *philo, t_fork *fork)
{
	int success;

	success = 0;
	pthread_mutex_lock(fork->mtx);
	if (fork->user == NULL || fork->user == philo)
	{
		fork->user = philo;
		success = 1;
	}
	pthread_mutex_unlock(fork->mtx);
	if (success)
		philo_inform(philo, philo_take_fork);
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
	if (philo->state == philo_thinking)
		return ;
	philo->state = philo_thinking;
	philo_inform(philo, philo_start_think);
}

void
	philo_should_stop(t_philo *philo)
{
	return (philo_is_dead(philo) || philo->state == philo_err);
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

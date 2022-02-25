/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   routine.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/21 14:27:20 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/25 10:18:42 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

#define PHILO_INTER 50

int
	philo_get_timestamp(t_app *app)
{
	struct timeval	val;

	gettimeofday(&val, NULL);
	return (val.tv_usec - app->start);
}

t_bool
	philo_is_dead(t_philo *philo)
{
	int	now;

	now = philo_get_timestamp(philo->app);
	return (now - philo->last_eat >= philo->attrib->death_time);
}

t_bool
	philo_should_sleep(t_philo *philo)
{
	return (philo->state == st_sleeping);
}

void
	philo_inform(t_philo *philo, t_philo_action action)
{
	static char	*messages[] = {
		"%d %d has taken a fork",
		"%d %d is eating",
		"%d %d is sleeping",
		"%d %d is thinking",
		"%d %d died"
	};

	pthread_mutex_lock(philo->app->mtx);
	printf(messages[action], philo_get_timestamp(philo->app), philo->id);
	pthread_mutex_unlock(philo->app->mtx);
}

int
	philo_sleep(t_philo *philo)
{
	suseconds_t	now;
	suseconds_t	death_time;
	suseconds_t	sleep_time;

	now = philo_get_timestamp(philo->app);
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
		now = philo_get_timestamp(philo->app);
	}
	if (philo_is_dead(philo))
	{
		philo_inform(philo, ac_die);
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
}

void
	philo_eat(t_philo *philo)
{
	philo->state = st_eating;
	philo_inform(philo, ac_start_eat);
	usleep(1000 * philo->attrib->eat_time);
	philo->last_eat = philo->last_eat + philo->attrib->eat_time;
	philo_drop(philo, philo->lfork);
	philo_drop(philo, philo->rfork);
}

int
	philo_try(t_philo *philo, t_fork *fork)
{
	int	success;

	success = 0;
	pthread_mutex_lock(fork->mtx);
	if (fork->user == NULL || fork->user == philo)
	{
		fork->user = philo;
		success = 1;
	}
	pthread_mutex_unlock(fork->mtx);
	if (success)
		philo_inform(philo, ac_take_fork);
	return (success);
}

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
	if (philo->state == st_thinking)
		return ;
	philo->state = st_thinking;
	philo_inform(philo, ac_start_think);
}

t_bool
	philo_should_stop(t_philo *philo)
{
	return (philo_is_dead(philo) || philo->state == st_err);
}

void*
	philo_run(void *param)
{
	t_philo	*philo;

	philo = param;
	while (!philo_should_stop(philo))
	{
		if (philo_should_sleep(philo))
		{
			if (!philo_sleep(philo))
				return (NULL);
		}
		if (!philo_try_eat(philo))
			philo_think(philo);
	}
	return (NULL);
}

t_bool
	philo_start(t_philo *philo)
{
	if (_philo_create_thread(philo))
		return (TRUE);
	return (FALSE);
}

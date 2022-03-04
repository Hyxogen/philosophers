/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   routine.c                                          :+:    :+:            */ /*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/21 14:27:20 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/01 11:02:52 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

#define PHILO_INTER 50

t_bool
	philo_is_dead(t_philo *philo)
{
	long	now;

	now = philo_get_now();
	return (((now - philo->last_eat) / 1000) > (philo->attrib->death_time / 1000));
}

t_bool
	philo_should_stop(t_philo *philo)
{
	t_bool	res;

	res = FALSE;
	pthread_mutex_lock(&philo->app->global_mtx);
	if (philo->app->should_stop)
		res = TRUE;
	pthread_mutex_unlock(&philo->app->global_mtx);
	return (res || philo->state == st_err);
}

void
	philo_die(t_philo *philo)
{
	pthread_mutex_lock(&philo->app->global_mtx);
	if (philo->app->should_stop)
	{
		pthread_mutex_unlock(&philo->app->global_mtx);
		return ;
	}
	else
		philo->app->should_stop = TRUE;
	pthread_mutex_unlock(&philo->app->global_mtx);
	philo_inform(philo, ac_die);
}

void
	philo_inform(t_philo *philo, t_philo_action action)
{
	static char	*messages[] = {
		"%d %d has taken a fork\n",
		"%d %d is eating\n",
		"%d %d is thinking\n",
		"%d %d is sleeping\n",
		"%d %d died\n"
	};

	pthread_mutex_lock(&philo->app->global_mtx);
	if (!philo->app->should_stop || (action == ac_die && philo->app->should_stop))
		printf(messages[action], philo_get_timestamp(philo->app), philo->id);
	pthread_mutex_unlock(&philo->app->global_mtx);
}

int
	philo_sleep(t_philo *philo)
{
	philo->state = st_sleeping;	
	if (!philo_usleep(philo, philo->attrib->sleep_time))
	{
		philo_inform(philo, ac_die);
		return (0);
	}
	return (1);
}

void
	philo_drop(t_philo *philo, t_fork *fork)
{
	(void)philo;
	pthread_mutex_lock(&fork->mtx);
	fork->user = NULL;
	pthread_mutex_unlock(&fork->mtx);
}

void
	philo_eat(t_philo *philo)
{
	philo->state = st_eating;
	philo_inform(philo, ac_start_eat);
	philo->last_eat = philo_get_now();
	philo_usleep(philo, philo->attrib->eat_time);
	philo_inform(philo, ac_start_sleep);
	philo_drop(philo, &philo->lfork);
	philo_drop(philo, philo->rfork);
	if (!philo_usleep(philo, philo->attrib->sleep_time))
	{
			philo_die(philo);
			return ;
	}
	philo_think(philo);
}

int
	philo_try(t_philo *philo, t_fork *fork)
{
	int	success;

	success = 0;
	if (fork == NULL)
		return (0);
	pthread_mutex_lock(&fork->mtx);
	if (fork->user == NULL)
	{
		fork->user = philo;
		success = 1;
	}
	else if (fork->user == philo)
		success = 2;
	pthread_mutex_unlock(&fork->mtx);
	if (success == 1)
		philo_inform(philo, ac_take_fork);
	return (success);
}

int
	philo_try_eat(t_philo *philo)
{
	if (philo_try(philo, &philo->lfork) && philo_try(philo, philo->rfork))
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
	philo_inform(philo, ac_start_think);
	philo->state = st_thinking;
}

void
	*philo_run(void *param)
{
	t_philo	*philo;

	philo = param;
	pthread_mutex_lock(&philo->app->global_mtx);
	if (philo->app->start == 0)
			philo->app->start = philo_get_now();
	pthread_mutex_unlock(&philo->app->global_mtx);
	philo_think(philo);
	if (philo->id & 1)
		philo_usleep(philo, 500);
	philo->last_eat = philo_get_now();
	while (!philo_should_stop(philo))
	{
		philo_try_eat(philo);
		if (philo_is_dead(philo))
		{
			philo_die(philo);
			return (NULL);
		}
		ft_usleep(10000);
	}
	return (NULL);
}

t_bool
	philo_start(t_philo *philo)
{
	if (pthread_create(&philo->thread, NULL, philo_run, philo) == 0)
		return (TRUE);
//	pthread_detach(philo->thread);
	return (FALSE);
}

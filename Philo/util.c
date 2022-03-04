/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   util.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/07 09:42:54 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/01 10:51:06 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#ifndef PHILO_SLEEP_INTER
# define PHILO_SLEEP_INTER 10000
#else
# if PHILO_SLEEP_INTER <= 0
#  error PHILO_SLEEP_INTER must be a positive integer
# endif
#endif

int
	philo_hsleep(t_philo *philo, long time)
{
		time += philo_get_now();

		while (philo_get_now() < time)
				if (philo_is_dead(philo))
						return (0);
		return (1);
}

int
	philo_usleep(t_philo *philo, long microseconds)
{
	struct timeval	val;
	long			now;
	long			death;
	long			beg;

//	dprintf(2, "%d wil try to sleep for %ld\n", philo->id, microseconds);
	if (gettimeofday(&val, NULL) <= -1)
		return (0);
	now = (val.tv_sec * 1000000) + val.tv_usec;
	beg = now;
	microseconds += now;
	death = now + (now - philo->last_eat) + philo->attrib->death_time;
	while (now < microseconds && now <= death)
	{
		if (now + PHILO_SLEEP_INTER >= microseconds)
		{
			usleep(microseconds - now);
			break ;
		}
		usleep(PHILO_SLEEP_INTER);
		if (gettimeofday(&val, NULL) <= -1)
			return (0);
		now = (val.tv_sec * 1000000) + val.tv_usec;
	}
	if (now < 0)
		return (-1);
	if (now >= death)
	{
		dprintf(2, "%d died, death time:%ld, now:%ld, diff:%ld\n", philo->id, death, now, now - death);	
		return (0);
	}
	if (gettimeofday(&val, NULL) <= -1)
		return (0);
	now = (val.tv_sec * 1000000) + val.tv_usec;

//	dprintf(2, "%d slept for %ld\n", philo->id, now - beg);
	return (1);
}

int
	ft_usleep(long microseconds)
{
	long			now;
	struct timeval	val;

	if (gettimeofday(&val, NULL) <= -1)
		return (0);
	now = (val.tv_sec * 1000000) + val.tv_usec;
	microseconds += now;
	while (now < microseconds)
	{
		if (now + PHILO_SLEEP_INTER >= microseconds)
		{
			usleep(microseconds - now);
			break ;
		}
		usleep(PHILO_SLEEP_INTER);
		if (gettimeofday(&val, NULL) <= -1)
			return (0);
		now = (val.tv_sec * 1000000) + val.tv_usec;
	}
	return (now >= 0);
}

long
	philo_get_timestamp(t_app *app)
{
	long	now;

	now = philo_get_now();
	if (now < 0)
		return (-1);
	return ((now - app->start) / 1000);
}

long
	philo_get_now(void)
{
	struct timeval	val;

	if (gettimeofday(&val, NULL) <= -1)
		return (-1);
	return ((val.tv_sec * 1000000) + val.tv_usec);
}

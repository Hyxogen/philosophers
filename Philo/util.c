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
# define PHILO_SLEEP_INTER 1000
#endif

t_bool
	fork_new(t_fork *fork)
{
	fork->user = NULL;
	if (pthread_mutex_init(&fork->mtx, NULL))
		return (FALSE);
	return (TRUE);
}

void
	fork_destroy(t_fork *fork)
{
	fork->user = NULL;
	if (!pthread_mutex_destroy(&fork->mtx))
	{
		printf("Something went very wrong\n");
		raise(SIGABRT);
	}
}

t_bool
	philo_new(t_philo *philo, int number, t_philo_attribs *attrib,
			t_philo	*right) {
	memset(philo, 0, sizeof(*philo));
	philo->id = number;
	philo->attrib = attrib;
	philo->state = st_start;
	philo->rfork = &right->lfork;
	if (!fork_new(&philo->lfork))
		return (FALSE);
	return (TRUE);
}

int
	philo_usleep(t_philo *philo, long microseconds)
{
	long	now;
	long	death;
	long	sleep;

	now = philo_get_now();
	microseconds += now;
	death = now + philo->last_eat + philo->attrib->death_time;
	while (now >= 0 && now < microseconds && now <= death)
	{
		if (now + PHILO_SLEEP_INTER >= microseconds)
		{
			usleep(microseconds - now);
			break ;
		}
		usleep(PHILO_SLEEP_INTER);
		now = philo_get_now();
	}
	if (now < 0)
		return (-1);
	if (now >= death)
		return (0);
	return (1);
}

int
	ft_usleep(long microseconds)
{
	long	now;
	struct timeval val;

	if (gettimeofday(&val, NULL) <= -1)
		return (0);
	now = (val.tv_sec * 1000000) + val.tv_usec;
	microseconds += now;
	while (now >= 0 && now < microseconds)
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
	philo_get_now(void)
{
	struct timeval val;

	if (gettimeofday(&val, NULL) <= -1)
		return (-1);
	return ((val.tv_sec * 1000000) + val.tv_usec);
}

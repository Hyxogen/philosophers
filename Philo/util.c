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
# define PHILO_SLEEP_INTER 10
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
	philo_usleep(t_philo *philo, suseconds_t microseconds)
{
	struct timeval	val;
	suseconds_t		now;
	suseconds_t		death;
	suseconds_t		sleep;

	gettimeofday(&val, NULL);
	now = val.tv_usec;
	microseconds += now;
	death = now + philo->last_eat + philo->attrib->death_time;
	while (now < microseconds && now <= death)
	{
		if (now + PHILO_SLEEP_INTER >= microseconds)
		{
			usleep(microseconds - now);
			break ;
		}
		usleep(PHILO_SLEEP_INTER);
		gettimeofday(&val, NULL);
		if (val.tv_usec < 0)
			return (0);
		now = val.tv_usec;
	}
	if (now >= death)
		return (0);
	return (1);
}

int
	ft_usleep(useconds_t microseconds)
{
	struct timeval	val;
	useconds_t		now;

	gettimeofday(&val, NULL);
	if (val.tv_usec < 0)
		return (0);
	now = val.tv_usec;
	microseconds += now;
	while (now < microseconds)
	{
		if (now + PHILO_SLEEP_INTER >= microseconds)
		{
			usleep(microseconds - now);
			break ;
		}
		usleep(PHILO_SLEEP_INTER);
		gettimeofday(&val, NULL);
		if (val.tv_usec < 0)
			return (0);
		now = val.tv_usec;
	}
	return (1);
}

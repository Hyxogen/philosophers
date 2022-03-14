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
#include <sys/time.h>

#ifndef PH_SLEEP_INTER
# define PH_SLEEP_INTER 1000
#else
# if PH_SLEEP_INTER <= 0
#  error PH_SLEEP_INTER must be a positive integer
# endif
#endif

int
	ph_philo_is_dead(t_philo *philo)
{
	long	now;

	now = ph_get_now();
	if (now < 0)
		return (-1);
	return (now >= (philo->last_eat + philo->attr->death_time));
}

int
	ph_fork_destroy(t_fork *fork)
{
	if (pthread_mutex_destroy(&fork->mtx))
		return (-1);
	return (0);
}

long
	ph_get_now(void)
{
	struct timeval	val;

	if (gettimeofday(&val, NULL) == -1)
		return (-1);
	return ((val.tv_sec * 1000000) + val.tv_usec);
}

long
	ph_get_timestamp(t_app *app)
{
	long	now;

	now = ph_get_now();
	if (now < 0)
		return (-1);
	return ((now - app->start) / 1000);
}

int
	ph_philo_usleep(t_philo *philo, long microseconds)
{
	long	now;
	long	death;	

	now = ph_get_now();
	death = philo->last_eat + philo->attr->death_time;
	microseconds += now;
	while (now >= 0 && now <= microseconds && now < death)
	{
		if (now + PH_SLEEP_INTER >= death)
		{
			usleep(death - now);
			return (1);
		}
		else if (now + PH_SLEEP_INTER >= microseconds)
			usleep(microseconds - now);
		else
			usleep(PH_SLEEP_INTER);
		now = ph_get_now();
	}
	if (now < 0)
		return (-1);
	return (now >= death);
}

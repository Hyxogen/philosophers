/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   setup.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/07 09:42:54 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/25 10:20:53 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <string.h>
#include <stdlib.h>

void
	fork_new(t_fork *fork)
{
	fork->user = NULL;
	fork->mtx = malloc(sizeof(*fork->mtx));
	pthread_mutex_init(fork->mtx, NULL);
}

t_bool
	_philo_create_thread(t_philo *philo)
{
	if (pthread_create(philo->thread, NULL, philo_run, philo) == 0)
		return (TRUE);
	return (FALSE);
}

void
	*philo_new(t_philo *philo, int number, t_philo_attribs *attrib,
			t_fork *forks) {
	memset(philo, 0, sizeof(*philo));
	philo->id = number;
	philo->attrib = attrib;
	philo->state = st_start;
	philo->lfork = forks;
	philo->rfork = forks + 1;
	return (philo);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_setup.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/07 09:42:54 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/10 08:23:46 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <string.h>

void	_philo_init_variables(t_philo *philo,
			int number, int philo_count, t_philo_settings settings) {
	philo->m_id = number;
	philo->m_philo_count = philo_count;
	philo->m_settings = settings;
}

t_bool	_philo_create_thread(t_philo *philo) {
	if (pthread_create(&philo->m_thread, NULL, philo_run, philo) == 0)
		return (TRUE);
	return (FALSE);
}

t_philo	*philo_create(int number, int philo_count, t_philo_settings settings) {
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	if (philo == NULL)
		return (NULL);
	_philo_init_variables(philo, number, philo_count, settings);
}

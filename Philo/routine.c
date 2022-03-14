/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   routine.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/21 14:27:20 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/01 11:02:52 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

void
	ph_inform(t_philo *philo, t_action action)
{
	static const char	*messages[] = {
		"%d %d has taken a fork\n",
		"%d %d is eating\n",
		"%d %d is thinking\n",
		"%d %d is sleeping\n",
		"%d %d died\n",
		"%d %d has dropped a fork\n"
	};

	pthread_mutex_lock(&philo->app->global_mtx);
	if ((philo->app->state >= 0 && (size_t) philo->app->state < philo->app->attr->count) || (action == ac_die && philo->app->state < 0))
		printf(messages[action], ph_get_timestamp(philo->app), philo->id);
	pthread_mutex_unlock(&philo->app->global_mtx);
}
	
int
	ph_stop(t_app *app)
{
	int	result;

	result = 0;
	pthread_mutex_lock(&app->global_mtx);
	if (app->state < 0 || (size_t) app->state == app->attr->count)
		result = 1;
	app->state = -1;
	pthread_mutex_unlock(&app->global_mtx);
	return (result);
}

int
	ph_should_stop(t_app *app)
{
	int	should_stop;

	pthread_mutex_lock(&app->global_mtx);
	should_stop = (app->state < 0 || (size_t) app->state == app->attr->count);
	pthread_mutex_unlock(&app->global_mtx);
	return (should_stop);
}

void
	*ph_philo_run(void *param)
{
		t_philo	*philo;
		t_app	*app;
		int		state;

		philo = param;
		app = philo->app;
		pthread_mutex_lock(&app->global_mtx);
		pthread_mutex_unlock(&app->global_mtx);
		philo->last_eat = ph_get_now();
		if (philo->id & 1)
				usleep(app->attr->eat_time);
		while (!ph_should_stop(app))
		{
			state = ph_philo_wait(philo);
			if (!state)
				state = ph_philo_eat(philo);
			if (!state)
				state = ph_philo_sleep(philo);
			if (state < 0)
				return (NULL);
			if (state == 1 && !(ph_stop(app)))
			{
				ph_inform(philo, ac_die);
				return (NULL);
			}
		}
		return (NULL);
}

int
	ph_philo_start(t_philo *philo)
{
	if (pthread_create(&philo->thread, NULL, ph_philo_run, philo))
		return (-1);
	return (0);
}

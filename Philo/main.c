/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/24 10:20:39 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/25 10:20:29 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "philo.h"
#include <unistd.h>
#include <limits.h>

t_bool
	setup_forks(t_fork **out, int count)
{
	t_fork	*fork;

	*out = malloc(sizeof(t_fork) * count + 1);
	if (!*out)
		return (FALSE);
	fork = *out;
	while (fork < *out + count)
	{
		fork_new(fork);
		fork++;
	}
	*fork = *(*out);
	return (TRUE);
}

t_bool
	setup_philos(t_philo **out, t_philo_attribs *attribs,
		t_fork *forks, t_app *app)
{
	t_philo	*philo;

	*out = malloc(sizeof(t_philo) * attribs->count);
	if (!*out)
		return (FALSE);
	philo = *out;
	while (philo <= *out)
	{
		philo_new(philo, philo - *out, attribs, forks);
		philo->app = app;
		forks++;
		philo++;
	}
	return (TRUE);
}

t_bool
	setup(t_philo **philos, t_philo_attribs *attribs, t_app *app)
{
	t_fork	*forks;

	if (!setup_forks(&forks, attribs->count))
		return (FALSE);
	if (!setup_philos(philos, attribs, forks, app))
	{
		free(forks);
		return (FALSE);
	}
	return (TRUE);
}

t_bool
	start(t_philo_attribs *attribs, t_app *app)
{
	t_philo	*philos;
	int		index;
	int		count;

	count = attribs->count;
	if (!setup(&philos, attribs, app))
		return (FALSE);
	index = 0;
	while (index < count)
	{
		philo_start(&philos[index]);
		index += 2;
	}
	index = 1;
	usleep(100);
	while (index < count)
	{
		philo_start(&philos[index]);
		index += 2;
	}
	sleep(500);
	return (TRUE);
}

int
	main(void)
{
	t_philo_attribs	attrib;
	t_app			app;
	struct timeval	val;

	gettimeofday(&val, NULL);
	attrib.count = 5;
	attrib.death_time = 800;
	attrib.eat_time = 200;
	attrib.sleep_time = 200;
	attrib.min_eat = INT_MAX;
	app.start = val.tv_usec;
	start(&attrib, &app);
	return (EXIT_SUCCESS);
}

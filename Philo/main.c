/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */ /*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/24 10:20:39 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/01 11:00:10 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "philo.h"
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <sys/time.h>

/* Always returns zero for norminette reasons */
int
	destroy_philos(t_philo *philos, size_t count)
{
	size_t	index;

	index = 0;
	while (index < count)
	{
		fork_destroy(&philos[index].lfork);
		index++;
	}
	return (0);
}

size_t
	setup_philos(t_philo **out, t_philo_attribs *attribs, t_app *app)
{
	size_t	index;

	*out = NULL;
	*out = malloc(sizeof(*(*out)) * attribs->count);
	if (!*out)
		return (0);
	index = 0;
	while (index < attribs->count)
	{
		if (!philo_new(&(*out)[index], index, attribs,
			&(*out)[(index + 1) % attribs->count]))
		{
			printf("Failed to create philo\n");
			destroy_philos(*out, index - 1);
			free(*out);
			return (0);
		}
		if (&(*out)[index].lfork == (*out)[index].rfork)
			(*out)[index].rfork = NULL;
		(*out)[index].app = app;
		index++;
	}
	return (index);
}

void
	wait_stop(const t_philo *philos, size_t count)
{
	size_t	index;

	index = 0;
	while (index < count)
	{
		pthread_join(philos[index].thread, NULL);
		index++;
	}
}

t_bool
	run(t_app *app, t_philo_attribs *attrib)
{
	t_philo	*philos;
	size_t	index;

	if (setup_philos(&philos, attrib, app) == 0)
		return (FALSE);
	index = 0;
	while (index < attrib->count)
	{
		if (!philo_start(&philos[index]))
			return (destroy_philos(philos, attrib->count));
		index += 1;
	}
	wait_stop(philos, attrib->count);
	return (TRUE);
}

int
	main(void)
{
	t_philo_attribs	attribs;
	t_app			app;

	attribs.count = 3;
	attribs.death_time = 650 * 1000;
	attribs.eat_time = 200 * 1000;
	attribs.sleep_time = 150 * 1000;
	attribs.min_eat = INT_MAX;
	app.start = philo_get_now();
	app.should_stop = 0;
	pthread_mutex_init(&app.global_mtx, NULL);
	if (!run(&app, &attribs))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

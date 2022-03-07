#include "philo.h"
#include <string.h>

int
	ph_fork_new(t_fork *fork)
{
		if (pthread_mutex_init(&fork->mtx, NULL))
				return (-1);
		fork->user = NULL;
		return (0);
}

int		ph_philo_new(t_philo *philo, int id, t_philo_attr *attr, t_philo *right)
{
		memset(philo, 0, sizeof(*philo));
		philo->id = id;
		philo->attr = attr;
		if (right)
				philo->rfork = &right->lfork;
		return (0);
}

int
	ph_app_new(t_app *app, t_philo_attr *attr)
{
		if (pthread_mutex_init(&app->global_mtx, NULL))
			return (-1);
		app->state = 0;
		app->start = 0;
		app->attr = attr;
		return (0);
}


#include "philo.h"

#include <stddef.h>
#include <unistd.h>
#include <sysexits.h>
#include <stdio.h>

#ifndef PH_EXPIRE_CHECK_INTERVAL
# define PH_EXPIRE_CHECK_INTERVAL 10
#endif

void
	ph_inform(t_philo *philo, t_action action)
{
	const static char	*messages[] = {
		"%d %d has taken a fork\n",
		"%d %d is eating\n",
		"%d %d is thinking\n",
		"%d %d is sleeping\n",
		"%d %d died\n"
	};
	t_app				*app;

	app = philo->app;
	ph_sem_wait(app, app->global_sem, ph_process_exit);
	printf(messages[action],
		ph_get_timestamp(app, ph_process_exit), philo->id + 1);
	if (action != ac_die)
		ph_sem_post(app, app->global_sem, ph_process_exit);
}

void
	*ph_philo_expire_routine(void *philo_ptr)
{
	long	death_time;
	long	now;
	long	last_eat;
	t_app	*app;
	t_philo	*philo;

	philo = philo_ptr;
	death_time = philo->attr->death_time;
	app = philo->app;
	while (1)
	{
		now = ph_get_now(app, ph_process_exit);
		ph_sem_wait(app, philo->bin_sem, ph_process_exit);
		last_eat = philo->last_eat;
		ph_sem_post(app, philo->bin_sem, ph_process_exit);
		if (now >= last_eat + death_time)
		{
			ph_inform(philo, ac_die);
			ph_process_exit(app, EX_OK);
		}
		ph_usleep(app, 10, ph_process_exit);
	}
	return (NULL);
}

static void
	ph_philo_set_last_eat(t_app *app, t_philo *philo, long time)
{
	ph_sem_wait(app, philo->bin_sem, ph_process_exit);
	philo->last_eat = time;
	ph_sem_post(app, philo->bin_sem, ph_process_exit);
}

int
	ph_philo_main_routine(t_philo *philo)
{
	long	sleep_time;
	long	eat_time;
	t_app	*app;

	sleep_time = philo->attr->sleep_time + 200;
	eat_time = philo->attr->eat_time;
	app = philo->app;
	while (1)
	{
		ph_inform(philo, ac_think);
		ph_sem_wait(app, philo->fork_sem, ph_process_exit);
		ph_inform(philo, ac_take_fork);
		ph_inform(philo, ac_take_fork);
		ph_inform(philo, ac_eat);
		ph_philo_set_last_eat(app, philo, ph_get_now(app, ph_process_exit));
		philo->eat_count += (philo->eat_count <= app->attr->min_eat);
		ph_interval_sleep(app, eat_time);
		if (philo->eat_count == app->attr->min_eat)
			ph_sem_post(app, app->eat_sem, ph_process_exit);
		ph_inform(philo, ac_sleep);
		ph_sem_post(app, philo->fork_sem, ph_process_exit);
		ph_interval_sleep(app, sleep_time);
	}
	return (0);
}

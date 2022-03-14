#include "philo.h"

#include <stddef.h>
#include <unistd.h>
#include <sysexits.h>

void
	*ph_philo_expire_routine(t_philo *philo)
{
	long	death_time;
	long	now;
	long	time_to_death;
	t_app	*app;

	death_time = philo->last_eat;
	app = philo->app;
	while (1)
	{
		now = ph_get_now(app);
		time_to_death = philo->last_eat + death_time - now;
		if (time_to_death <= 0)
			ph_philo_die(philo);
		ph_usleep(app, time_to_death);
	}
	return (NULL);
}

void
	*ph_philo_monitor_routine(t_app *app)
{
	ph_sem_wait(app, app->dms_sem);
	ph_quit(app, EX_OK);
	return (NULL);
}

int
	ph_philo_main_routine(t_philo *philo)
{
	long	sleep_time;
	long	eat_time;
	t_app	*app;

	sleep_time = philo->attr->sleep_time;
	eat_time = philo->attr->eat_time;
	app = philo->app;
	while (1)
	{
		ph_inform(philo, ac_think);
		ph_sem_wait(app, philo->fork_sem);
		ph_inform(philo, ac_eat);
		ph_usleep(app, eat_time);
		ph_inform(philo, ac_sleep);
		ph_usleep(app, sleep_time);
	}
	return (0);
}

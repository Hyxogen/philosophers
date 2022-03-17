#include "philo.h"

#include <unistd.h>
#include <pthread.h>
#include <sysexits.h>
#include <sys/stat.h>
#include <string.h>


#include <stdio.h>

void
	ph_philo_new(t_philo *philo, int id, t_app *app)
{
	memset(philo, 0, sizeof(*philo));
	philo->app = app;
	philo->attr = app->attr;
	philo->id = id;
	philo->fork_sem = app->fork_sem;
}

int
	ph_attr_setup(t_philo_attr *attr, int optc, char **vals)
{
	int	success;

	memset(attr, 0, sizeof(*attr));
	success = 1;
	success *= ft_atoiu((unsigned int *) &attr->count, vals[0]);
	success *= ft_atoiu((unsigned int *) &attr->death_time, vals[1]);
	success *= ft_atoiu((unsigned int *) &attr->eat_time, vals[2]);
	success *= ft_atoiu((unsigned int *) &attr->sleep_time, vals[3]);
	if (optc == 5)
		success *= ft_atoiu((unsigned int *) &attr->min_eat, vals[4]);
	else
		attr->min_eat = -1;
	attr->death_time *= 1000;
	attr->eat_time *= 1000;
	attr->sleep_time *= 1000;
	return (!success);
}

int
	ph_app_new(t_app *app, t_philo_attr *attr)
{
	memset(app, 0, sizeof(*app));
	ph_sem_unlink(PH_GLOBAL_SEM_NAME);
	ph_sem_unlink(PH_EAT_SEM_NAME);
	ph_sem_unlink(PH_FORK_SEM_NAME);
	ph_sem_unlink(PH_APP_BIN_SEM);
	app->attr = attr;
	app->global_sem = ph_sem_open(PH_GLOBAL_SEM_NAME, O_CREAT, S_IRWXU, 1);
	app->eat_sem = ph_sem_open(PH_EAT_SEM_NAME, O_CREAT, S_IRWXU, app->attr->count);
	app->fork_sem = ph_sem_open(PH_FORK_SEM_NAME, O_CREAT, S_IRWXU, app->attr->count / 2);
	app->bin_sem = ph_sem_open(PH_APP_BIN_SEM, O_CREAT, S_IRWXU, 1);
	return (0);
}

int
	ph_philo_start(t_philo *philo)
{
	t_app	*app;
	pid_t	pid;

	app = philo->app;
	pid = fork();
	if (pid < 0)
		return (-1);
	else if (pid)
		return (pid);
	ph_sem_wait(app, app->bin_sem, ph_process_exit);
	ph_sem_wait(app, app->eat_sem, ph_process_exit);
	ph_sem_post(app, app->bin_sem, ph_process_exit);
	philo->last_eat = ph_get_now(app, ph_process_exit);
	if (pthread_create(&philo->expire_thread, NULL, ph_philo_expire_routine, philo))
		ph_process_exit(philo->app, EX_OSERR);
	ph_philo_main_routine(philo);
	return (-1);
}

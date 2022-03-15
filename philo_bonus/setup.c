#include "philo.h"

#include <sys/_types/_s_ifmt.h>
#include <unistd.h>
#include <pthread.h>
#include <sysexits.h>
#include <sys/stat.h>
#include <string.h>

void
	ph_philo_new(t_philo *philo, int id, t_app *app)
{
	memset(philo, 0, sizeof(*philo));
	philo->app = app;
	philo->attr = &app->attr;
	philo->id = id;
}

int
	ph_attr_setup(t_philo_attr *attr, int optc, char **vals)
{
	int	success;

	success = 1;
	success *= ft_atoiu((unsigned int *) &attr->count, vals[0]);
	success *= ft_atoiu((unsigned int *) &attr->death_time, vals[1]);
	success *= ft_atoiu((unsigned int *) &attr->eat_time, vals[2]);
	success *= ft_atoiu((unsigned int *) &attr->sleep_time, vals[3]);
	attr->min_eat = -1; if (optc == 5)
		success *= ft_atoiu((unsigned int *) &attr->min_eat, vals[4]);
	attr->death_time *= 1000;
	attr->eat_time *= 1000;
	attr->sleep_time *= 1000;
	return (!success);
}

int
	ph_app_new(t_app *app)
{
	memset(app, 0, sizeof(*app));
	ph_sem_unlink(PH_GLOBAL_SEM_NAME);
	ph_sem_unlink(PH_EAT_SEM_NAME);
	ph_sem_unlink(PH_FORK_SEM_NAME);
	app->global_sem = ph_sem_open(PH_GLOBAL_SEM_NAME, O_CREAT, S_IRWXU, 1);
	app->eat_sem = ph_sem_open(PH_EAT_SEM_NAME, O_CREAT, S_IRWXU, app->attr.count);
	app->fork_sem = ph_sem_open(PH_FORK_SEM_NAME, O_CREAT, S_IRWXU, app->attr.count / 2);
	return (0);
}

int
	ph_philo_start(t_philo *philo)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (-1);
	else if (pid)
		return (pid);
	if (pthread_create(&philo->expire_thread, NULL, ph_philo_expire_routine, philo))
		ph_philo_quit(philo->app, EX_OSERR);
	ph_philo_main_routine(philo);
	return (-1);
}

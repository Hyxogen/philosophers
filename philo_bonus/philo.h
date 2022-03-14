#ifndef PHILO_H
# define PHILO_H

#include <sys/_types/_useconds_t.h>
# include <sys/types.h>
# include <semaphore.h>

typedef enum e_action {
	ac_take_fork,
	ac_eat,
	ac_think,
	ac_sleep,
	ac_die
}	t_action;

typedef struct s_philo_attr {
	long	count;
	long	min_eat;
	long	death_time;
	long	sleep_time;
	long	eat_time;
}	t_philo_attr;

typedef struct s_app {
	sem_t			*global_sem;
	sem_t			*eat_sem;
	sem_t			*dms_sem;
	long			start;
	t_philo_attr	attr;
}	t_app;

typedef struct s_philo {
	long			last_eat;
	long			eat_count;
	t_philo_attr	*attr;
	t_app			*app;
	sem_t			*fork_sem;
	int				id;
	pthread_t		expire_thread;
	pthread_t		monitor_thread;
}	t_philo;

void	*ph_safe_malloc(size_t size);

int		ph_app_new(t_app *app);
int		ph_philo_new(t_philo *philo, const t_app *app);
int		ph_setup_attrib(t_philo_attr *attr, int count, char **vals);

int		ph_run(t_app *app);

long	ph_get_now(t_app *app);
long	ph_get_timestamp(const t_app *app);

int		ph_inform(t_philo *philo, t_action action);

int		ph_philo_start(t_philo *philo);

void	*ph_philo_expire_routine(t_philo *philo);
void	*ph_philo_monitor_routine(t_app *app);
int		ph_philo_main_routine(t_philo *philo);

void	ph_philo_die(t_philo *philo);
void	ph_quit(t_app *app, int code);

void	ph_usleep(t_app *app, useconds_t microseconds);
void	ph_sem_wait(t_app *app, sem_t *sem);
void	ph_sem_post(t_app *app, sem_t *sem);
#endif

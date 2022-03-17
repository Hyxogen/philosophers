#ifndef PHILO_H
# define PHILO_H

# include <sys/_types/_useconds_t.h>
# include <sys/types.h>
# include <semaphore.h>

# ifndef PH_DMS_SEM_NAME
#  define PH_DMS_SEM_NAME "phdmssem"
# endif
# ifndef PH_FORK_SEM_NAME
#  define PH_FORK_SEM_NAME "phforksem"
# endif
# ifndef PH_EAT_SEM_NAME
#  define PH_EAT_SEM_NAME "pheatsem"
# endif
# ifndef PH_GLOBAL_SEM_NAME
#  define PH_GLOBAL_SEM_NAME "phglobalsem"
# endif
# ifndef PH_APP_BIN_SEM
#  define PH_APP_BIN_SEM "phappbinsem"
# endif

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
	sem_t			*fork_sem;
	sem_t			*bin_sem;
	long			start;
	pid_t			*childs;
	t_philo_attr	*attr;
	int				done;
	pthread_t		monitor_thread;
}	t_app;

typedef struct s_philo {
	long			last_eat;
	long			eat_count;
	t_philo_attr	*attr;
	t_app			*app;
	sem_t			*fork_sem;
	int				id;
	pthread_t		expire_thread;
}	t_philo;

typedef void	(*t_exception_proc)(t_app*,int);

/* Only called in main process */
void	*ph_safe_malloc(size_t size);

int		ph_app_new(t_app *app, t_philo_attr *attr);
int		ph_attr_setup(t_philo_attr *attr, int count, char **vals);
void	ph_philo_new(t_philo *philo, int id, t_app *app);

int		ph_run(t_app *app);
int		ph_wait_philos(t_app *app);

void	ph_inform(t_philo *philo, t_action action);

int		ph_philo_start(t_philo *philo);
void	ph_stop_philos(t_app *app);

void	*ph_philo_expire_routine(void *philo_ptr);
int		ph_philo_main_routine(t_philo *philo);

void	ph_philo_die(t_philo *philo);

void	ph_process_exit(t_app *app, int status);
void	ph_app_quit(t_app *app, int status);

void	ph_usleep(t_app *app, useconds_t microseconds,
			t_exception_proc exception_proc);
long	ph_get_now(t_app *app, t_exception_proc exception_proc);
long	ph_get_timestamp(t_app *app, t_exception_proc exception_proc);
void	ph_sem_wait(t_app *app, sem_t *sem, t_exception_proc exception_proc);
void	ph_sem_post(t_app *app, sem_t *sem, t_exception_proc exception_proc);

/* Only called in main process */
sem_t	*ph_sem_open(const char *name, int oflag, mode_t mode, int num);
int		ph_sem_unlink(const char *name);
int		ph_waitpid(t_app *app, pid_t pid, int *stat_loc, int options);

int		ft_atoiu(unsigned int *out, const char *str);
#endif

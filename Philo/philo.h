/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/07 09:28:59 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/01 10:51:16 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <unistd.h>

# ifndef TRUE
#  define TRUE 1
# endif

# ifndef FALSE
#  define FALSE 0
# endif

typedef struct s_fork			t_fork;
typedef struct s_philo			t_philo;
typedef struct s_philo_attr		t_philo_attr;
typedef struct s_app			t_app;

typedef enum e_action {
	ac_take_fork,
	ac_start_eat,
	ac_start_think,
	ac_start_sleep,
	ac_die
}	t_action;

struct s_philo_attr {
	size_t			count;
	int				min_eat;
	long			death_time;
	long			sleep_time;
	long			eat_time;
};

struct s_fork {
	t_philo			*user;
	pthread_mutex_t	mtx;
};

struct s_app {
	int				state;
	long			start;
	t_philo_attr	*attr;
	pthread_mutex_t	global_mtx;
};

struct s_philo {
	int							id;
	long						last_eat;
	unsigned int				eat_count;
	t_philo_attr				*attr;
	t_fork						lfork;
	t_fork						*rfork;
	pthread_t					thread;
	t_app						*app;
};

int		ph_fork_new(t_fork *fork);
int		ph_philo_new(t_philo *philo, int id,
			t_philo_attr *attr, t_philo *right);
int		ph_app_new(t_app *app, t_philo_attr *attr);

int		ph_attr_setup(t_philo_attr *attr, int optc, char **vals);

int		ph_fork_destroy(t_fork *fork);

long	ph_get_now(void);
long	ph_get_timestamp(t_app *app);
int		ph_should_stop(t_app *app);
int		ft_atoiu(unsigned int *out, const char *str);

int		ph_sleep(long microseconds);

void	*ph_philo_run(void *param);

void	ph_inform(t_philo *philo, t_action action);

int		ph_philo_is_dead(t_philo *philo);

int		ph_philo_start(t_philo *philo);
void	ph_philo_drop(t_philo *philo, t_fork *fork);
int		ph_philo_take(t_philo *philo, t_fork *fork);
int		ph_philo_wait(t_philo *philo);
int		ph_philo_eat(t_philo *philo);
int		ph_philo_sleep(t_philo *philo);

int		ph_philo_usleep(t_philo *philo, long microseconds);
#endif

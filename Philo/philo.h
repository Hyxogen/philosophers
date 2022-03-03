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
# include <sys/time.h>
# include <unistd.h>

# ifndef TRUE
#  define TRUE 1
# endif

# ifndef FALSE
#  define FALSE 0
# endif

typedef struct s_fork			t_fork;
typedef struct s_philo			t_philo;
typedef struct s_philo_attribs	t_philo_attribs;
typedef int						t_bool;
typedef struct s_app			t_app;

typedef enum e_philo_state {
	st_err = -1,
	st_start,
	st_sleeping,
	st_eating,
	st_thinking
}	t_philo_state;

typedef enum e_philo_action {
	ac_take_fork,
	ac_start_eat,
	ac_start_think,
	ac_start_sleep,
	ac_die
}	t_philo_action;

struct s_philo_attribs {
	size_t	count;
	int	death_time;
	int	min_eat;
	int	sleep_time;
	int	eat_time;
};

struct s_fork {
	t_philo			*user;
	pthread_mutex_t	mtx;
};

struct s_app {
	pthread_mutex_t	global_mtx;
	long		start;
	t_bool		should_stop;
};

struct s_philo {
	int				id;
	int				eat_count;
	long			last_eat;
	t_philo_attribs	*attrib;
	t_philo_state	state;
	t_fork			lfork;
	t_fork			*rfork;
	pthread_t		thread;
	t_app			*app;
};

void			_philo_init_variables(t_philo *philo,
					int number, int philo_count, t_philo_attribs settings);
t_bool			_philo_create_thread(t_philo *philo);
t_bool			philo_new(t_philo *philo, int number, t_philo_attribs *attrib,
					t_philo	*right);
void			philo_destroy(t_philo *philo);

void			philo_inform(t_philo *philo, t_philo_action action);
t_philo_state	philo_get_astate(int id, int philo_count, int cycle);

t_bool			philo_take_left(t_philo *philo);
t_bool			philo_take_right(t_philo *philo);
t_bool			philo_take_forks(t_philo *philo);

t_bool			philo_sleep(t_philo *philo);
void			philo_eat(t_philo *philo);
void			philo_think(t_philo *philo);

t_bool			philo_is_dead(t_philo *philo);
t_bool			philo_should_sleep(t_philo *philo);

/* internal routine */
void			*philo_run(void *param);
/* create thread and run */
t_bool			philo_start(t_philo *philo);

long			philo_get_timestamp(t_app *app);
long			philo_get_now(void);

t_bool			fork_new(t_fork *fork);
t_fork			fork_create(void);
void			fork_destroy(t_fork *fork);

int				ft_usleep(long microseconds);
int				philo_usleep(t_philo *philo, long microseconds);

#endif

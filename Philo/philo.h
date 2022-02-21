/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/07 09:28:59 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/21 14:38:27 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>

typedef struct s_fork			t_fork;
typedef struct s_philo			t_philo;
typedef struct s_philo_attribs	t_philo_attribs;
typedef int						t_bool;

typedef enum e_philo_state {
	philo_err = -1,
	philo_sleeping,
	philo_eating,
	philo_thinking
}	t_philo_state;

typedef enum e_philo_action {
	philo_take_fork,
	philo_drop_fork,
	philo_start_eat,
	philo_start_think,
	philo_start_sleep,
	philo_die
}	t_philo_action;

struct s_philo_attribs {
	int	death_time;
	int	min_eat;
	int	sleep_time;
	int	eat_time;
};

struct s_fork {
	t_philo			*user;
	pthread_mutex_t	*mtx;
};

struct s_philo {
	int				id;
	int				philo_count;
	int				eat_count;
	suseconds_t		last_eat;
	t_philo_attribs	*attrib;
	t_philo_state	state;
	t_fork			*lfork;
	t_fork			*rfork;
	pthread_t		*thread;
};

void			_philo_init_variables(t_philo *philo,
					int number, int philo_count, t_philo_attribs settings);
t_bool			_philo_create_thread(t_philo *philo);
t_philo			*philo_create(int number, int philo_count, t_philo_attribs settings);
void			philo_destroy(t_philo *philo);

void			philo_inform(t_philo *philo, t_philo_action action);
t_philo_state	philo_get_astate(int id, int philo_count, int cycle);

t_bool			philo_take_left(t_philo *philo);
t_bool			philo_take_right(t_philo *philo);
t_bool			philo_take_forks(t_philo *philo);

t_bool			philo_sleep(t_philo *philo);
t_bool			philo_eat(t_philo *philo);
t_bool			philo_think(t_philo *philo);

t_fork			fork_create(void);
t_bool			fork_destroy(t_fork fork);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/07 09:28:59 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/14 13:13:56 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>

typedef pthread_mutex_t*	t_fork;
typedef int					t_bool;

typedef enum e_philo_state {
	philo_sleeping,
	philo_eating,
	philo_thinking
}	t_philo_state;

typedef struct s_philo_attribs {
	int	m_death_time;
}	t_philo_attribs;

typedef struct s_philo {
	int			id;
	int			philo_count;
	int			death_time;
	int			sleep_time;
	int			eat_time;
	int			eat_count;
	int			min_eat;
	int			cycle;
	t_fork		lfork;
	t_fork		rfork;
	pthread_t	*thread;
}	t_philo;

void			_philo_init_variables(t_philo *philo,
					int number, int philo_count, t_philo_attribs settings);
t_bool			_philo_create_thread(t_philo *philo);
t_philo			*philo_create(int number, int philo_count, t_philo_attribs settings);
void			philo_destroy(t_philo *philo);

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

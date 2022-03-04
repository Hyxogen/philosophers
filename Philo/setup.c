#include "philo.h"
#include <string.h>

t_bool
	fork_new(t_fork *fork)
{
	fork->user = NULL;
	if (pthread_mutex_init(&fork->mtx, NULL))
		return (FALSE);
	return (TRUE);
}

void
	fork_destroy(t_fork *fork)
{
	fork->user = NULL;
	pthread_mutex_destroy(&fork->mtx);
}

t_bool
	philo_new(t_philo *philo, int number, t_philo_attribs *attrib,
			t_philo	*right) {
	memset(philo, 0, sizeof(*philo));
	philo->id = number;
	philo->attrib = attrib;
	philo->state = st_start;
	philo->rfork = &right->lfork;
	if (!fork_new(&philo->lfork))
		return (FALSE);
	return (TRUE);
}

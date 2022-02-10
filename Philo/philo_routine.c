#include "philo.h"

void
	philo_run(t_philo *philo)
{
	while (1)
	{
		if (philo_should_eat(philo))
		{
			philo_take_forks(philo);
			philo_eat(philo);
		}
		else if (philo_should_wait(philo))
			philo_wait_actively(philo);
		else if (philo_should_sleep(philo))
		{
			philo_drop_forks(philo);
			philo_sleep(philo);
		}
}

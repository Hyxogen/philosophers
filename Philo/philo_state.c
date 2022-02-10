#include "philo.h"
#include <stdio.h>

t_philo_state
	philo_get_astate(int id, int philo_count, int cycle)
{
	int	wait_cycle;

	wait_cycle = (cycle + id - (philo_count - 2)) % philo_count;
	if (wait_cycle == 0)
		return (philo_thinking);
	if (wait_cycle < 0)
		return (~wait_cycle & 1);
	return (wait_cycle & 1);
}

int main(void) {
	int philo_count;
	int cycle;
	int philo;
	int state;
	
	philo_count = 7;
	for (cycle = 0; cycle < 15; cycle++) {
		for (philo = 0; philo < philo_count; philo++) {
			state = philo_get_astate(philo, philo_count, cycle);
			//printf("[%d]", state);
			printf("[%c]", state == philo_sleeping ? 'S' : state == philo_eating ? 'E' : 'T');
		}
		printf("\n");
	}
}

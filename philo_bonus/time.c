#include "philo.h"

#include <sysexits.h>
#include <sys/time.h>

long
	ph_get_now(t_app *app, t_exception_proc exception_proc)
{
	struct timeval	val;

	if (gettimeofday(&val, NULL) < 0)
		exception_proc(app, EX_OSERR);
	return ((val.tv_sec * 1000000) + val.tv_usec);
}

long
	ph_get_timestamp(t_app *app, t_exception_proc exception_proc)
{
	return ((ph_get_now(app, exception_proc) - app->start) / 1000L);
}

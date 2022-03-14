#include "philo.h"

#include <stdlib.h>
#include <sysexits.h>

int
	main(int argc, char **argv)
{
	t_app			app;

	if (argc != 5 && argc != 6)
		return (EX_USAGE);
	ph_app_new(&app);
	if (ph_setup_attrib(&app.attr, argc - 1, argv + 1))
		return (EX_USAGE);
	if (ph_run(&app))
		return (EX_OSERR);
	return (EX_OK);
}


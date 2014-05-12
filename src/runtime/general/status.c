#include "runtime/general/status.h"
#include "state.h"

#include <stdio.h>

int 
runtime_command_status (struct command_args_t *args)
{
	fprintf(stdout, "          State: %s\n", state_to_string(*(args->state)));
	fprintf(stdout, "Executable path: %s\n", *(args->exec_path));

	return 0;
}

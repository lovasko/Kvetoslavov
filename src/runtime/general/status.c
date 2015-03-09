#include <stdio.h>

#include "runtime/general/status.h"
#include "state/state.h"

int 
runtime_command_status(struct command_args* cmd_args)
{
	fprintf(stdout, "            PID: %d\n", *(cmd_args->pid));
	fprintf(stdout, "Executable path: %s\n", *(cmd_args->exec_path));

	return 0;
}


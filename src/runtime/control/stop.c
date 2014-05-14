#include "runtime/control/stop.h"
#include "runtime/util.h"
#include "state.h"

#include <sys/ptrace.h>
#include <sys/types.h>

int 
stop (pid_t pid)
{
	if (ptrace(PT_KILL, pid, 0, 0) == 0)
		return 0;
	else
		return 1;
}

int 
runtime_command_stop (struct command_args_t *args)
{
	if (runtime_process_exists(*(args->pid)) == 0)
	{
		if (stop(*(args->pid)) == 0)
		{
			*(args->state) = DEFAULT;
			*(args->pid) = -1;
			*(args->exec_path) = NULL;	
			/* TODO nullate breakpoint lists */

			fprintf(stdout, "Debuggee terminated.\n");
		}
		else
		{
			fprintf(stderr, "Unable to terminate the debuggee.\n");
		}
	}
	else
	{
		fprintf(stderr, "Process does not exitst.\n");	
	}

	return 0;
}


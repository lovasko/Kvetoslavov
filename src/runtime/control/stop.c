#include <sys/types.h>
#include <sys/ptrace.h>

#include "runtime/control/stop.h"
#include "runtime/util.h"
#include "state/state.h"

static int 
stop(pid_t pid)
{
	if (ptrace(PT_KILL, pid, 0, 0) == 0)
		return 0;
	else
		return 1;
}

int 
runtime_command_stop(struct command_args* cmd_args)
{
	if (runtime_process_exists(*(cmd_args->pid)) == 0) {
		if (stop(*(cmd_args->pid)) == 0) {
			*(cmd_args->state) = DEFAULT;
			*(cmd_args->pid) = -1;
			*(cmd_args->exec_path) = NULL;	
			/* TODO nullate breakpoint lists */

			fprintf(stdout, "Debuggee terminated.\n");
		} else {
			fprintf(stderr, "Unable to terminate the debuggee.\n");
		}
	} else {
		fprintf(stderr, "Process does not exitst.\n");	
	}

	return 0;
}


#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

#include <libgen.h>

#include "runtime/breakpoint/breakpoint.h"
#include "runtime/control/control.h"
#include "state/state.h"

/* TODO add error handling and reporting */
static int 
start(struct breakpoint** bp, char* path, pid_t* pid)
{
	char* args[2];
	int wait_status;
	struct breakpoint* node;
	
	*pid = fork();
	if (*pid == 0) {
		/* if we are in the child, allow the tracing */
		ptrace(PT_TRACE_ME, 0, 0, 0);
				
		args[0] = basename(path);
		args[1] = NULL;
		
		 /* rewrite the process with desired executable */
		execv(path, args);
	} else {
		/* stop after first instruction */
		wait(&wait_status);

		/* insert breakpoints and save the original byte */
		node = *bp;
		while (node != NULL) { 
			node->orig = ptrace(PT_READ_I, *pid, (caddr_t)node->addr, 0);

			/* replace first byte for 0xCC */
			node->oxcc = (node->orig & (unsigned long)0xFFFFFFFFFFFFFF00) | 0xCC;
			ptrace(PT_WRITE_I, *pid, (caddr_t)node->addr, node->oxcc);
			
			node = node->next;
		}
		
		/* carry on */
		ptrace(PT_CONTINUE, *pid, (caddr_t)1, 0);
		wait(&wait_status);
		return after_wait(wait_status, pid, bp);
	}

	return 0;
}

int 
runtime_command_run(struct command_args* cmd_args)
{
	if (start(cmd_args->bp_head, *(cmd_args->exec_path), cmd_args->pid) == 0) {
		*(cmd_args->state) = DEFAULT;
		*(cmd_args->pid) = -1;
		*(cmd_args->exec_path) = NULL;
	} else {
		*(cmd_args->state) = RUNNING;
	}

	return 0;
}


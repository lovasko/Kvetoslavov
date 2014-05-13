#include "runtime/control/run.h"
#include "runtime/control/common.h"
#include "runtime/breakpoint/breakpoint.h"
#include "state.h"

#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <libgen.h>

int 
start (struct breakpoint_t **bp, char *path, pid_t *pid)
{
	char *args[2];
	int wait_status;
	struct breakpoint_t *node;
	
	*pid = fork();
	if (*pid == 0)
	{
		/* if we are in the child, allow the tracing */
		ptrace(PT_TRACE_ME, 0, 0, 0);
				
		args[0] = basename(path);
		args[1] = NULL;
		
		 /* rewrite the process with desired executable */
		execv(path, args);
	}
	else
	{
		/* stop after first instruction */
		wait(&wait_status);

		/* insert breakpoints and save the original byte */
		/*
		node = *bp;
		while (node != NULL)
		{ 
			node->orig = ptrace(PT_READ_I, *pid, node->addr, 0);

			/* replace first byte for 0xCC */
			/*node->oxcc = (node->orig & (unsigned long)0xFFFFFFFFFFFFFF00) | 0xCC;
			ptrace(PT_WRITE_I, *pid, node->addr, node->oxcc);
			
			node = node->next;
		}
		*/
		
		/* carry on */
		ptrace(PT_CONTINUE, *pid, (caddr_t)1, 0);
		wait(&wait_status);
		return after_wait(wait_status, pid, bp);
	}
}

int 
runtime_command_run (struct command_args_t *args)
{
	if (start(args->head, *(args->exec_path), args->pid) == 0)
	{
		*(args->state) = DEFAULT;
		*(args->pid) = -1;
		*(args->exec_path) = NULL;
	}
	else
		*(args->state) = RUNNING;

	return 0;
}


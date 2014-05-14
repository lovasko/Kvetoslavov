#include "runtime/control/detach.h"
#include "state.h"

#include <sys/ptrace.h>
#include <sys/types.h>

static int 
detach (pid_t pid, struct breakpoint_t *bp)
{
	struct breakpoint_t *node; 
	
	/* remove all breakpoints */
	node = bp;
	while (node != NULL)
	{
		ptrace(PT_WRITE_I, pid, bp->addr, node->orig);
		node = node->next;
	}
	
	if (ptrace(PT_DETACH, pid, 0, 0) == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int 
runtime_command_detach (struct command_args_t *args)
{
	/* TODO breakpoints */
	if (detach(*(args->pid), NULL) == 0)
		printf("Detach was successful.\n");
	else
		printf("Detach was unsuccessful.\n");

	/*
	remove_all_breakpoints(&head);
	remove_all_breakpoints(&head_add);
	remove_all_breakpoints(&head_remove);
	*/

	*(args->state) = DEFAULT;
	*(args->pid) = -1;
	*(args->exec_path) = NULL;

	return 0;
}


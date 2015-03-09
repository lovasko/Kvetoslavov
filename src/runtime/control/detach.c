#include <sys/types.h>
#include <sys/ptrace.h>

#include "runtime/control/detach.h"
#include "state/state.h"

static int 
detach(pid_t pid, struct breakpoint* bp)
{
	struct breakpoint* node; 
	
	/* remove all breakpoints */
	node = bp;
	while (node != NULL) {
		ptrace(PT_WRITE_I, pid, (caddr_t)bp->addr, node->orig);
		node = node->next;
	}
	
	if (ptrace(PT_DETACH, pid, 0, 0) == 0)
		return 0;
	else
		return 1;
}

int 
runtime_command_detach(struct command_args* cmd_args)
{
	/* TODO breakpoints */
	if (detach(*(cmd_args->pid), NULL) == 0)
		printf("Detach was successful.\n");
	else
		printf("Detach was unsuccessful.\n");

	/*
	remove_all_breakpoints(&head);
	remove_all_breakpoints(&head_add);
	remove_all_breakpoints(&head_remove);
	*/

	*(cmd_args->state) = DEFAULT;
	*(cmd_args->pid) = -1;
	*(cmd_args->exec_path) = NULL;

	return 0;
}


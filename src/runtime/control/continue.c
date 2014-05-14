#include "runtime/control/continue.h"
#include "runtime/breakpoint/breakpoint.h"
#include "runtime/control/common.h"
#include "state.h"

#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/types.h>
#include <machine/reg.h>

static int 
resume (struct breakpoint_t **bp, pid_t pid, struct breakpoint_t *to_add, struct breakpoint_t *to_remove)
{
	int wait_status;
	struct reg regs;
	struct breakpoint_t *node, *node_remove, *bp_node, *node_add;
	int is_breakpoint;
	
	/* if no breakpoints present, 
	 *     just simply continue */
	if (bp == NULL)
	{
		ptrace(PT_CONTINUE, pid, (caddr_t)1, 0);
		
		wait(&wait_status);
		return after_wait(wait_status, &pid, bp);
	}
	
	/* handle breakpoints */
	ptrace(PT_GETREGS, pid, &regs, 0);

	/* fix the breakpoint address with original instruction byte */
	node = *bp;
	
	is_breakpoint = 0; 
	
	while (node != NULL)
	{
		if (node->addr == regs.r_eip - 1)
		{
			ptrace(PT_WRITE_I, pid, node->addr, node->orig);
			is_breakpoint = 1;
			break;
		}
		node = node->next;
	}
	
	/* do this only, if we were standing on a breakpoint */
	if (is_breakpoint == 1)
	{	
		/* move one instruction back */
		regs.r_eip -= 1;
		ptrace(PT_SETREGS, pid, &regs, 0);
		
		/* perform the original instruction and react to possible changes*/
		ptrace(PT_STEP, pid, (caddr_t)1, 0);	
		wait(&wait_status);
		
		/* check whether the single step caused anything */
		if (WIFEXITED(wait_status))
		{
			printf("Child exited on breakpoint in file %s at line %d.\n", node->path, node->line);
			return 0;
		}
		
		/* put the breakpoint instruction 0xCC back */
		ptrace(PT_WRITE_I, pid, node->addr, node->oxcc);
		
		/* remove breakpoints ought to be removed */
		node_remove = to_remove;
		while (node_remove != NULL)
		{
			/* find the same breakpoint in bp, because it has the valid orig field */
			bp_node = *bp;
			while (bp_node != NULL)
			{
				if (bp_node->addr == node_remove->addr)
				{
					ptrace(PT_WRITE_I, pid, bp_node->addr, bp_node->orig); 
				}
				
				bp_node = bp_node->next;
			}
			
			node_remove = node_remove->next;
		}
	}
	
	/* add new breakpoints */
	node_add = to_add;
	while (node_add != NULL)
	{
		node_add->orig = ptrace(PT_READ_I, pid, node_add->addr, 0);
		node_add->oxcc = (node_add->orig & (unsigned long)0xFFFFFFFFFFFFFF00) | 0xCC;
		
		ptrace(PT_WRITE_I, pid, node_add->addr, node_add->oxcc);
		
		node_add = node_add->next;
	}
		
	/* carry on */
	ptrace(PT_CONTINUE, pid, (caddr_t)1, 0);
	wait(&wait_status);
	
	return after_wait(wait_status, &pid, bp);
}

int 
runtime_command_continue (struct command_args_t *args)
{
	struct breakpoint_t *node_remove;

	/* TODO breakpoint */
	if (resume(NULL, *(args->pid), NULL, NULL) == 0)
	{
		*(args->state) = DEFAULT;
	}
	else
		*(args->state) = RUNNING;

	/* remove breakpoints to be removed */
	/*
	node_remove = head_remove;
	while (node_remove != NULL)
	{
		remove_breakpoint(&head, node_remove->path, node_remove->line);
		node_remove = node_remove->next;
	}

	remove_all_breakpoints(&head_add);
	remove_all_breakpoints(&head_remove);
	*/
	return 0;
}


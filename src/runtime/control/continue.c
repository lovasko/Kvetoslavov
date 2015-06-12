#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>

#include <machine/reg.h>

#include "runtime/breakpoint/breakpoint.h"
#include "runtime/control/control.h"
#include "state/state.h"

static int 
resume(struct breakpoint** bp, pid_t pid, struct breakpoint* to_add, 
    struct breakpoint* to_remove)
{
	int wait_status;
	int is_breakpoint;
	struct reg regs;
	struct breakpoint* node;
	struct breakpoint* node_remove;
	struct breakpoint* bp_node;
	struct breakpoint* node_add;
	
	/* if no breakpoints present, 
	 *     just simply continue */
	if (bp == NULL) {
		ptrace(PT_CONTINUE, pid, (caddr_t)1, 0);
		
		wait(&wait_status);
		return after_wait(wait_status, &pid, bp);
	}
	
	/* handle breakpoints */
	ptrace(PT_GETREGS, pid, (caddr_t)&regs, 0);

	/* fix the breakpoint address with original instruction byte */
	node = *bp;
	
	is_breakpoint = 0; 
	
	while (node != NULL) {
		if (node->addr == regs.r_eip - 1) {
			ptrace(PT_WRITE_I, pid, (caddr_t)node->addr, node->orig);
			is_breakpoint = 1;
			break;
		}
		node = node->next;
	}
	
	/* do this only, if we were standing on a breakpoint */
	if (is_breakpoint == 1) {	
		/* move one instruction back */
		regs.r_eip -= 1;
		ptrace(PT_SETREGS, pid, (caddr_t)&regs, 0);
		
		/* perform the original instruction and react to possible changes*/
		ptrace(PT_STEP, pid, (caddr_t)1, 0);	
		wait(&wait_status);
		
		/* check whether the single step caused anything */
		if (WIFEXITED(wait_status)) {
			printf("Child exited on breakpoint in file %s at line %d.\n", node->path, 
			    node->line);
			return 0;
		}
		
		/* put the breakpoint instruction 0xCC back */
		ptrace(PT_WRITE_I, pid, (caddr_t)node->addr, node->oxcc);
		
		/* remove breakpoints ought to be removed */
		node_remove = to_remove;
		while (node_remove != NULL) {
			/* find the same breakpoint in bp, because it has the valid orig field */
			bp_node = *bp;
			while (bp_node != NULL) {
				if (bp_node->addr == node_remove->addr)
					ptrace(PT_WRITE_I, pid, (caddr_t)bp_node->addr, bp_node->orig); 
				
				bp_node = bp_node->next;
			}
			
			node_remove = node_remove->next;
		}
	}
	
	/* add new breakpoints */
	node_add = to_add;
	while (node_add != NULL) {
		node_add->orig = ptrace(PT_READ_I, pid, (caddr_t)node_add->addr, 0);
		node_add->oxcc = (node_add->orig & (unsigned long)0xFFFFFFFFFFFFFF00) | 
		    0xCC;
		
		ptrace(PT_WRITE_I, pid, (caddr_t)node_add->addr, node_add->oxcc);
		
		node_add = node_add->next;
	}
		
	/* carry on */
	ptrace(PT_CONTINUE, pid, (caddr_t)1, 0);
	wait(&wait_status);
	
	return after_wait(wait_status, &pid, bp);
}

int 
runtime_command_continue(struct command_args* cmd_args)
{
	struct breakpoint* node_remove;

	if (resume(cmd_args->bp_head, *(cmd_args->pid), *(cmd_args->bp_head_add), 
	           *(cmd_args->bp_head_remove)) == 0)
		*(cmd_args->state) = DEFAULT;
	else
		*(cmd_args->state) = RUNNING;

	/* remove breakpoints to be removed */
	node_remove = *(cmd_args->bp_head_remove);
	while (node_remove != NULL) {
		/* remove_breakpoint(cmd_args->head, node_remove); */
		node_remove = node_remove->next;
	}

	/* remove_all_breakpoints(cmd_args->head_add); */
	/* remove_all_breakpoints(cmd_args->head_remove); */

	return 0;
}


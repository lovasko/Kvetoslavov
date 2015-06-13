#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>

#include <machine/reg.h>

#include "runtime/breakpoint/breakpoint.h"
#include "runtime/control/control.h"
#include "runtime/state.h"

static int 
resume(struct context* ctx)
{
	int wait_status;
	/* int is_breakpoint; */
	/* struct reg regs; */
	uint64_t bp_count;
	
	/* if no breakpoints present, just simply continue */
	m_list_size(&ctx->breakpoints, &bp_count);
	if (bp_count == 0) {
		ptrace(PT_CONTINUE, ctx->pid, (caddr_t)1, 0);
		wait(&wait_status);
		return after_wait(wait_status, ctx);
	}
	
	/* handle breakpoints */
	/* ptrace(PT_GETREGS, ctx->pid, (caddr_t)&regs, 0); */

	/* fix the breakpoint address with original instruction byte */
	/* node = *bp; */
	
	/* is_breakpoint = 0; */ 
	
	/* while (node != NULL) { */
	/* 	if (node->addr == regs.r_eip - 1) { */
	/* 		ptrace(PT_WRITE_I, pid, (caddr_t)node->addr, node->orig); */
	/* 		is_breakpoint = 1; */
	/* 		break; */
	/* 	} */
	/* 	node = node->next; */
	/* } */
	
	/* do this only, if we were standing on a breakpoint */
	/* if (is_breakpoint == 1) { */	
		/* move one instruction back */
		/* regs.r_eip -= 1; */
		/* ptrace(PT_SETREGS, pid, (caddr_t)&regs, 0); */
		
		/* perform the original instruction and react to possible changes*/
		/* ptrace(PT_STEP, pid, (caddr_t)1, 0); */	
		/* wait(&wait_status); */
		
		/* check whether the single step caused anything */
		/* if (WIFEXITED(wait_status)) { */
		/* 	printf("Child exited on breakpoint in file %s at line %d.\n", node->path, */ 
		/* 	    node->line); */
		/* 	return 0; */
		/* } */
		
		/* put the breakpoint instruction 0xCC back */
		/* ptrace(PT_WRITE_I, pid, (caddr_t)node->addr, node->oxcc); */
		
		/* remove breakpoints ought to be removed */
		/* node_remove = to_remove; */
		/* while (node_remove != NULL) { */
		/* 	/1* find the same breakpoint in bp, because it has the valid orig field *1/ */
		/* 	bp_node = *bp; */
		/* 	while (bp_node != NULL) { */
		/* 		if (bp_node->addr == node_remove->addr) */
		/* 			ptrace(PT_WRITE_I, pid, (caddr_t)bp_node->addr, bp_node->orig); */ 
				
		/* 		bp_node = bp_node->next; */
		/* 	} */
			
		/* 	node_remove = node_remove->next; */
		/* } */
	/* } */
	
	/* add new breakpoints */
	/* node_add = to_add; */
	/* while (node_add != NULL) { */
	/* 	node_add->orig = ptrace(PT_READ_I, pid, (caddr_t)node_add->addr, 0); */
	/* 	node_add->oxcc = (node_add->orig & (unsigned long)0xFFFFFFFFFFFFFF00) | */ 
	/* 	    0xCC; */
		
	/* 	ptrace(PT_WRITE_I, pid, (caddr_t)node_add->addr, node_add->oxcc); */
		
	/* 	node_add = node_add->next; */
	/* } */
		
	/* carry on */
	/* ptrace(PT_CONTINUE, ctx->pid, (caddr_t)1, 0); */
	/* wait(&wait_status); */
	
	/* return after_wait(wait_status, ctx); */
	return 0;
}

int 
runtime_command_continue(struct context* ctx)
{
	if (resume(ctx) == 0)
		ctx->state = STATE_DEFAULT;
	else
		ctx->state = STATE_RUNNING;

	/* TODO investigate breakpoint removal */
	m_list_clear(&ctx->breakpoints_add);
	m_list_clear(&ctx->breakpoints_remove);

	return 0;
}


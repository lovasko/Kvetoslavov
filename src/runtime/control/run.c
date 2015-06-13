#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

#include <libgen.h>

#include "runtime/breakpoint/breakpoint.h"
#include "runtime/control/control.h"
#include "runtime/state.h"

static void
insert_breakpoint(void* _bp, void* _ctx)
{
	struct breakpoint* bp;
	struct context* ctx;

	bp = _bp;
	ctx = _ctx;

	bp->orig = ptrace(PT_READ_I, ctx->pid, (caddr_t)bp->addr, 0);
	bp->oxcc = (bp->orig & (unsigned long)0xFFFFFFFFFFFFFF00) | 0xCC;
	ptrace(PT_WRITE_I, ctx->pid, (caddr_t)bp->addr, bp->oxcc);
}

static int 
start(struct context* ctx)
{
	char* args[2];
	int wait_status;
	
	ctx->pid = fork();
	if (ctx->pid == 0) {
		/* if we are in the child, allow the tracing */
		ptrace(PT_TRACE_ME, 0, 0, 0);
				
		args[0] = basename(ctx->exec_path);
		args[1] = NULL;
		
		 /* rewrite the process with desired executable */
		execv(ctx->exec_path, args);
	} else {
		/* stop after first instruction */
		wait(&wait_status);

		/* insert breakpoints and save the original byte */
		m_list_map(&ctx->breakpoints, insert_breakpoint, ctx);

		/* carry on */
		ptrace(PT_CONTINUE, ctx->pid, (caddr_t)1, 0);
		wait(&wait_status);
		return after_wait(wait_status, ctx);
	}

	return 0;
}

int 
runtime_command_run(struct context* ctx)
{
	if (start(ctx) == 0) {
		ctx->state = STATE_DEFAULT;
		ctx->pid = -1;
		ctx->exec_path = NULL;
	} else {
		ctx->state = STATE_RUNNING;
	}

	return 0;
}


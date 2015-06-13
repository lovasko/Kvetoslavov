#include <sys/types.h>
#include <sys/ptrace.h>

#include <stdio.h>

#include "runtime/breakpoint/breakpoint.h"
#include "runtime/control/control.h"
#include "runtime/state.h"

static void
remove_breakpoint(void* _bp, void* _ctx)
{
	struct breakpoint* bp;
	struct context* ctx;

	bp = _bp;
	ctx = _ctx;
	
	ptrace(PT_WRITE_I, ctx->pid, (caddr_t)bp->addr, bp->orig);
}

static int 
detach(struct context* ctx)
{
	m_list_map(&ctx->breakpoints, remove_breakpoint, ctx);
	
	if (ptrace(PT_DETACH, ctx->pid, 0, 0) == 0)
		return 0;
	else
		return 1;
}

int 
runtime_command_detach(struct context* ctx)
{
	if (detach(ctx) == 0)
		fprintf(stdout, "Detach was successful.\n");
	else
		fprintf(stderr, "Detach was unsuccessful.\n");

	m_list_clear(&ctx->breakpoints);
	m_list_clear(&ctx->breakpoints_add);
	m_list_clear(&ctx->breakpoints_remove);

	ctx->state = STATE_DEFAULT;
	ctx->pid = -1;
	ctx->exec_path = NULL;

	return 0;
}


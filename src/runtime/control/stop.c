#include <sys/types.h>
#include <sys/ptrace.h>

#include <stdio.h>

#include "runtime/control/control.h"
#include "runtime/state.h"

static int 
stop(pid_t pid)
{
	if (ptrace(PT_KILL, pid, 0, 0) == 0)
		return 0;
	else
		return 1;
}

int 
runtime_command_stop(struct context* ctx)
{
	if (runtime_process_exists(ctx->pid) == 0) {
		if (stop(ctx->pid) == 0) {
			ctx->state = STATE_DEFAULT;
			ctx->pid = -1;
			ctx->exec_path = NULL;
			m_list_clear(&ctx->breakpoints);
			m_list_clear(&ctx->breakpoints_add);
			m_list_clear(&ctx->breakpoints_remove);

			fprintf(stdout, "Debuggee terminated.\n");
		} else {
			fprintf(stderr, "Unable to terminate the debuggee.\n");
		}
	} else {
		fprintf(stderr, "Process does not exist.\n");	
	}

	return 0;
}


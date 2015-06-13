#include <stdio.h>

#include "runtime/general/general.h"

int 
runtime_command_status(struct context* ctx)
{
	fprintf(stdout, "            PID: %d\n", ctx->pid);
	fprintf(stdout, "Executable path: %s\n", ctx->exec_path);

	return 0;
}


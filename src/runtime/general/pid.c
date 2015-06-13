#include <stdio.h>

#include "runtime/general/general.h"

int 
runtime_command_pid(struct context* ctx)
{
	printf("%d\n", ctx->pid);
	return 0;
}


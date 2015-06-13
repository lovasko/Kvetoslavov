#ifndef KVETOSLAVOV_RUNTIME_COMMAND_H
#define KVETOSLAVOV_RUNTIME_COMMAND_H

#include "runtime/context.h"

struct command
{
	const char* name;
	int (*fn)(struct context* ctx);
	int state_flags;
	int arg_count;
};

#endif


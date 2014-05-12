#ifndef KVETOSLAVOV_RUNTIME_COMMANDS
#define KVETISLAVOV_RUNTIME_COMMANDS

#include "state.h"
#include "command.h"
#include "runtime/general/help.h"

struct command_t registred_commands[] =
{
	{"?",    runtime_command_help, ALL_STATES, 1},
	{"help", runtime_command_help, ALL_STATES, 1},
	NULL
};

#endif


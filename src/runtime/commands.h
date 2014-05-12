#ifndef KVETOSLAVOV_RUNTIME_COMMANDS
#define KVETISLAVOV_RUNTIME_COMMANDS

#include "state.h"
#include "command.h"
#include "runtime/general/help.h"
#include "runtime/general/exit.h"

struct command_t registred_commands[] =
{
	{"?",    runtime_command_help, ALL_STATES, 1},
	{"help", runtime_command_help, ALL_STATES, 1},
	{"exit", runtime_command_exit, ALL_STATES, 1},
	{"quit", runtime_command_exit, ALL_STATES, 1},
	NULL
};

#endif


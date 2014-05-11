#ifndef COMMANDS_H
#define COMMANDS_H

#include "state.h"
#include "command.h"
#include "general/help.h"

struct command_t registred_commands[] =
{
	{"?",    runtime_command_help, ALL_STATES, 1},
	{"help", runtime_command_help, ALL_STATES, 1},
	NULL
};

#endif


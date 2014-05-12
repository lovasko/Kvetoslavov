#ifndef KVETOSLAVOV_RUNTIME_COMMANDS
#define KVETISLAVOV_RUNTIME_COMMANDS

#include "state.h"
#include "command.h"
#include "runtime/general/help.h"
#include "runtime/general/exit.h"
#include "runtime/general/select.h"
#include "runtime/general/status.h"

struct command_t registred_commands[] =
{
	{"?",    runtime_command_help, ALL_STATES, 1},
	{"help", runtime_command_help, ALL_STATES, 1},
	{"exit", runtime_command_exit, ALL_STATES, 1},
	{"quit", runtime_command_exit, ALL_STATES, 1},
	{"select", runtime_command_select, DEFAULT, 2},
	{"status", runtime_command_status, ALL_STATES, 1},
	{"info", runtime_command_status, ALL_STATES, 1},
	NULL
};

#endif


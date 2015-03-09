#ifndef KVETOSLAVOV_RUNTIME_COMMANDS_H
#define KVETISLAVOV_RUNTIME_COMMANDS_H

#include "command/command.h"
#include "runtime/breakpoint/master.h"
#include "runtime/control/attach.h"
#include "runtime/control/continue.h"
#include "runtime/control/detach.h"
#include "runtime/control/run.h"
#include "runtime/control/stop.h"
#include "runtime/general/exit.h"
#include "runtime/general/help.h"
#include "runtime/general/select.h"
#include "runtime/general/status.h"
#include "state/state.h"

struct command registred_commands[] =
{
	{"?",          runtime_command_help,              ALL_STATES, -1},
	{"attach",     runtime_command_attach,            DEFAULT,    1},
	{"continue",   runtime_command_continue,          RUNNING,    0},
	{"detach",     runtime_command_detach,            RUNNING,    0},
	{"exit",       runtime_command_exit,              ALL_STATES, 0},
	{"help",       runtime_command_help,              ALL_STATES, -1},
	{"info",       runtime_command_status,            RUNNING,    0},
	{"quit",       runtime_command_exit,              ALL_STATES, 0},
	{"run",        runtime_command_run,               PREPARING,  0},
	{"select",     runtime_command_select,            DEFAULT,    1},
	{"status",     runtime_command_status,            RUNNING,    0},
	{"stop",       runtime_command_stop,              RUNNING,    0},
/*
	{"bp",         runtime_command_breakpoint_master, 0,         -1},
	{"breakpoint", runtime_command_breakpoint_master, 0,         -1},
*/
};

#endif


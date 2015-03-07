#include <stdio.h>

#include "command/command.h"
#include "state/state.h"
#include "util/line.h"

void
command_print_compatible_states(struct command* cmd)
{
	if (state_is_compatible(DEFAULT, command))   
		fprintf(stderr, "\tdef\n");

	if (state_is_compatible(PREPARING, command))   
		fprintf(stderr, "\tprp\n");

	if (state_is_compatible(RUNNING, command))   
		fprintf(stderr, "\trun\n");
}

struct command*
command_match(struct command* cmds, unsigned int n_cmds, char* line)
{
	unsigned int i;

	for (i = 0; commands[i].name != NULL && i < n_cmds; i++)
		if (line_starts_with(line, commands[i].name) == 0)
			return &commands[i];

	return NULL;
}


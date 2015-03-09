#include <stdio.h>

#include "command/command.h"
#include "state/state.h"
#include "util/line.h"

void
command_print_compatible_states(struct command* cmd)
{
	if (state_is_compatible(DEFAULT, cmd))
		fprintf(stderr, "\tdef\n");

	if (state_is_compatible(PREPARING, cmd))
		fprintf(stderr, "\tprp\n");

	if (state_is_compatible(RUNNING, cmd))
		fprintf(stderr, "\trun\n");
}

struct command*
command_match(struct command* cmds, unsigned int n_cmds, char* line)
{
	unsigned int i;

	for (i = 0; cmds[i].name != NULL && i < n_cmds; i++)
		if (line_starts_with(line, cmds[i].name) == 0)
			return &cmds[i];

	return NULL;
}


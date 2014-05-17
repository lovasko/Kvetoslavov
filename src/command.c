#include "command.h"
#include "state.h"
#include "line.h"

void
command_print_compatible_states (struct command_t *command)
{
	if (state_is_compatible(DEFAULT, command))   
		fprintf(stderr, "\tdef\n");

	if (state_is_compatible(PREPARING, command))   
		fprintf(stderr, "\tprp\n");

	if (state_is_compatible(RUNNING, command))   
		fprintf(stderr, "\trun\n");
}

struct command_t*
command_match (struct command_t *commands, char *line)
{
	unsigned int i;

	for (i = 0; commands[i].name != NULL; i++)
		if (line_starts_with(line, commands[i].name) == 0)
			return &commands[i];

	return NULL;
}


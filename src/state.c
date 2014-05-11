#include "state.h"

#include <stdio.h>

void 
state_print (int state)
{
	switch (state)
	{
		case DEFAULT:
			printf("def");
		break;

		case PREPARING:
			printf("prp");
		break;

		case RUNNING:
			printf("run");
		break;

		default:
			printf("ERROR: unknown state");
		break;
	}

	printf("> ");
	fflush(stdout);
}

int 
state_is_compatible (int check, struct command_t* command)
{
	return (check & command->compatible_state);
}


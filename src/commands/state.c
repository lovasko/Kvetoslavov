#include "state.h"

#include <stdio.h>

void 
state_print (enum state_t state)
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
state_is_compatible (enum state_t base, int checked)
{
	return (base & checked);
}


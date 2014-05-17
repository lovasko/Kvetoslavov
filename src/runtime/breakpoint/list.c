#include "runtime/breakpoint/list.h"
#include "runtime/breakpoint/breakpoint.h"

#include <stdio.h>

int
runtime_command_breakpoint_list (struct command_args_t *args)
{
	struct breakpoint_t *node;

	if (*(args->head) == NULL)
		printf("No breakpoints yet.\n");
	
	node = *(args->head); 
	while (node != NULL)
	{
		printf("Breakpoint in file %s at line %d (0x%X)\n", node->path, node->line, 
		    (unsigned long)node->addr);
		node = node->next;
	}

	return 0;
}


#include <stdio.h>

#include "runtime/breakpoint/breakpoint.h"
#include "runtime/breakpoint/list.h"

int
runtime_command_breakpoint_list(struct command_args* cmd_args)
{
	struct breakpoint* node;

	if (*(cmd_args->head) == NULL)
		printf("No breakpoints yet.\n");
	
	node = *(cmd_args->head); 
	while (node != NULL) {
		printf("Breakpoint in file %s at line %d (0x%X)\n", node->path, node->line, 
		    node->addr);
		node = node->next;
	}

	return 0;
}


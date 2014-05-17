#include "runtime/breakpoint/remove.h"
#include "runtime/breakpoint/breakpoint.h"
#include "runtime/breakpoint/common.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int
runtime_command_breakpoint_remove (struct command_args_t *args)
{
	int line_number;
	int remove_retval;
	struct breakpoint_t scheduled_removal;

	if (*(args->head) == NULL && *(args->head_add) == NULL)
	{
		fprintf(stderr, "There are no breakpoints!\n");
		return 0;
	}
	
	errno = 0;
	line_number = strtol(args->text_args[2], NULL, 10);
	if (errno != 0)
	{
		fprintf(stderr, "Wrong line number format.\n");	
		return 0;
	}

	if (compilation_unit_exists(*(args->exec_path), args->text_args[1]) != 0)
	{
		fprintf(stderr, "No such compilation unit.\n");
		return 0;
	}

	scheduled_removal.path = strdup(args->text_args[1]);
	scheduled_removal.line = line_number;
	scheduled_removal.next = NULL;
	
	remove_retval = remove_breakpoint(args->head_add, &scheduled_removal);	
	if (remove_retval == 0)
	{
		fprintf(stdout, 
		    "Successfully removed a breakpoint scheduled for addition.\n");
	}
	if (remove_retval == 1)
	{
		fprintf(stderr, "No such breakpoint found.\n");
		return 0;
	}

	add_breakpoint(*(args->head_remove), &scheduled_removal);

	return 0;
}


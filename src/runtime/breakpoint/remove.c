#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "runtime/breakpoint/breakpoint.h"
#include "runtime/breakpoint/common.h"
#include "runtime/breakpoint/remove.h"

int
runtime_command_breakpoint_remove(struct command_args* cmd_args)
{
	int line_number;
	int remove_retval;
	struct breakpoint scheduled_removal;

	if (*(cmd_args->head) == NULL && *(cmd_args->head_add) == NULL) {
		fprintf(stderr, "There are no breakpoints!\n");
		return 0;
	}
	
	errno = 0;
	line_number = strtol(cmd_args->text_args[2], NULL, 10);
	if (errno != 0) {
		fprintf(stderr, "Wrong line number format.\n");	
		return 0;
	}

	if (compilation_unit_exists(*(cmd_args->exec_path), cmd_args->text_args[1]) 
	    != 0) {
		fprintf(stderr, "No such compilation unit.\n");
		return 0;
	}

	scheduled_removal.path = strdup(cmd_args->text_args[1]);
	scheduled_removal.line = line_number;
	scheduled_removal.next = NULL;
	
	remove_retval = remove_breakpoint(cmd_args->head_add, &scheduled_removal);	
	if (remove_retval == 0)
		fprintf(stdout, 
		    "Successfully removed a breakpoint already scheduled for addition.\n");

	if (remove_retval == 1) {
		fprintf(stderr, "No such breakpoint found.\n");
		return 0;
	}

	add_breakpoint(*(cmd_args->head_remove), &scheduled_removal);

	return 0;
}


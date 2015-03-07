#include "runtime/breakpoint/master.h"
#include "runtime/breakpoint/list.h"
#include "runtime/breakpoint/insert.h"
#include "runtime/breakpoint/remove.h"
#include "command.h"
#include "state.h"

#include <string.h>

static unsigned int
get_arg_count(char** args)
{
	unsigned int i;
	
	for (i = 0; args[i]; i++);

	return i;
}

int
runtime_command_breakpoint_master(struct command_args* cmd_args)
{
	struct command_args new_args;
	int arg_count;

	new_args = *cmd_args;
	new_args.text_args = &(cmd_args->text_args[1]);

	arg_count = get_arg_count(&cmd_args->text_args[1]);

	if (arg_count == 0) {
		fprintf(stderr, "Subcommand not present. Try 'help'.\n");	
		return 0;
	}

	if (strcmp(cmd_args->text_args[1], "list") == 0) {
		/* omitting the state check, since this command can run in any state */
		if (arg_count == 1)
			return runtime_command_breakpoint_list(&new_args);
		else
			fprintf(stderr, "Wrong argument count.");	
	} else if (strcmp(cmd_args->text_args[1], "insert") == 0) {
		if (*(cmd_args->state) == PREPARING ||
		    *(cmd_args->state) == RUNNING) {
			if (arg_count == 3)
				return runtime_command_breakpoint_insert(&new_args);	
			else
				fprintf(stderr, "Wrong argument count.");	
		} else {
			fprintf(stderr, "%s\n%s\n",
					"You are not in a compatible state for this operation. ",
					"Compatible states are:\n\tprp\n\trun");
		}
	} else if (strcmp(cmd_args->text_args[1], "remove") == 0) {
		if (arg_count == 3)
			return runtime_command_breakpoint_remove(&new_args);
		else
			fprintf(stderr, "Wrong argument count.");	
	} else {
		fprintf(stderr, "No such breakpoint subcommand.\n");
	}

	return 0;
}


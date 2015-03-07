#include <stdlib.h>
#include <stdio.h>

#include "command/command.h"
#include "runtime/commands.h"
#include "state/state.h"
#include "util/line.h"
#include "util/util.h"

/**
 * FreeBSD x86 ELF Debugger.
 * @param argc not used
 * @param argv not used
 * @return 0 on success, 1 otherwise
 */
int 
main(int argc, char* argv[])
{
	char* exec_path;
	char* line;
	char* line_free;
	char** arguments;
	int arg_count;
	int return_value;
	int state;
	pid_t pid;
	struct breakpoint* bp_head;
	struct command* cmd;
	struct command_args cmd_args;

	(void) argc;
	(void) argv;

	ignore_sigint();
	fprintf(stdout, "Kvetoslavov Debugger\n");

	bp_head = NULL;
	exec_path = NULL;
	pid = -1;
	return_value = 0;
	state = DEFAULT;

	do {
		state_print(state);
		line_free = line = line_read_stdin(1024);
		if (line[0] == '\0') {
			free(line);
			continue;
		}

		cmd = command_match(registred_commands, line);
		if (cmd != NULL) {
			if (state_is_compatible(state, cmd)) {
				arg_count = line_argument_count(line, ' ');
				if (cmd->expected_arg_count == arg_count ||
				    cmd->expected_arg_count == -1) {
					**arguments = line_get_arguments(&line, ' ');

					cmd_args.state = &state;
					cmd_args.text_args = arguments;
					cmd_args.exec_path = &exec_path;
					cmd_args.pid = &pid;
					cmd_args.head = &bp_head;
					return_value = cmd->function(&cmd_args);
				} else {
					fprintf(stderr, "Wrong number of arguments. %d supplied, %d "
					    "expected.\n", arg_count, cmd->expected_arg_count);
				}
			} else {
				fprintf(stderr, "%s\n%s\n",
				    "You are not in a compatible state for this operation. ",
						"Compatible states are:");

				command_print_compatible_states(cmd);
			}
		} else {
			fprintf(stderr, "Unknown command. In case of confusion, try 'help'.\n");
		}

		free(line_free);
	} while (return_value != -1);

	return EXIT_SUCCESS;
}


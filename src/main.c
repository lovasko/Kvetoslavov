#include "command.h"
#include "state.h"
#include "line.h"
#include "util.h"
#include "runtime/commands.h"

/**
 * FreeBSD x86 ELF Debugger.
 * @param argc not used
 * @param argv not used
 * @return 0 on success, 1 otherwise
 */
int 
main (int argc, char **argv)
{
	int state;
	char *line, *line_free;
	int return_value;
	struct command_t *command;
	struct command_args_t args;
	char *exec_path = NULL;
	pid_t pid = -1;
	struct breakpoint_t *head = NULL;

	(void) argc;
	(void) argv;

	ignore_sigint();
	fprintf(stdout, "Kvetoslavov Debugger\n");

	state = DEFAULT;
	return_value = 0;

	do
	{
		state_print(state);
		line_free = line = line_read_stdin(1024);
		if (line[0] == '\0')
		{
			free(line);
			continue;
		}

		command = command_match(registred_commands, line);
		if (command != NULL)
		{
			if (state_is_compatible(state, command))
			{
				int arg_count = line_argument_count(line, ' ');
				if (command->expected_arg_count == arg_count ||
				    command->expected_arg_count == -1)
				{
					char **arguments = line_get_arguments(&line, ' ');

					args.state = &state;
					args.text_args = arguments;
					args.exec_path = &exec_path;
					args.pid = &pid;
					args.head = &head;
					return_value = command->function(&args);
				}
				else
				{
					fprintf(stderr, "Wrong number of arguments. %d supplied, %d "
					    "expected.\n", arg_count, command->expected_arg_count);
				}
			}
			else
			{
				fprintf(stderr, "%s\n%s\n",
				    "You are not in a compatible state for this operation. ",
						"Compatible states are:");

				command_print_compatible_states(command);
			}
		}
		else
		{
			fprintf(stderr, "Unknown command. In case of confusion, try 'help'.\n");
		}

		free(line_free);
	}
	while (return_value != -1);

	return 0;
}


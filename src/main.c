#include <stdlib.h>
#include <stdio.h>

#include "runtime/general/general.h"
#include "runtime/control/control.h"
#include "runtime/command.h"
#include "runtime/state.h"
#include "util/line.h"
#include "util/util.h"

/**
 * Inform user about the debugging phase we are in.
 * @param state the state to be printed
 */
static void 
state_print(int state)
{
	switch (state) {
		case STATE_DEFAULT:
			printf("def");
		break;

		case STATE_PREPARING:
			printf("prp");
		break;

		case STATE_RUNNING:
			printf("run");
		break;

		default:
			printf("ERROR: unknown state");
		break;
	}

	printf("> ");
	fflush(stdout);
}

/**
 * Check if the state is compatible.
 * @param check 
 * @param command 
 * @return 1 if command is compatible with check, 0 otherwise
 */
static int 
state_is_compatible(int state, struct command* cmd)
{
	if (cmd->state_flags == 0)
		return 1;

	return (state & cmd->state_flags);
}

/**
 * Print all compatible state names to stdout.
 * @param command command
 */
void
command_print_compatible_states(struct command* cmd)
{
	if (state_is_compatible(STATE_DEFAULT, cmd))
		fprintf(stderr, "\tdef\n");

	if (state_is_compatible(STATE_PREPARING, cmd))
		fprintf(stderr, "\tprp\n");

	if (state_is_compatible(STATE_RUNNING, cmd))
		fprintf(stderr, "\trun\n");
}

/**
 * Find command by matching its name with the string.
 * @param commands pointer to array of commands
 * @param line string
 * @return pointer to one of the array elements or NULL on failed match
 */
struct command*
command_match(struct m_list* commands, char* line)
{
	struct m_elem* runner;
	struct command* cmd;

	m_list_first(commands, &runner);
	while (runner != NULL) {
		m_elem_data(runner, (void**)&cmd);
		if (line_starts_with(line, cmd->name) == 0)
			return cmd;
		m_elem_next(runner, &runner);
	}

	return NULL;
}

static void
add_default_commands(struct m_list* commands)
{
	unsigned int i;
	struct command default_commands[] =
	{
		{"?",          runtime_command_help,              STATE_ALL,      -1},
		{"attach",     runtime_command_attach,            STATE_DEFAULT,   1},
		{"continue",   runtime_command_continue,          STATE_RUNNING,   0},
		{"detach",     runtime_command_detach,            STATE_RUNNING,   0},
		{"exit",       runtime_command_exit,              STATE_ALL,       0},
		{"help",       runtime_command_help,              STATE_ALL,      -1},
		{"info",       runtime_command_status,            STATE_RUNNING,   0},
		{"quit",       runtime_command_exit,              STATE_ALL,       0},
		{"pid",        runtime_command_pid,               STATE_ALL,       0},
		{"run",        runtime_command_run,               STATE_PREPARING, 0},
		{"select",     runtime_command_select,            STATE_DEFAULT,   1},
		{"status",     runtime_command_status,            STATE_RUNNING,   0},
		{"stop",       runtime_command_stop,              STATE_RUNNING,   0},
	};

	for (i = 0; i < sizeof(default_commands)/sizeof(default_commands[0]); i++)
		m_list_append(commands,
		              M_LIST_COPY_DEEP,
		              &default_commands[i],
		              sizeof(struct command));
}

/**
 * FreeBSD x86 ELF Debugger.
 * @param argc not used
 * @param argv not used
 * @return 0 on success, 1 otherwise
 */
int 
main(int argc, char* argv[])
{
	char* line;
	char* line_free;
	int arg_count;
	int return_value;
	struct command* cmd;
	struct context ctx;

	(void) argc;
	(void) argv;

	ignore_sigint();
	printf("kvet debugger\n");

	ctx.exec_path = NULL;
	ctx.pid = -1;
	ctx.state = STATE_DEFAULT;
	m_list_init(&ctx.breakpoints);
	m_list_init(&ctx.breakpoints_add);
	m_list_init(&ctx.breakpoints_remove);

	m_list_init(&ctx.commands);
	add_default_commands(&ctx.commands);

	return_value = 0;
	do {
		state_print(ctx.state);
		line_free = line = line_read_stdin(1024);
		if (line[0] == '\0') {
			free(line);
			continue;
		}

		cmd = command_match(&ctx.commands, line);
		if (cmd != NULL) {
			if (state_is_compatible(ctx.state, cmd)) {
				arg_count = line_argument_count(line, ' ');
				if (cmd->arg_count == arg_count || cmd->arg_count == -1) {
					ctx.text_args = line_get_arguments(&line, ' ');
					return_value = cmd->fn(&ctx);
				} else {
					fprintf(stderr, "Wrong number of arguments. %d supplied, %d "
					    "expected.\n", arg_count, cmd->arg_count);
				}
			} else {
				fprintf(stderr, "%s\n%s\n",
				    "You are not in a compatible state for this operation. ",
						"Compatible states are:");

				command_print_compatible_states(cmd);
			}
		} else {
			fprintf(stderr, "Unknown command. Use the 'help' command.\n");
		}

		free(line_free);
	} while (return_value != -1);

	return EXIT_SUCCESS;
}


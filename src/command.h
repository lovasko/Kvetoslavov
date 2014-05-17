#ifndef KVETOSLAVOV_COMMAND 
#define KVETOSLAVOV_COMMAND

#include "runtime/breakpoint/breakpoint.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct command_args_t
{
	int *state;
	char **text_args;
	char **exec_path;
	pid_t *pid;
	struct breakpoint_t **head;
	struct breakpoint_t **head_add;
	struct breakpoint_t **head_remove;
};

struct command_t
{
	const char *name;
	int (*function)(struct command_args_t *args);
	int compatible_state;
	unsigned short expected_arg_count;
};

/**
 * Print all compatible state names to stdout.
 * @param command command
 */
void
command_print_compatible_states (struct command_t *command);

/**
 * Find command by matching its name with the string.
 * @param commands pointer to array of commands
 * @param line string
 * @return pointer to one of the array elements or NULL on failed match
 */
struct command_t*
command_match (struct command_t* commands, char *line);

#endif


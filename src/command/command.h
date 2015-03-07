#ifndef KVETOSLAVOV_COMMAND_COMMAND_H
#define KVETOSLAVOV_COMMAND_COMMAND_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "runtime/breakpoint/breakpoint.h"

struct command_args
{
	int* state;
	char** text_args;
	char** exec_path;
	pid_t* pid;
	struct breakpoint** bp_head;
	struct breakpoint** bp_head_add;
	struct breakpoint** bp_head_remove;
};

struct command
{
	const char* name;
	int (*function)(struct command_args* args);
	int compatible_state;
	int expected_arg_count;
};

/**
 * Print all compatible state names to stdout.
 * @param command command
 */
void
command_print_compatible_states(struct command* cmd);

/**
 * Find command by matching its name with the string.
 * @param commands pointer to array of commands
 * @param line string
 * @return pointer to one of the array elements or NULL on failed match
 */
struct command*
command_match(struct command* cmds, unsigned int n_cmds, char* line);

#endif


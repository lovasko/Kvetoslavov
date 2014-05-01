#include "breakpoint.h"
#include "util.h"
#include "control.h"
#include "units.h"
#include "subprogram.h"
#include "commands/command.h"
#include "commands/state.h"
#include "platform.h"

#define COMMAND_MAX_ARG 3

/**
 * Set SIGINT handler to ignore for this process.
 */
void ignore_sigint()
{
	struct sigaction new_sa;
	struct sigaction old_sa;

	sigfillset(&new_sa.sa_mask);
	new_sa.sa_handler = SIG_IGN;
	new_sa.sa_flags = 0;

	if (sigaction(SIGINT, &new_sa, &old_sa) == 0 && old_sa.sa_handler != SIG_IGN)
	{
		sigaction(SIGINT, &new_sa, 0);
	}
}

/**
 * Print help message.
 * @param argv unused
 * @return 0
 */
int dbg_help(char **argv)
{
	printf("List of possible commands in each phase:\n"
			"\tDEFAULT\n"
				"\tattach <PID>\n"
				"\tselect <EXECUTABLE PATH>\n"
				"\thelp/?\n"
				"\texit\n\n"
			
			"\tPREPARING\n"
				"\tlist_files\n"
				"\tlist_functions\n"
				"\tlist_breaks\n"
				"\tadd_break <FILE PATH> <LINE NUMBER>\n"
				"\tremove_break <FILE PATH> <LINE NUMBER>\n"
				"\trun\n\n"
			
			"\tRUNNING\n"
				"\tcontinue\n"
				"\tstop\n"
				"\tdump <ADDRESS>\n"
				"\tpid \n"
		  );

	return 0;
}

/**
 * Try to terminate the debugee and break from main loop.
 * @param argv unused
 * @return -1
 */
int dbg_exit(char **argv)
{
	char dirname[512];
	
	memset(dirname, '\0', 512);
	snprintf(dirname, 512, "/proc/%d", pid);

	if (directory_exists(dirname) == 0)
	{
		if (stop(pid) == 0)
		{
			printf("Child terminated.\n");

			remove_all_breakpoints(&head);
			remove_all_breakpoints(&head_add);
			remove_all_breakpoints(&head_remove);
		}
		else
		{
			printf("Unable to terminate the child. Do it yourself, the PID is %d.\n",
			        pid);
		}
	}
	
	return -1;
}

/**
 * Try to stop the debuggee.
 * @param argv unused
 * @return 0
 */
int dbg_stop(char **argv)
{
	char dirname[512];
	
	memset(dirname, '\0', 512);
	snprintf(dirname, 512, "/proc/%d", pid);

	if (directory_exists(dirname) == 0)
	{
		if (stop(pid) == 0)
		{
			state = DEFAULT;

			head = NULL;
			head_add = NULL;
			head_remove = NULL;
			pid = -1;

			printf("Child terminated.\n");
		}
		else
		{
			printf("Unable to terminate the child.\n");
		}
	}

	return 0;
}

/**
 * Start the debuggee.
 * @param argv unused
 * @return 0
 */
int dbg_run(char **argv)
{
	if (start(&head, exec_path, &pid) == 0)
	{
		state = DEFAULT;
	}
	else
	{
		state = RUNNING;
	}

	remove_all_breakpoints(&head_add);

	return 0;
}

/**
 * Resume paused debuggee.
 * @param argv unused
 * @return 0
 */
int dbg_continue(char **argv)
{
	struct breakpoint *node_remove;

	if (resume(&head, &pid, head_add, head_remove) == 0)
	{
		state = DEFAULT;
	}
	else
	{
		state = RUNNING;
	}

	/* remove breakpoints to be removed */
	node_remove = head_remove;
	while (node_remove != NULL)
	{
		remove_breakpoint(&head, node_remove->path, node_remove->line);
		node_remove = node_remove->next;
	}

	remove_all_breakpoints(&head_add);
	remove_all_breakpoints(&head_remove);

	return 0;
}

/**
 * Basic checks on the executable.
 * @param argv second element is used as a path to the executable
 * @return 0
 */
int dbg_select(char **argv)
{
	memset(exec_path, '\0', 512);
	snprintf(exec_path, 512, "%s", argv[1]);

	if (file_exists(exec_path) == 0)
	{
		if (is_elf(exec_path) == 0)
		{
			state = PREPARING;
		}
		else
		{
			fprintf(stderr, "Selected file is not an ELF binary.\n");
		}
	}
	else
	{
		fprintf(stderr, "No such file.\n");
	}

	return 0;
}

/**
 * List compilation units.
 * @param argv unused
 * @return 0
 */
int dbg_list_files(char **argv)
{
	list_units(exec_path);
	return 0;
}

/**
 * List already inserted breakpoints.
 * @param argv unused
 * @return 0
 */
int dbg_list_breaks(char **argv)
{
	print_breakpoints(head);
	return 0;
}

/**
 * List subprograms used in the executable.
 * @param argv unused
 */
int dbg_list_functions(char **argv)
{
	list_subprograms(exec_path, NULL);
	return 0;
}

/**
 * Print the PID of debuggee.
 * @param argv unused
 * @return 0
 */
int dbg_pid(char **argv)
{
	printf("%d\n", pid);
	return 0;
}

/**
 * Print data inside the process at specified address.
 * @param argv  second element is used as the address (hexadecimal)
 * @return 0
 */
int dbg_dump(char **argv)
{
	int address; 
	
	address = strtol(argv[1], NULL, 16);
	print_address(pid, address);

	return 0;
}

/**
 * Add breakpoint to the debuggee.
 * @param argv second element is used as a path to the compilation unit
               third element is used as a line number
 * @return 0
 */
int dbg_add_break(char **argv)
{
	int line_number, rc;
	struct breakpoint *node;
	
	line_number = strtol(argv[2], NULL, 10);

	rc = add_breakpoint(&head, exec_path, argv[1], line_number);

	if (rc == 0)
	{
		printf("Added breakpoint in file %s at line %d\n", argv[1], line_number);

		if (state == RUNNING)
		{
			add_breakpoint(&head_add, exec_path, argv[1], line_number);

			/* find last breakpoint and fill it's orig and oxcc */
			node = head;
			while (node->next != NULL)
			{
				node = node->next;
			}

			node->orig = ptrace(PTRACE_PEEKTEXT, pid, node->addr, 0);
			node->oxcc = (node->orig & 0xFFFFFFFFFFFFFF00UL) | 0xCC;
		}
	}

	return 0;
}

/**Remove breakpoint from the debuggee.
 * @param argv second element is used as a path to the compilation unit
               third element is used as a line number
 * @return 0
 */
int dbg_remove_break(char **argv)
{
	int line_number;
	
	line_number = strtol(argv[2], NULL, 10);
	
	remove_breakpoint(&head_add, argv[1], line_number);
	add_breakpoint(&head_remove, exec_path, argv[1], line_number);

	return 0;
}

/**Attach to an already running process.
 * @param argv second element used as a PID
 * @return 0
 */
int dbg_attach(char **argv)
{
	char proc_path[512];
	
	memset(proc_path, '\0', 512);
	snprintf(proc_path, 512, "/proc/%s", argv[1]);
	pid = strtol(argv[1], NULL, 10);

	if (directory_exists(proc_path) == 0)
	{
		if (attach(pid) == 0)
		{
			printf("Attach was successful.\n");
			state = RUNNING;
		}
		else
		{
			printf("Attach was unsuccessful.\n");
		}
	}
	else
	{
		printf("Process with such PID does not exist.\n");
	}

	return 0;
}

/**
 * Detach from the debuggee.
 * @param argv unused
 * @return 0
 */
int dbg_detach(char **argv)
{
	if (detach(pid, head) == 0)
	{
		printf("Detach was successful.\n");
		state = DEFAULT;
	}
	else
	{
		printf("Detach was unsuccessful.\n");
	}

	remove_all_breakpoints(&head);
	remove_all_breakpoints(&head_add);
	remove_all_breakpoints(&head_remove);

	pid = -1;

	return 0;
}

/**
 * Program entry point.
 * @param argc not used
 * @param argv not used
 * @return 0 on success, 1 otherwise
 */
int main(int argc, char **argv)
{
	char *line, *dup_line;
	int i;
	struct cmd *command;
	int my_argc;
	/* + 1 is here because command name is treated as argument also */
	char *my_argv[COMMAND_MAX_ARG + 1];
	int return_value;
	enum state_t state;
	char exec_path[512];
	pid_t pid;
	struct breakpoint *head;
	struct breakpoint *head_add;
	struct breakpoint *head_remove;
	
	ignore_sigint();
	
	printf("x86 ELF Debugger by Daniel Lovasko\n");
	state = DEFAULT;

	head = NULL;
	head_add = NULL;
	head_remove = NULL;

	struct cmd commands[] =
	{
		{"?",              command_help,         DEFAULT | PREPARING | RUNNING, 1},
		{"help",           command_help,         DEFAULT | PREPARING | RUNNING, 1},
		{"exit",           command_exit,         DEFAULT | PREPARING | RUNNING, 1},
		{"run",            command_run,            PREPARING,                   1},
		{"continue",       command_continue,       RUNNING,                     1},
		{"select",         command_select,         DEFAULT,                     2},
		{"list_files",     command_list_files,     PREPARING | RUNNING,         1},
		{"list_breaks",    command_list_breaks,    PREPARING | RUNNING,         1},
		{"list_functions", command_list_functions, PREPARING | RUNNING,         1},
		{"pid",            command_pid,            RUNNING,                     1},
		{"dump",           command_dump,           RUNNING,                     2},
		{"add_break",      command_add_break,      PREPARING | RUNNING,         3},
		{"remove_break",   command_remove_break,   PREPARING | RUNNING,         3},
		{"attach",         command_attach,         DEFAULT,                     2},
		{"detach",         command_detach,         RUNNING,                     1},
		{"stop",           command_stop,           RUNNING,                     1},
		NULL
	};
	
	/* strtok remarks:
	 *   strtok does not allocate new memory, just returns
	 *   pointer to the supplied string.
	 *   So freeing each token returned by strtok leads to
	 *   crash, when we try to free the whole string
	 *   allocated by read_line (due to double-freeing).
	 *   Freeing the line allocated by read_line at the end
	 *   of the cycle is enough.
	 */
	return_value = 0;
	do
	{
		print_state();
		
		/* block until return pressed */
		line = read_line(1024);

		/* try to find suitable command */
		command = NULL;
		for (i = 0; commands[i].cmd_name != NULL; i++)
		{
			if (starts_with(line, commands[i].cmd_name) == 0)
			{
				command = &commands[i];
				break;
			}
		}

		/* if we found one */
		if (command != NULL)
		{
			/* check state compatibility */
			if (state & command->compatible_state)
			{
				dup_line = strdup(line);

				/* parse the line */
				my_argc = 0;
				if ((my_argv[0] = strtok(dup_line, " ")) != NULL)
				{
					my_argc = 1;
					while ((my_argv[my_argc] = strtok(NULL, " ")) != NULL && my_argc < COMMAND_MAX_ARG)
					{
						my_argc++;
					}

					/* check parsed argument count */
					if (my_argc == command->expected_arg_count)
					{
						return_value = command->fnc(my_argv);
					}
					else
					{
						fprintf(stderr, "Wrong number of arguments. %d supplied, %d expected.\n", my_argc, command->expected_arg_count);
					}
				}

				free(dup_line);
			}
			else
			{
				fprintf(stderr, "You are not in a compatible state for this operation.\nCompatible states are: \n");
				if (command->compatible_state & DEFAULT)   fprintf(stderr, "\tdef\n");
				if (command->compatible_state & PREPARING) fprintf(stderr, "\tprep\n");
				if (command->compatible_state & RUNNING)   fprintf(stderr, "\trun\n");
				fprintf(stderr, "\n");
			}
		}
		else
		{
			fprintf(stderr, "Unknown command. In case of confusion, try 'help'\n");
		}

		free(line);
	}
	while (return_value != -1);

	printf("Bye.\n");
	return 0;
}

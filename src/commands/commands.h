#ifndef COMMANDS_H
#define COMMANDS_H

struct command_args_t
{
	
}

struct command_t
{
	const char *name;
	int (*function)(char**);
	int compatible_state;
	unsigned short expected_arg_count;
};

/**
 * Print help message.
 * @param argv unused
 * @return 0
 */
int 
command_help (char **argv);

/**
 * Try to terminate the debuggee and break from main loop.
 * @param argv unused
 * @return -1
 */
int 
command_exit (char **argv);

/**
 * Try to stop the debuggee.
 * @param argv unused
 * @return 0
 */
int 
command_stop (char **argv);

/**
 * Start the debuggee.
 * @param argv unused
 * @return 0
 */
int 
command_run (char **argv);

/**
 * Resume paused debuggee.
 * @param argv unused
 * @return 0
 */
int 
command_continue (char **argv);

/**
 * Basic checks on the executable.
 * @param argv second element is used as a path to the executable
 * @return 0
 */
int
command_select (char **argv);

/**
 * List compilation units.
 * @param argv unused
 * @return 0
 */
int
command_list_files (char **argv);

/**
 * List already inserted breakpoints.
 * @param argv unused
 * @return 0
 */
int
command_list_breaks (char **argv);

/**
 * List subprograms used in the executable.
 * @param argv unused
 * @return 0
 */
int
command_list_functions (char **argv);

/**
 * Print the PID of debuggee.
 * @param argv unused
 * @return 0
 */
int
command_pid (char **argv);

/**
 * Print data inside the process at specified address.
 * @param argv  second element is used as the address (hexadecimal)
 * @return 0
 */
int
command_dump (char **argv);

/**
 * Add breakpoint to the debuggee.
 * @param argv second element is used as a path to the compilation unit
               third element is used as a line number
 * @return 0
 */
int
command_add_break (char **argv);

/**
 * Remove breakpoint from the debuggee.
 * @param argv second element is used as a path to the compilation unit
 *             third element is used as a line number
 * @return 0
 */
int
command_remove_break (char **argv);

/**
 * Attach to an already running process.
 * @param argv second element used as a PID
 * @return 0
 */
int
command_attach (char **argv);

/**
 * Detach from the debuggee.
 * @param argv unused
 * @return 0
 */
int
command_detach (char **argv);

#endif


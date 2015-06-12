#include <stdio.h>

#include "runtime/general/general.h"

int 
runtime_command_help(struct command_args* cmd_args)
{
	(void) cmd_args;

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


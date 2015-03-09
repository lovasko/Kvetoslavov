#include <errno.h>
#include <stdio.h>

#include "runtime/general/select.h"
#include "state/state.h"
#include "util/util.h"

int 
runtime_command_select(struct command_args* cmd_args)
{
	char* full_path;
	
	errno = 0;
	full_path = realpath(cmd_args->text_args[1], NULL);

	if (errno != 0) {
		perror("select: realpath");
		return 0;
	}

	if (file_exists(full_path) == 0) {
		if (is_elf(full_path) == 0) {
			*(cmd_args->state) = PREPARING;
			*(cmd_args->exec_path) = full_path;
		} else {
			fprintf(stderr, "Selected file is not an ELF binary.\n");
		}
	} else {
		fprintf(stderr, "No such file: '%s'.\n", full_path);
	}

	return 0;
}


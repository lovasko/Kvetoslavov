#include "runtime/general/select.h"
#include "util.h"
#include "state.h"

#include <errno.h>
#include <stdio.h>

int 
runtime_command_select (struct command_args_t *args)
{
	char *full_path;
	
	errno = 0;
	full_path = realpath(args->text_args[1], NULL);

	if (errno != 0)
	{
		perror("select error");	
		return 0;
	}

	if (file_exists(full_path) == 0)
	{
		if (is_elf(full_path) == 0)
		{
			*(args->state) = PREPARING;
			*(args->exec_path) = full_path;
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


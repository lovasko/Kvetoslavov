#include <errno.h>
#include <stdio.h>

#include "runtime/general/general.h"
#include "runtime/state.h"
#include "util/util.h"

int 
runtime_command_select(struct context* ctx)
{
	char* full_path;
	
	errno = 0;
	full_path = realpath(ctx->text_args[1], NULL);

	if (errno != 0) {
		perror("select: realpath");
		return 0;
	}

	if (file_exists(full_path) == 0) {
		if (is_elf(full_path) == 0) {
			ctx->state = STATE_PREPARING;
			ctx->exec_path = full_path;
		} else {
			fprintf(stderr, "Selected file is not an ELF binary.\n");
		}
	} else {
		fprintf(stderr, "No such file: '%s'.\n", full_path);
	}

	return 0;
}


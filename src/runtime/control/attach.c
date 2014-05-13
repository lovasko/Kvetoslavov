#include "runtime/control/attach.h"
#include "runtime/util.h"
#include "state.h"

#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>

int
attach (pid_t pid)
{
	int wait_status;
	
	if (ptrace(PT_ATTACH, pid, NULL, NULL) == 0)
	{
		wait(&wait_status);
		return 0;
	}
	else 
	{
		return 1;
	}
}

int
parse_pid (char *text, pid_t *pid)
{
	long int input;
	
	errno = 0;
	input = strtol(text, NULL, 10);

	if (errno != 0)
	{
		if (errno == EINVAL)
			return 2;
		else if (errno == ERANGE)
			return 3;
		else
			return 4;
	}

	if (input < 0)
		return 1;

	*pid = input;
	return 0;
}

int 
runtime_command_attach (struct command_args_t *args)
{
	pid_t pid;
	int pid_parse_error;

	if ((pid_parse_error = parse_pid(args->text_args[1], &pid)) != 0)
	{
		switch (pid_parse_error)
		{
			case 1:
				fprintf(stderr, "The PID is negative.\n");
			break;

			default:
				fprintf(stderr, "Wrong PID format.\n");	
		}
		return 0;
	}

	if (runtime_process_exists(pid) == 0)
	{
		if (attach(pid) == 0)
		{
			printf("Attach was successful.\n");

			*(args->state) = RUNNING;
			*(args->pid) = pid;
			/* todo get executable path */
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


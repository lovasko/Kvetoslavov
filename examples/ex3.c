#include <unistd.h>
#include <stdio.h>

int
main(void)
{
	char* args[3];

	printf("Going to execute 'ls /'\n");

	args[0] = "ls";
	args[1] = "/";
	args[2] = NULL;

	execvp("ls", args);
}

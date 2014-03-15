#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	printf("Going to execute 'ls /'\n");

	char *args[3];
	args[0] = "ls";
	args[1] = "/";
	args[2] = NULL;

	execvp("ls", args);
}

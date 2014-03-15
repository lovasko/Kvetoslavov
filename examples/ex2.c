#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	printf("before fork\n");
	
	if (fork() == 0)
	{
		if (fork() == 0)
		{
			printf("boy\n");
		}
		else
		{
			printf("father\n");
		}
	}
	else
	{
		printf("grandfather\n");
	}
	
	return 0;
}

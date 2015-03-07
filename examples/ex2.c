#include <unistd.h>
#include <stdio.h>

int
main(void)
{
	printf("before fork\n");
	
	if (fork() == 0) {
		if (fork() == 0) {
			printf("child\n");
		} else {
			printf("father\n");
		}
	} else {
		printf("grandfather\n");
	}
	
	return 0;
}

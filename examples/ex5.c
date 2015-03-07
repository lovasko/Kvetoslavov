#include <stdio.h>
#include <unistd.h>

int
main(void)
{
	int i;

	for(i = 0; ; i++) {
		printf("%d\n", i);
	  sleep(2);
	}
}


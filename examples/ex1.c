#include <stdio.h>

int i;

int
main(void)
{
	for (i = 0; ; i++) {
		printf("%da\n", i);
		printf("%db\n", i);
	}
}


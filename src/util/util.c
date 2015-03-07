#include "util.h"

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

void ignore_sigint()
{
	struct sigaction new_sa;
	struct sigaction old_sa;

	sigfillset(&new_sa.sa_mask);
	new_sa.sa_handler = SIG_IGN;
	new_sa.sa_flags = 0;

	if (sigaction(SIGINT, &new_sa, &old_sa) == 0 && old_sa.sa_handler != SIG_IGN)
	{
		sigaction(SIGINT, &new_sa, 0);
	}
}

int 
file_exists (char *path)
{
	int fd;
 
	fd = open(path, O_RDONLY);
	close(fd);
	
	if (fd < 0) 
		return 1;
	else 
		return 0;
}

int 
is_elf (char *path)
{
	int fd;
	char magic[4]; 
	
	fd = open(path, O_RDONLY);
    
	/* test the beginning of file for ELF magic number */
	memset(magic, 0, 4);
	read(fd, &magic, 4);

	close(fd);

	if (strncmp(magic, "\177ELF", 4) == 0)
		return 0;
	else
		return 1;
}


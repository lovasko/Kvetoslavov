#include "util.h"
#include "platform.h"

void 
print_address (pid_t pid, unsigned long address)
{
	unsigned long value;
	int i; 
	
	value = ptrace(PTRACE_PEEKTEXT, pid, address, 0);
	
	printf("0x%X: ", address);
	for (i = 0; i < sizeof(unsigned long); i++)
	{
		/* create mask for one byte at i-th position: (unsigned long)0xFF << i*8
		 * apply it to value by & operator
		 * shift i-th byte to least significant position: >> i*8
		 */
		printf("%02X ", (value & ((unsigned long)0xFF << i*8)) >> i*8);
	}
	printf("\n");
}

void 
print_hex (unsigned long number)
{
	int i;
	
	printf("%ld: ", number);
	for (i = 0; i < sizeof(unsigned long); i++)
	{
		/* create mask for one byte at i-th position: (unsigned long)0xFF << i*8
		 * apply it to value by & operator
		 * shift i-th byte to least significant position: >> i*8
		 */
		printf("%02X ", (number & ((unsigned long)0xFF << i*8)) >> i*8);
	}
	printf("\n");
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

int 
directory_exists (char *path)
{
	struct stat sb;

	if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))
		return 0;
	else
		return 1;
}


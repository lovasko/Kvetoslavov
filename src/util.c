#include "util.h"

#include "platform.h"
                                     
/**Accept bytes from standard input, until end of line or max_length is reached.
 * @arg maximum length of line in bytes, including terminating NULL
 * @return NULL terminated array of char
 */
char *read_line(int max_length)
{
	char *line;
	int i;
	char c;
	
	/* this malloc should be freed by the caller */
	line = (char*)malloc(sizeof(char) * max_length);
	memset(line, 0, max_length);
	
	/* upper limit is max_lenght - 1 to ensure NULL ending */
	for (i = 0; i < max_length - 1; i++) 
	{ 
		read(0, &c, 1);
		if(c == '\n') break;
		
		line[i] = c;
	}
	
	return line;
}



/**Pretty print address and it's content to stdout.
 * @arg process ID
 * @arg address
 */
void print_address(pid_t pid, unsigned long address)
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



/**Pretty print number in hexadecimal format.
 * @arg number
 */
void print_hex(unsigned long number)
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


/**Check whether file exists.
 * @arg path to file
 * @return 0 if file exists, 1 otherwise otherwise
 */
int file_exists(char *path)
{
	int fd;
	 
	fd = open(path, O_RDONLY);
	
	if (fd < 0) return 1;
	else 
	{
		close(fd);
		return 0;
	}
}



/**Check whether file is ELF binary.
 * @arg path to file
 * @return 0 if file is ELF binary, 1 otherwise
 */  
int is_elf(char *path)
{
	int fd;
	char magic[4]; 
	
	fd = open(path, O_RDONLY);
    
	/* test the beginning of file for ELF magic number */
	memset(magic, 0, 4);
	read(fd, &magic, 4);
    
	if (strncmp(magic, "\177ELF", 4) == 0)
	{
		close(fd);
		return 0;
	}
	else
	{
		close(fd);
		return 1;
	}
}

/**Check whether string starts with given prefix.
 * @arg string
 * @arg prefix
 */
int starts_with(char *string, char *prefix)
{
	if (strncmp(string, prefix, strlen(prefix)) == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/**Check whether directory exists.
 * @arg path to directory
 * @return 0 if exists, 1 otherwise
 */
int directory_exists(char *path)
{
	struct stat sb;

	if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

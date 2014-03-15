#include <fcntl.h>      /* open */
#include <unistd.h>     /* read, close */
#include <string.h>     /* memset */
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* malloc */
#include <sys/types.h>
#include <sys/ptrace.h> /* ptrace */
#include <elf.h>
#include <libelf.h>
#include <sys/stat.h>

/**Accept bytes from standard input, until end of line or max_length is reached.
 * @arg maximum length of line in bytes, including terminating NULL
 * @return NULL terminated array of char
 */
char *read_line(int max_length);



/**Pretty print address and it's content to stdout.
 * @arg process ID
 * @arg address
 */
void print_address(pid_t pid, unsigned long address);



/**Pretty print number in hexadecimal format.
 * @arg number
 */
void print_hex(unsigned long number);



/**Check whether file exists.
 * @arg path to file
 * @return 1 if file exists, 0 otherwise
 */
int file_exists(char *path);



/**Check whether file is ELF binary.
 * @arg path to file
 * @return 0 if file is ELF binary, 1 otherwise
 */  
int is_elf(char *path);


/**Check whether string starts with given prefix.
 * @arg string
 * @arg prefix
 * @return 0 if string starts with prefix, 1 otherwise 
 */  
int starts_with(char *string, char *prefix);


/**Check whether directory exists.
 * @arg path to directory
 * @return 0 if exists, 1 otherwise
 */
int directory_exists(char *path);

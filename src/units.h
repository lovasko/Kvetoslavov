#include <dwarf.h>
#include <libdwarf.h>
#include <stdio.h>
#include <fcntl.h>      
#include <unistd.h>     

/**Print all compilation units inside the executable
 * @arg path to executable
 */
void list_units(char *path);
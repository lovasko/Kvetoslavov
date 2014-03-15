#include <stdio.h>  /* printf */
#include <string.h> /* strcmp */
#include <stdlib.h> /* malloc */
#include <unistd.h>
#include <fcntl.h>
#include <dwarf.h>
#include <libdwarf.h>

#ifndef SUBPROGRAM_H
#define SUBPROGRAM_H

/**Print list of subprograms(functions) within one or all compilation units
 * @arg path to executable
 * @arg path to compilation unit (if NULL, all units are being processed)
 */
void list_subprograms(char *exe, char *unit);

#endif

#include <stdio.h>  /* printf */
#include <string.h> /* strcmp */
#include <stdlib.h> /* malloc */
#include <unistd.h>
#include <fcntl.h>
#include <dwarf.h>
#include <libdwarf.h>

#ifndef BREAKPOINT_H
#define BREAKPOINT_H

/**Linked list to store breakpoint information */
struct breakpoint
{
	int line;                 /* number of the breakpoint's line */
	char *path;               /* path to file in which is the breakpoint */
	long orig;                /* original instruction, before substitution for 0xCC */
	long oxcc;                /* original instruction, with first byte modified to 0xCC */
	unsigned long addr;       /* address of the address in executable corresponding to line number */
	
	struct breakpoint *next;  /* pointer to next node in linked list */
};



/**Add breakpoint on specified line in specified file
 * @arg pointer to head of breakpoint linked list
 * @arg path to executable
 * @arg path to compilation unit
 * @arg line number
 * @return 0 on success, 1 otherwise
 */
int add_breakpoint(struct breakpoint **bp, char *exe, char *path, int line);



/**Print information about all set breakpoints to stdout.
 * @arg pointer to head of breakpoint linked list
 */
void print_breakpoints(struct breakpoint *bp);



/**Remove matching breakpoint
 * @arg pointer to head of breakpoint linked list
 * @arg matching path
 * @arg matching line
 */
void remove_breakpoint(struct breakpoint **bp, char *path, int line);

#endif

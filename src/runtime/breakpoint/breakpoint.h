#ifndef KVETOSLAVOV_RUNTIME_BREAKPOINT_BREAKPOINT_H
#define KVETOSLAVOV_RUNTIME_BREAKPOINT_BREAKPOINT_H

#include <sys/queue.h>

struct breakpoint
{
	int line; /* number of the breakpoint's line */
	char* path; /* path to file in which is the breakpoint */
	long orig; /* original instruction, before substitution for 0xCC */
	long oxcc; /* original instruction, with first byte modified to 0xCC */
	unsigned int addr; /* address of the address in executable corresponding to 
	                     * line number */

	struct breakpoint* next;  /* pointer to next node in linked list */
};

#endif


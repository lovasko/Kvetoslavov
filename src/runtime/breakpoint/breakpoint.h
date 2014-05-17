#ifndef KVETOSLAVOV_RUNTIME_BREAKPOINT
#define KVETOSLAVOV_RUNTIME_BREAKPOINT

struct breakpoint_t
{
	int line; /* number of the breakpoint's line */
	char *path; /* path to file in which is the breakpoint */
	long orig; /* original instruction, before substitution for 0xCC */
	long oxcc; /* original instruction, with first byte modified to 0xCC */
	unsigned long addr; /* address of the address in executable corresponding to 
	                     * line number */

	struct breakpoint_t *next;  /* pointer to next node in linked list */
};

#endif


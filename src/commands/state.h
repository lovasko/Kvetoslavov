#ifndef STATE_H
#define STATE_H

enum state_t
{
	DEFAULT = 1,
	PREPARING = 2,
	RUNNING = 4,
	ALL_STATES = 7
};

/**
 * Inform user about the debugging phase we are in.
 * @param state the state to be printed
 */
void 
state_print (enum state_t state);

/**
 * Check if the state is compatible.
 * @param base 
 * @param checked 
 * @return 1 if check has base inside, 0 otherwise
 */
int 
state_is_compatible (enum state base, int checked);

#endif

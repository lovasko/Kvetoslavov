#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include "breakpoint.h"
#include <sys/ptrace.h>
#include <errno.h>



#include <sys/wait.h>
#ifndef CONTROL_H
#define CONTROL_H


/**Common checking routine after calling wait().
 * @arg wait() result
 * @arg PID of the process
 * @arg head of breakpoint linked list
 * @return 0 if program exited, 1 if program paused for some reason, -1 otherwise
 */
int after_wait(int wait_status, pid_t *pid, struct breakpoint **bp);

/**Start the selected program and insert breakpoints.
 * @arg head of linked list
 * @arg path to executable
 * @arg output parameter of started process's PID
 * @return 0 if program exited, -1 if program paused for some reason, -1 otherwise
 */
int start(struct breakpoint **bp, char *path, pid_t *pid);

/**Place the original instruction, simulate one-step progress, insert the breakpoint back and carry on.
 * @arg head of linked list
 * @arg PID to resume
 * @return 0 if program exited, -1 if program paused for some reason, -1 otherwise
 */
int resume(struct breakpoint **bp, pid_t *pid, struct breakpoint *to_add, struct breakpoint *to_remove);

/**Attach to running process, and pause it.
 * @arg PID to attach to
 * @return 0 if attach was successful, 1 otherwise
 */
int attach(pid_t pid);

/**Fix all breakpoints and detach from running process.
 * @arg PID to detach from
 * @return 0 if detach was successful, 1 otherwise
 */
int detach(pid_t pid, struct breakpoint *bp);

/**Send SIGKILL.
 * @arg PID
 * @return 0 if successful, 1 otherwise
 */
int stop(pid_t pid);

#endif

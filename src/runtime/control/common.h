#ifndef KVETOSLAVOV_RUNTIME_CONTROL_COMMON_H
#define KVETOSLAVOV_RUNTIME_CONTROL_COMMON_H

#include "runtime/breakpoint/breakpoint.h"

#include <unistd.h>

int 
after_wait(int wait_status, pid_t* pid, struct breakpoint** bp);

/**
 * Checks whether process exists.
 * @param pid the PID of the process
 * @return 0 if exists, 1 otherwise
 */
int
runtime_process_exists(pid_t pid);

#endif


#ifndef KVETOSLAVOV_RUNTIME_UTIL_H
#define KVETOSLAVOV_RUNTIME_UTIL_H

#include <unistd.h>

/**
 * Checks whether process exists.
 * @param pid the PID of the process
 * @return 0 if exists, 1 otherwise
 */
int
runtime_process_exists(pid_t pid);

#endif


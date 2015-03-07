#ifndef KVETOSLAVOV_RUNTIME_CONTROL_COMMON_H
#define KVETOSLAVOV_RUNTIME_CONTROL_COMMON_H

#include "runtime/breakpoint/breakpoint.h"

#include <unistd.h>

int 
after_wait(int wait_status, pid_t* pid, struct breakpoint** bp);

#endif


#ifndef KVETOSLAVOV_RUNTIME_BREAKPOINT_COMMON_H
#define KVETOSLAVOV_RUNTIME_BREAKPOINT_COMMON_H

#include "runtime/breakpoint/breakpoint.h"

int
compilation_unit_exists(char* exec_path, char* unit_name);

int
remove_breakpoint(struct breakpoint** head, struct breakpoint* to_remove);

int
remove_all_breakpoints(struct breakpoint** head);

int
add_breakpoint(struct breakpoint* head, struct breakpoint* to_add);

#endif


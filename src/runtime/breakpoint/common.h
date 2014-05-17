#ifndef KVETOSLAVOV_RUNTIME_BREAKPOINT_COMMON
#define KVETOSLAVOV_RUNTIME_BREAKPOINT_COMMON

#include "runtime/breakpoint/breakpoint.h"

int
compilation_unit_exists (char *exec_path, char *unit_name);

int
remove_breakpoint (struct breakpoint_t **head, struct breakpoint_t *to_remove);

int
add_breakpoint (struct breakpoint_t *head, struct breakpoint_t *to_add);

#endif


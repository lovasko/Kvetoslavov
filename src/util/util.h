#ifndef KVETOSLAVOV_UTIL_UTIL_H
#define KVETOSLAVOV_UTIL_UTIL_H

/**
 * Set SIGINT handler to ignore for this process.
 */
void ignore_sigint();

/**
 * Check whether file exists.
 * @param path to file
 * @return 1 if file exists, 0 otherwise
 */
int 
file_exists(char *path);

/**
 * Check whether file is an ELF binary.
 * @param path to file
 * @return 0 if file is an ELF binary, 1 otherwise
 */  
int 
is_elf(char *path);

#endif


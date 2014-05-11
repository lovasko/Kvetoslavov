#ifndef LINE_H
#define LINE_H

/**
 * Counts the number of delimiters in the line. 
 * Consecutive delimiters are treated as one.
 * @param line the text to be searched
 * @param delimiter the delimiter
 * @return number of delimiters in the text
 */
unsigned int
line_argument_count (char *line, char delimiter);

/**
 * Accept bytes from standard input, until end of line or max_length is reached.
 * @param max_length maximum length of line in bytes, including terminating NULL
 * @return NULL-terminated array of char
 */
char*
line_read_stdin (unsigned int max_length);

/**
 * Returns array of strings.
 * Consecutive delimiters are treated as one.
 * @param line the text
 * @param delimiter the delimiter
 * @return array of char pointers 
 */
char**
line_get_arguments (char **line, char delimiter);

/**
 * Check whether the line starts with the given prefix.
 * @param line the line 
 * @param prefix the prefix
 * @return 0 if line starts with prefix, 1 otherwise 
 */  
int 
line_starts_with (char *line, const char *prefix);

#endif


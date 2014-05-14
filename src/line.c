#include "line.h"

#include <string.h>
#include <stdlib.h>
#include <unistd.h>

unsigned int
line_argument_count (char *line, char delimiter)
{
	unsigned int i, count;

	count = 0;
	for	(i = 0; line[i]; i++)
	{
		if (line[i] == delimiter)	
		{
			if (line[i+1] == line[i] || line[i+1] == '\0')
				continue;

			count++;
		}
	}

	return count;
}

char**
line_get_arguments (char **line, char delimiter)
{
	char **result;
	char *token;
	unsigned int index;
	
	result = (char**)malloc(sizeof(char*) * line_argument_count(*line,
	    delimiter));	
	index = 0;

	while ((token = strsep(line, &delimiter)) != NULL)	

	{
		if (token[0] == '\0')
			continue;
		
		result[index++] = token;
	}

	return result;
}

char*
line_read_stdin (unsigned int max_length)
{
	char *line;
	int i;
	char c;
	
	/* this malloc should be freed by the caller */
	line = (char*)malloc(sizeof(char) * max_length);
	memset(line, 0, max_length);
	
	/* upper limit is max_length - 1 to ensure NULL ending */
	for (i = 0; i < max_length - 1; i++) 
	{ 
		read(0, &c, 1);
		if(c == '\n') 
			break;
		
		line[i] = c;
	}
	
	return line;
}

int 
line_starts_with (char *line, const char *prefix)
{
	if (strncmp(line, prefix, strlen(prefix)) == 0)
		return 0;
	else
		return 1;
}


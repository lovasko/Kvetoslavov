#include "breakpoint.h"
        
/**Add breakpoint on specified line in specified file and obtain address in executable
 * @arg pointer to head of breakpoint linked list
 * @arg path to executable
 * @arg breakpoint file's path
 * @arg breakpoint's line
 * @return 0 on success, 1 otherwise
 */
int add_breakpoint(struct breakpoint **bp, char *exe, char *path, int line)
{
	int fd;
	Dwarf_Debug dbg = 0;
	Dwarf_Error err;
	Dwarf_Unsigned cu_header_length, abbrev_offset, next_cu_header;
	Dwarf_Half version_stamp, address_size;
	Dwarf_Die no_die = 0, child_die;
	char *comp_dir_string;
	Dwarf_Attribute comp_dir_attr;
	Dwarf_Attribute name_attr;
	char *name_string;
	char die_path[512];
	int k;
	Dwarf_Line li;
	Dwarf_Line *lines;
	Dwarf_Addr pc = 0;
	Dwarf_Unsigned lineno = 0;
	Dwarf_Signed count;
	struct breakpoint *node;
		
	fd = open(exe, O_RDONLY);
	dwarf_init(fd, DW_DLC_READ, 0, 0, &dbg, &err);
	

	/* dwarf debug information consist of list of compilation units
	 * compilation unit is source file that was used in compilation process
	 * each compilation unit consists of list of tags
	 * first tag of each compilation unit is DW_TAG_compile_unit
	 * each tag contains attributes
	 * in particular we are interested in DW_AT_comp_dir and DW_AT_name of DW_TAG_compile_unit
	 */
	
	dwarf_next_cu_header(dbg, &cu_header_length, &version_stamp, &abbrev_offset, &address_size, &next_cu_header, &err);
	
	/* by calling this function with the "no_die" argument (which is 0), we get the first tag available, the DW_TAG_compile_unit */
	dwarf_siblingof(dbg, no_die, &child_die, &err);

	while (1)
	{
		/* get the attributes and convert them to strings */
		dwarf_attr(child_die, DW_AT_comp_dir, &comp_dir_attr, &err);
		dwarf_formstring(comp_dir_attr, &comp_dir_string, &err);

		dwarf_attr(child_die, DW_AT_name, &name_attr, &err);
		dwarf_formstring(name_attr, &name_string, &err);

		/* comp_dir_string is dirname part of the path, and name the basename, so we put them together */
		memset(die_path, '\0', 512);
		snprintf(die_path, 512, "%s/%s", comp_dir_string, name_string);
		
		/* if compilation unit's path is the same
		 * search for the matching line number
		 * if line numbers match, save the address of corresponding instruction in executable
		 */

		if (strcmp(path, die_path) == 0)
		{
			dwarf_srclines(child_die, &lines, &count, &err);
			
			for (k = 0; k < count; k++)
			{
				li =  lines[k];
			
				dwarf_lineaddr(li, &pc, &err);
				dwarf_lineno(li, &lineno, &err);
				
				if (line == lineno)
				{
					/* if there is no breakpoint yet */
					if (*bp == NULL)
					{
						*bp = (struct breakpoint *)malloc(sizeof(struct breakpoint));
						(*bp)->line = line;
						(*bp)->path = strdup(path);
						(*bp)->addr = pc;
						(*bp)->next = NULL;
						
						//printf("Added breakpoint in file %s at line %d (0x%X).\n", (*bp)->path, (*bp)->line, pc);
					}
					
					/* find the last breakpoint, and create new one at the ->next */
					else
					{
						/* we can do this, since bp is not NULL */
						node = (*bp);
						while (node->next != NULL) 
						{
							node = node->next;
						}
						
						node->next = (struct breakpoint *)malloc(sizeof(struct breakpoint));
						node->next->line = line;
						node->next->path = strdup(path);
						node->next->addr = pc;
						node->next->next = NULL;
						
						//printf("Added breakpoint in file %s at line %d (0x%X).\n", node->next->path, node->next->line, pc);
					}
					
					dwarf_finish(dbg, &err);
					close(fd);
					return 0;
				}
			}
			
			printf("Could not place breakpoint at that line.\n");
			dwarf_finish(dbg, &err);
			close(fd);
			return 1;
		}
		
		/* move to next compilation unit, if possible */
		dwarf_next_cu_header(dbg, &cu_header_length, &version_stamp, &abbrev_offset, &address_size, &next_cu_header, &err);
		if(dwarf_siblingof(dbg, no_die, &child_die, &err)!= DW_DLV_OK)
		{
			break;
		}
	}
	
	printf("No such compilation unit.\n");
	
	dwarf_finish(dbg, &err);
	close(fd);
	return 1;
}



/**Print information about all set breakpoints
 * @arg pointer to head of breakpoint linked list
 */
void print_breakpoints(struct breakpoint *bp)
{
	struct breakpoint *node;

	if (bp == NULL)
	{
		printf("No breaks yet.\n");
		return;
	}
	
	node = bp;
	while (node != NULL)
	{
		printf("Breakpoint in file %s at line %d (0x%X)\n", node->path, node->line, (unsigned long)node->addr);
		node = node->next;
	}
}



/**Remove matching breakpoint
 * @arg pointer to head of breakpoint linked list
 * @arg matching path
 * @arg matching line
 */
void remove_breakpoint(struct breakpoint **bp, char *path, int line)
{
	struct breakpoint *next;
	struct breakpoint **prev, **node;
	
	if (*bp == NULL)
	{
		return;
	}
	
	/* check first node */
	if (line == (*bp)->line && strcmp(path, (*bp)->path) == 0)
	{
		next = (*bp)->next;
		free(*bp);
		*bp = next;

		printf("Breakpoint successfuly removed.\n");
		return;
	}

	/* continue from second node until end of list
	 * in case of match in node n,
	 *   point previous node to n->next */
	prev = bp;
	node = &(*bp)->next;

	while (*node != NULL)
	{
		if (line == (*node)->line && strcmp(path, (*node)->path) == 0)
		{
			(*prev)->next = (*node)->next;
			free(*node);

			printf("Breakpoint successfuly removed.\n");
			return;
		}

		*node = (*node)->next;
	}
}

/**Remove all breakpoints.
 * @arg pointer to head
 */
void remove_all_breakpoints(struct breakpoint **bp)
{
	struct breakpoint **node, **next, *middle;
	
	if (*bp == NULL) return;
	
	node = bp;
	while (*node != NULL)
	{
		middle = (*node)->next;
		next = &middle;

		free(*node);
		node = next;
	}

	*bp = NULL;
}

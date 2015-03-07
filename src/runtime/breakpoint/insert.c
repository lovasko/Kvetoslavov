#include <sys/ptrace.h>

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <dwarf.h>
#include <libdwarf.h>

#include "runtime/breakpoint/breakpoint.h"
#include "runtime/breakpoint/common.h"
#include "runtime/breakpoint/insert.h"
#include "state/state.h"

static int
search_for_breakpoint (char* exec_path, char* unit_path, int line, 
    struct breakpoint* found)
{
	Dwarf_Addr pc;
	Dwarf_Attribute comp_dir_attr;
	Dwarf_Attribute name_attr;
	Dwarf_Debug dbg;
	Dwarf_Die child_die;
	Dwarf_Die no_die;
	Dwarf_Error err;
	Dwarf_Half address_size;
	Dwarf_Half version_stamp;
	Dwarf_Line li;
	Dwarf_Line* lines;
	Dwarf_Off abbrev_offset;
	Dwarf_Signed count;
	Dwarf_Unsigned cu_header_length;
	Dwarf_Unsigned lineno;
	Dwarf_Unsigned next_cu_header;
	char die_path[PATH_MAX];
	char* comp_dir_string;
	char* name_string;
	int fd;
	int k;

	dbg = 0;
	lineno = 0;
	no_die = 0;
	pc = 0;
		
	if ((fd = open(exec_path, O_RDONLY) < 0)) {
		perror("open");	
		return 1;
	}

	if (dwarf_init(fd, DW_DLC_READ, 0, 0, &dbg, &err) != DW_DLV_OK) {
		printf("%s\n", dwarf_errmsg(err));
		return 1;
	}
	
	dwarf_next_cu_header(dbg, &cu_header_length, &version_stamp, &abbrev_offset, 
	    &address_size, &next_cu_header, &err);
	
	dwarf_siblingof(dbg, no_die, &child_die, &err);

	while (1) {
		dwarf_attr(child_die, DW_AT_comp_dir, &comp_dir_attr, &err);
		dwarf_formstring(comp_dir_attr, &comp_dir_string, &err);

		dwarf_attr(child_die, DW_AT_name, &name_attr, &err);
		dwarf_formstring(name_attr, &name_string, &err);

		memset(die_path, '\0', PATH_MAX);
		snprintf(die_path, PATH_MAX, "%s/%s", comp_dir_string, name_string);
		
		if (strcmp(unit_path, die_path) == 0) {
			dwarf_srclines(child_die, &lines, &count, &err);
			
			for (k = 0; k < count; k++) {
				li = lines[k];
			
				dwarf_lineaddr(li, &pc, &err);
				dwarf_lineno(li, &lineno, &err);

				if ((unsigned int)line == lineno) {
					found->line = line;
					found->path = strdup(unit_path);
					found->addr = pc;

					dwarf_finish(dbg, &err);
					close(fd);
					return 0;
				}
			}
			
			dwarf_finish(dbg, &err);
			close(fd);
			return 1;
		}
		
		dwarf_next_cu_header(dbg, &cu_header_length, &version_stamp, &abbrev_offset, 
		    &address_size, &next_cu_header, &err);

		if (dwarf_siblingof(dbg, no_die, &child_die, &err) != DW_DLV_OK)
			break;
	}
	
	dwarf_finish(dbg, &err);
	close(fd);
	return 2;
}

int
runtime_command_breakpoint_insert(struct command_args* cmd_args)
{
	int line_number;
	struct breakpoint new_node;
	int search_retval;
	
	errno = 0;
	line_number = strtol(cmd_args->text_args[2], NULL, 10);
	if (errno != 0) {
		fprintf(stderr, "Wrong line number format.\n");
		return 0;
	}

	if (compilation_unit_exists(*(cmd_args->exec_path), cmd_args->text_args[1]) 
	    != 0) {
		fprintf(stderr, "No such compilation unit.\n");
		return 0;
	}

	search_retval = search_for_breakpoint(*(cmd_args->exec_path), 
	  cmd_args->text_args[1], line_number, &new_node);

	if (search_retval == 0) {
		if (*(cmd_args->state) == RUNNING) {
			new_node.orig = ptrace(PT_READ_I, *(cmd_args->pid), 
			    (caddr_t)new_node.addr, 0);
			new_node.oxcc = (new_node.orig & 0xFFFFFFFFFFFFFF00UL) | 0xCC;
		
			add_breakpoint(*(cmd_args->head_add), &new_node);
			fprintf(stdout, "Successfully scheduled breakpoint for addition.\n");
		}

		add_breakpoint(*(cmd_args->head), &new_node);
		printf("Added breakpoint in file %s at line %d\n", cmd_args->text_args[1], 
		    line_number);
	}

	if (search_retval == 1)
		fprintf(stderr, "Could not place breakpoint - invalid line.\n");	

	if (search_retval == 2)
		fprintf(stderr, "No such compilation unit.\n");	

	return 0;
}


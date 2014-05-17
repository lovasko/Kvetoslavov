#include "runtime/breakpoint/common.h"
#include "runtime/breakpoint/breakpoint.h"

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <dwarf.h>
#include <libdwarf.h>
#include <sys/syslimits.h>

int
compilation_unit_exists (char *exec_path, char *unit_path)
{
	int fd;
	Dwarf_Debug dbg;
	Dwarf_Error err;
	Dwarf_Unsigned cu_header_length;
	Dwarf_Unsigned abbrev_offset;
	Dwarf_Unsigned next_cu_header;
	Dwarf_Half version_stamp;
	Dwarf_Half address_size;
	Dwarf_Die no_die = 0;
	Dwarf_Die child_die;
	Dwarf_Attribute comp_dir_attr;
	Dwarf_Attribute name_attr;
	char *comp_dir_string;
	char *name_string;
	char die_path[PATH_MAX];

	if ((fd = open(exec_path, O_RDONLY) < 0))
	{
		perror("open");	
		return 1;
	}

	if (dwarf_init(fd, DW_DLC_READ, 0, 0, &dbg, &err) != DW_DLV_OK)
	{
		printf("%s\n", dwarf_errmsg(err));
	}

	dwarf_next_cu_header(dbg, &cu_header_length, &version_stamp, &abbrev_offset, 
	    &address_size, &next_cu_header, &err);

	dwarf_siblingof(dbg, no_die, &child_die, &err);

	while (1)
	{
		dwarf_attr(child_die, DW_AT_comp_dir, &comp_dir_attr, &err);
		dwarf_formstring(comp_dir_attr, &comp_dir_string, &err);

		dwarf_attr(child_die, DW_AT_name, &name_attr, &err);
		dwarf_formstring(name_attr, &name_string, &err);

		memset(die_path, '\0', PATH_MAX);
		snprintf(die_path, PATH_MAX, "%s/%s", comp_dir_string, name_string);

		if (strncmp(unit_path, die_path, PATH_MAX) == 0)
			return 0;

		dwarf_next_cu_header(dbg, &cu_header_length, &version_stamp, &abbrev_offset, 
		    &address_size, &next_cu_header, &err);
		if (dwarf_siblingof(dbg, no_die, &child_die, &err) != DW_DLV_OK)
			break;
	}

	return 1;
}

int
remove_breakpoint (struct breakpoint_t **head, struct breakpoint_t *to_remove)
{
	struct breakpoint_t *next;
	struct breakpoint_t **prev, **node;

	/* check first node */
	if (to_remove->line == (*head)->line && 
	    strcmp(to_remove->path, (*head)->path) == 0)
	{
		next = (*head)->next;
		free(*head);
		*head = next;

		return 0;
	}

	/* 
	 * continue from second node until end of list
	 * in case of match in node n,
	 * point previous node to n->next 
	 */
	prev = head;
	node = &(*head)->next;

	while (*node != NULL)
	{
		if (to_remove->line == (*node)->line && 
		    strcmp(to_remove->path, (*node)->path) == 0)
		{
			(*prev)->next = (*node)->next;
			free(*node);

			return 0;
		}

		*node = (*node)->next;
	}

	return 1;
}

int
add_breakpoint (struct breakpoint_t *head, struct breakpoint_t *to_add)
{
	struct breakpoint_t *node;

	if (head == NULL)
	{
		head = (struct breakpoint_t*)malloc(sizeof(struct breakpoint_t));
		head->line = to_add->line;
		head->path = strdup(to_add->path);
		head->addr = to_add->addr;
		head->next = NULL;
	}
	else
	{
		for (node = head->next; node->next; node = node->next);	

		node->next = (struct breakpoint_t*)malloc(sizeof(struct breakpoint_t));
		node->next->line = to_add->line;
		node->next->path = strdup(to_add->path);
		node->next->addr = to_add->addr;
		node->next->next = NULL;
	}

	return 0;
}


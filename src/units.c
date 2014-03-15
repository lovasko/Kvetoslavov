#include "units.h"

/**Print all compilation units inside the executable
 * @arg path to executable
 */
void list_units(char *path)
{
	int fd;
	Dwarf_Debug dbg = 0;
	Dwarf_Error err;
	Dwarf_Unsigned cu_header_length, abbrev_offset, next_cu_header;
	Dwarf_Half version_stamp, address_size;
	Dwarf_Die no_die = 0, cu_die, child_die;
	Dwarf_Attribute comp_dir, name;
	char *comp_dir_string;
	char *name_string;
	
	
	fd = open(path, O_RDONLY);
	dwarf_init(fd, DW_DLC_READ, 0, 0, &dbg, &err);
	
	dwarf_next_cu_header(dbg, &cu_header_length, &version_stamp, &abbrev_offset, &address_size, &next_cu_header, &err);
	dwarf_siblingof(dbg, no_die, &child_die, &err);
	
	while (1)
	{
		dwarf_attr(child_die, DW_AT_comp_dir, &comp_dir, &err);
		dwarf_formstring(comp_dir, &comp_dir_string, &err);
	
		dwarf_attr(child_die, DW_AT_name, &name, &err);
		dwarf_formstring(name, &name_string, &err);
		
		printf("%s/%s\n", comp_dir_string, name_string);

		dwarf_next_cu_header(dbg, &cu_header_length, &version_stamp, &abbrev_offset, &address_size, &next_cu_header, &err);

		if (dwarf_siblingof(dbg, no_die, &child_die, &err) != DW_DLV_OK)
		{
			break;
		}
	}
	
	dwarf_finish(dbg, &err);
	close(fd);
}

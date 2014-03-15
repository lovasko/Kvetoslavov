#include "subprogram.h"

/**Print list of subprograms(functions) within one or all compilation units
 * @arg path to executable
 * @arg path to compilation unit (if NULL, all units are being processed)
 */
void list_subprograms(char *exe, char *unit)
{
	int fd;	
	Dwarf_Debug dbg = 0;
	Dwarf_Error err;
	Dwarf_Unsigned cu_header_length, abbrev_offset, next_cu_header;
	Dwarf_Half version_stamp, address_size;
	Dwarf_Die no_die = 0, child_die, subprogram_die, argument_die;
	Dwarf_Attribute comp_dir_attr, name_attr;
	char die_path[512];
	char *comp_dir_string;
	char *name_string;
	Dwarf_Half tag, arg_tag;
	
	
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
		
		/* check the unit name only if not NULL, which means we list all */
		if (unit == NULL || strcmp(unit, die_path) == 0)
		{
			printf("%s:\n", die_path);
			dwarf_child(child_die, &subprogram_die, &err);
			
			while (1)
			{
				dwarf_tag(subprogram_die, &tag, &err);
				
				if (tag == DW_TAG_subprogram)
				{
					dwarf_attr(subprogram_die, DW_AT_name, &name_attr, &err);
					dwarf_formstring(name_attr, &name_string, &err);
					
					printf("\t%s(", name_string);

					if (dwarf_child(subprogram_die, &argument_die, &err) == DW_DLV_OK)
					{
						while (1)
						{
							dwarf_tag(argument_die, &arg_tag, &err);
							if (arg_tag == DW_TAG_formal_parameter)
							{
								Dwarf_Attribute name_attr;
								dwarf_attr(argument_die, DW_AT_name, &name_attr, &err);
								dwarf_formstring(name_attr, &name_string, &err);
								
								printf(" %s ", name_string);
							}
							
							if (dwarf_siblingof(dbg, argument_die, &argument_die, &err)!= DW_DLV_OK)
							{
								break;
							}
						}
					}
					
					printf(")\n");
				}
				
				if (dwarf_siblingof(dbg, subprogram_die, &subprogram_die, &err)!= DW_DLV_OK)
				{
					break;
				}
			}
		}
		
		/* move to next compilation unit, if possible */
		dwarf_next_cu_header(dbg, &cu_header_length, &version_stamp, &abbrev_offset, &address_size, &next_cu_header, &err);
		if (dwarf_siblingof(dbg, no_die, &child_die, &err)!= DW_DLV_OK)
		{
			break;
		}
	}
	
	dwarf_finish(dbg, &err);
	close(fd);
}

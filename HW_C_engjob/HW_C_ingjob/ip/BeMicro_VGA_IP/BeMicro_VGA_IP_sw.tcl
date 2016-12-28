# Multi_Timer_driver.tcl
# Create a new driver
create_driver BeMicro_VGA_IP

# Associate it with some hardware known as "altera_avalon_uart"
set_sw_property hw_class_name BeMicro_VGA_IP

# The version of this driver
set_sw_property version 1

# This driver may be incompatible with versions of hardware less
# than specified below. Updates to hardware and device drivers
# rendering the driver incompatible with older versions of
# hardware are noted with this property assignment.
# Multiple-Version compatibility was introduced in version 7.1;
# prior versions are therefore excluded.
set_sw_property min_compatible_hw_version 1.0

# Initialize the driver in alt_sys_init() � not in this case
# set_sw_property auto_initialize true

# Location in generated BSP that above sources will be copied into
set_sw_property bsp_subdirectory drivers

#
# Source file listings...
#

# C/C++ source files � in this case no c files.

# Include files � it is only one � altera_avalon_timer_regs.h�
add_sw_property include_source HAL/inc/BeMicro_VGA_IP_Driver.h

# This driver supports HAL type
add_sw_property supported_bsp_type HAL

# End of file

/*
 * BeMicro_VGA_IP_Driver.h
 *
 *  Created on: 18 aug 2016
 *      Author: Andre
 */

#ifndef BEMICRO_VGA_IP_DRIVER_H_
#define BEMICRO_VGA_IP_DRIVER_H_

//#include <io.h>
//#include <system.h>

#define set_address_pixel(address, color) IOWR_32DIRECT(BEMICRO_VGA_IP_0_BASE, (address)*4, (color))
#define read_address_pixel(address) IORD_32DIRECT(BEMICRO_VGA_IP_0_BASE, (address)*4)

#define set_pixel(x_pos, y_pos, color) IOWR_32DIRECT(BEMICRO_VGA_IP_0_BASE, ((y_pos) * 320 + (x_pos)) * 4, (color))
#define read_pixel(x_pos, y_pos) IORD_32DIRECT(BEMICRO_VGA_IP_0_BASE, ((y_pos) * 320 + (x_pos)) * 4)

#endif /* BEMICRO_VGA_IP_DRIVER_H_ */

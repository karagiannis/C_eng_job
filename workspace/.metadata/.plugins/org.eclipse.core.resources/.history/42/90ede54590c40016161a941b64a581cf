/*
 * print_util.h
 *
 *  Created on: 4 dec 2016
 *      Author: root
 */

#ifndef PRINT_UTIL_H_
#define PRINT_UTIL_H_

#include <stdio.h>
#include <string.h>
#include <system.h>
#include <io.h>
#include <alt_types.h>
#include <stdlib.h>
#include <BeMicro_VGA_IP_Driver.h>
#include "font8x8_basic.h"
typedef alt_u8 pixel_data;

void print_pix(alt_u32 x,alt_u32 y,alt_u32 rgb);
void print_hline(alt_u32 x_start,alt_u32 y_start, alt_u32 len,alt_u32 RGB);
void print_vline(alt_u32 x_start,alt_u32 y_start, alt_u32 len, alt_u32 RGB);
void  print_char(alt_u32 x,alt_u32 y,alt_u32 rgb,alt_u32 BG_RGB,char Character);
void print_str(alt_u32 x_start, alt_u32 y_start,alt_u32 rgb,char *str);
void print_circle(alt_u32 radie, alt_u32 x_centrum, alt_u32 y_centrum, alt_u32 rgb);
void print_empty_circle(alt_u32 radie, alt_u32 x_centrum, alt_u32 y_centrum, alt_u32 rgb);
void print_symmetry_dots_circle(alt_u32 x, alt_u32 y, alt_u32 x_centrum, alt_u32 y_centrum, alt_u32 rgb);
void clear_screen(alt_u32 rgb);
pixel_data read_pixel_ram_int(alt_u32 x_start, alt_u32 y_start);
void print_line(alt_u32 x_start,alt_u32 y_start,alt_u32 x_slut,alt_u32 y_slut);
void print_hArrow(alt_u32 x_start,alt_u32 y_start);
void print_vArrow(alt_u32 x_start,alt_u32 y_start);
void print_coordinate_system(alt_u32 x_origin,alt_u32 y_origin);
void print_welcome_screen();
void print_menu();

#endif /* PRINT_UTIL_H_ */

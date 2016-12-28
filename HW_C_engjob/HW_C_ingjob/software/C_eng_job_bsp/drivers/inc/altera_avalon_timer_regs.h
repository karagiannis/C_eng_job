#ifndef ALTERA_AVALON_TIMER_REGS_H_		//if this file has been read by the linker before, the
#define ALTERA_AVALON_TIMER_REGS_H_		//linker will skip all lines until the next #endif
#include <io.h>			//include HAL macros
#include <system.h>			//include base address defintions
//define device driver macros
#define TIMER_STOP IOWR_32DIRECT(TIMER_HW_IP_0_BASE,4,0x00000000)	// write to control register
#define TIMER_RESET IOWR_32DIRECT(TIMER_HW_IP_0_BASE,4,0x40000000)	// write to control register
#define TIMER_START IOWR_32DIRECT(TIMER_HW_IP_0_BASE,4,0x80000000)	// write to control register
#define TIMER_READ IORD_32DIRECT(TIMER_HW_IP_0_BASE,0)	// read from data register

#endif 	//ALTERA_AVALON_TIMER_REGS_H_
//This ends the #ifndef


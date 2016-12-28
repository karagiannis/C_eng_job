/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

#include <stdio.h>
#include <system.h>
#include <io.h>
#include <alt_types.h>
#include <BeMicro_VGA_IP_Driver.h>

#include <altera_avalon_spi.h>
#include <altera_avalon_spi_regs.h>

#include <altera_avalon_timer_regs.h>

#define WRITE_COMMAND 0x0A 	// Used for acc SPI
#define READ_COMMAND 0x0B	// Used for acc SPI

#define ADC_INIT IOWR_32DIRECT(MODULAR_ADC_0_SEQUENCER_CSR_BASE,0,0x1) // Start continuous sampling
#define ADC_READ_PHOTO IORD_32DIRECT(MODULAR_ADC_0_SAMPLE_STORE_CSR_BASE, 0)  // Read ADC value
#define ADC_READ_TEMP IORD_32DIRECT(MODULAR_ADC_0_SAMPLE_STORE_CSR_BASE, 4)  // Read ADC value

alt_u32 i = 0;
alt_u32 j = 0;

typedef struct mystruct {
		alt_8 x;
		alt_8 y;
		alt_8 z;
} ACCELEROMETER;

ACCELEROMETER accel_data;
alt_u8 spi_command_tx[2] = {0x0B, 0x00}; //, 0x00, 0x00}; // read one register from address 0x00
alt_u8 spi_command_rx[4] = {0xB,0,0,0};

int main()
{
	printf("Test for all components initiate!\n\n");
	for(i = 0; i < 78600; i++)
		set_address_pixel(i, 0);
	for(i = 0; i < 320; i++){
		set_pixel(i,0,7);
		set_pixel(i,239,7);
	}
	for(i = 0; i < 240; i++){
		set_pixel(0,i,7);
		set_pixel(319,i,7);
	}
	for(i = 0; i < 100; i++)
		for(j = 0; j < 100; j++){
			set_pixel(i+1,j+1,1);
			set_pixel(318-i,j+1,2);
			set_pixel(i+1,238-j,4);
		}
	set_pixel(0,0,1);
	set_pixel(1,1,2);
	set_pixel(2,2,4);

	i = read_pixel(0,0);
	printf("value 0,0: %d\t First bit, Blue\n", i);
	i = read_pixel(1,1);
	printf("value 1,1: %d\t Second bit, Green\n", i);
	i = read_pixel(2,2);
	printf("value 2,2: %d\t Third bit, Red\n", i);
	printf("---- VGA test complete ----\n\n");

// ------------------------------------------ Init SPI
	spi_command_tx[0] = WRITE_COMMAND; // write command
	spi_command_tx[1] = 0x2D; // Address 0x2D (go bit location)
	spi_command_tx[2] = 0x02; // go bit value

	alt_avalon_spi_command( ACCELEROMETER_SPI_BASE, 0,
						  3, spi_command_tx,
						  0, spi_command_rx,
						  0);
// ------------------------------------------ Read SPI
	spi_command_tx[0] = READ_COMMAND; // read command
	spi_command_tx[1] = 0x08; // Address of the data registers

	for(i = 0; i < 5; i++)
	{
		alt_avalon_spi_command( ACCELEROMETER_SPI_BASE, 0,
							  2, spi_command_tx,
							  3, &accel_data,
							  0);

		printf("X:%d\t Y:%d\t Z:%d\t\n",accel_data.x, accel_data.y, accel_data.z);
		for(j = 0; j < 200000; j++);
	}
	printf("---- SPI test complete ----\n\n");

	ADC_INIT;
	for(i = 0; i < 5; i++)
	{
		printf("ADC Photo: %d\t", ADC_READ_PHOTO);
		printf("ADC Temp: %d\n", ADC_READ_TEMP);
		for(j = 0; j < 200000; j++);
	}
	printf("---- ADC test complete ----\n\n");

	for(i = 0; i < 5; i++)
	{
		TIMER_RESET;
		TIMER_START;
		while(TIMER_READ < 50000000);
		printf("%d sec\n", i+1);
	}

	printf("---- Timer test complete ----\n\n");

	for(i = 0; i < 8; i++)
	{
		IOWR_32DIRECT(LED_BASE, 0, 1<<i);
		for(j = 0; j < 50000; j++);
	}
	for(i = 0; i < 8; i++)
	{
		IOWR_32DIRECT(LED_BASE, 0, 1<<7-i);
		for(j = 0; j < 50000; j++);
	}
	IOWR_32DIRECT(LED_BASE, 0, 0);
	printf("---- LED test complete ----\n\n");

	for(i = 0; i < 3; i++)
	{
		printf("Press button %d!\n", i+1);
		while(IORD_32DIRECT(KEY_INPUT_BASE,0) & 1<<i);
	}
	printf("---- KEY test complete ----\n\n");

	printf("All tests complete!\n");

	return 0;
}

/*
 * sensor.h
 *
 *  Created on: 28 dec 2016
 *      Author: root
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#define QUEUESIZE 10
#include <system.h>
#include <alt_types.h>
#include <io.h>
#include <altera_avalon_spi.h>
#include <altera_avalon_spi_regs.h>
#include <string.h>
#define WRITE_COMMAND 0x0A 	// Used for acc SPI
#define READ_COMMAND 0x0B	// Used for acc SPI

#define ADC_INIT IOWR_32DIRECT(MODULAR_ADC_0_SEQUENCER_CSR_BASE,0,0x1) // Start continuous sampling
#define ADC_READ_PHOTO IORD_32DIRECT(MODULAR_ADC_0_SAMPLE_STORE_CSR_BASE, 0)  // Read ADC value
#define ADC_READ_TEMP IORD_32DIRECT(MODULAR_ADC_0_SAMPLE_STORE_CSR_BASE, 4)  // Read ADC value




//alt_u32 i = 0;
//alt_u32 j = 0;



typedef struct mystruct {
		alt_8 x;
		alt_8 y;
		alt_8 z;
} ACCELEROMETER;

ACCELEROMETER accel_data;
//alt_u8 spi_command_tx[2] = {0x0B, 0x00}; //, 0x00, 0x00}; // read one register from address 0x00
//alt_u8 spi_command_rx[4] = {0xB,0,0,0};


typedef struct myQUEUE
{
    int items[QUEUESIZE];
    int rindex;
    int windex;
    int numitems;
}QUEUE;

void queue_init(QUEUE *q);
int queue_enqueue(QUEUE *q,int item);
int queue_dequeue(QUEUE *q);
void queue_print(QUEUE *q);



typedef struct SENSOR_CLASS SENSOR_OBJECT;


struct SENSOR_CLASS {
    char description[80];
    alt_u32 x_origo;
    alt_u32 y_origo;
    alt_u32 time_base;
    alt_u32 normalization_factor;
    alt_u32 offset;
    alt_u32 rgb;
    QUEUE queue;
    QUEUE *q;
    void (*configure_time_base)(alt_u32, void*);
    void (*init_measurement)(void*);
    void (*read_sensor)(QUEUE*);
    void (*update_graph)(void*);
    void *this;
    void (*draw_graph)(void*);
};

void config_time_base(alt_u32, SENSOR_OBJECT*);
void init_measurement(SENSOR_OBJECT*);
void read_accelerometerX(QUEUE *);
void update_graph(SENSOR_OBJECT*);
void draw_graph(SENSOR_OBJECT*);
//void read_accelerometerY(QUEUE *q);
//void read_accelerometerZ(QUEUE *q);
void read_temp(QUEUE *q);
void read_light(QUEUE *q);
void queue_print_screen(QUEUE *q, int x_origo, int y_origo, int normalization, int offset, int rgb, SENSOR_OBJECT *sensor_obj);

unsigned int i2bcd(unsigned int i);
void update_time(unsigned int i);

#endif /* SENSOR_H_ */

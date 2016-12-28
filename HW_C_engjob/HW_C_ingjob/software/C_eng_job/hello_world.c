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

//#include "queue.h"
//#include "print_util.h"
#include <string.h>

#define WRITE_COMMAND 0x0A 	// Used for acc SPI
#define READ_COMMAND 0x0B	// Used for acc SPI

#define ADC_INIT IOWR_32DIRECT(MODULAR_ADC_0_SEQUENCER_CSR_BASE,0,0x1) // Start continuous sampling
#define ADC_READ_PHOTO IORD_32DIRECT(MODULAR_ADC_0_SAMPLE_STORE_CSR_BASE, 0)  // Read ADC value
#define ADC_READ_TEMP IORD_32DIRECT(MODULAR_ADC_0_SAMPLE_STORE_CSR_BASE, 4)  // Read ADC value


//#define DEBUG

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

/********************************************************/
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
//void print_line(alt_u32 x_start,alt_u32 y_start,alt_u32 x_slut,alt_u32 y_slut);
void print_welcome_screen();


/************************************************/

#define QUEUESIZE 10

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



/***************************************************/



typedef struct SENSOR_CLASS SENSOR_OBJECT;
enum sampling_state  {five_Hz_sampling, one_Hz_sampling,dot2_Hz_sampling};


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


int main()
{
	print_welcome_screen();
	int sub_seconds = 0;
	enum sampling_state state = one_Hz_sampling;
	QUEUE q1;
	//QUEUE q2;
	//QUEUE q3;
	QUEUE q4;
	QUEUE q5;
	QUEUE* q11 = &q1;
	//QUEUE* q22 = &q2;
	//QUEUE* q33 = &q3;
	QUEUE* q44 = &q4;
	QUEUE* q55 = &q5;



	SENSOR_OBJECT accelorometerX =
		{	"Accelerom. x",
			30, 50,1,1,25,4, q1,q11,
			config_time_base,
			init_measurement,
			read_accelerometerX,
			update_graph,
			&accelorometerX,
			draw_graph
		};

	/*SENSOR_OBJECT accelorometerY =
					{	"Accelerom. y",
						320/3 +30, 50,1,1,20,4, q2,q22,
						config_time_base,
						init_measurement,
						read_accelerometerY,
						update_graph,
						&accelorometerY,
						draw_graph
					};

	SENSOR_OBJECT accelorometerZ =
				{	"Accelerom. z",
					2*320/3 +30, 50,1,1,60,4, q3,q33,
					config_time_base,
					init_measurement,
					read_accelerometerZ,
					update_graph,
					&accelorometerZ,
					draw_graph
				};
*/
	SENSOR_OBJECT temp_sensor =
					{	"Temperature",
						30, 180,1,100,10,4, q4,q44,
						config_time_base,
						init_measurement,
						read_temp,
						update_graph,
						&temp_sensor,
						draw_graph
					};

	SENSOR_OBJECT light_sensor =
						{	"Light",
							320/3 +30, 180,1,100,10,4, q5,q55,
							config_time_base,
							init_measurement,
							read_light,
							update_graph,
							&light_sensor,
							draw_graph
						};

/*SENSOR_OBJECT sensors[5]={accelorometerX,
							accelorometerY,
							accelorometerZ,
							temp_sensor,
							light_sensor};*/

	SENSOR_OBJECT sensors[3]={accelorometerX,
								temp_sensor,
								light_sensor};



		for(i = 0; i < 78600; i++)
			set_address_pixel(i, 0);

		print_hline(0,120,320,6);
		print_vline(320/3,0,240,6);
		print_vline(2*320/3,0,240,6);

		print_str(2*320/3 +5,210-60,5,"1st btn 5Hz");
		print_str(2*320/3 +5,210-30,5,"2nd btn 1Hz");
		print_str(2*320/3 +5,210,5,"3d btn .2Hz");
		print_str(2*320/3 +5,230,5,"L.Karagiannis");


		for(int i = 0; i<3; i++)
			sensors[i].init_measurement(&sensors[i]);

	while(1){


		if(IORD_32DIRECT(KEY_INPUT_BASE,0) == 6)//first button
			state = five_Hz_sampling;
		if(IORD_32DIRECT(KEY_INPUT_BASE,0) == 5)//snd button
			state = one_Hz_sampling;
		if(IORD_32DIRECT(KEY_INPUT_BASE,0) == 3)//third button
					state = dot2_Hz_sampling;

		switch(state){
		case five_Hz_sampling:
#ifdef DEBUG
			alt_printf("5Hz sampling\n");

#endif
			print_str(2*320/3 +5,210-60,2,"1st btn 5Hz");
			print_str(2*320/3 +5,210-30,5,"2nd btn 1Hz");
			print_str(2*320/3 +5,210,5,"3d btn .2Hz");
			TIMER_RESET;
			TIMER_START;
			while(TIMER_READ < 100000);
			sub_seconds++;
			if(sub_seconds == 5){
			update_time(1);
			sub_seconds = 0;
			}

			for(int i = 0; i<3; i++){
				sensors[i].configure_time_base(1,&sensors[i]);
				sensors[i].read_sensor(sensors[i].q);
				sensors[i].update_graph(&sensors[i]);
			}
			break;

		case one_Hz_sampling:
#ifdef DEBUG
			alt_printf("slow\n");

#endif
			print_str(2*320/3 +5,210-60,5,"1st btn 5Hz");
			print_str(2*320/3 +5,210-30,2,"2nd btn 1Hz");
			print_str(2*320/3 +5,210,5,"3d btn .2Hz");
			TIMER_RESET;
			TIMER_START;
				while(TIMER_READ < 50000000);
				update_time(1);
				TIMER_RESET;
				TIMER_START;

			for(int i = 0; i<3; i++){
				sensors[i].configure_time_base(10,&sensors[i]);
				sensors[i].read_sensor(sensors[i].q);
				sensors[i].update_graph(&sensors[i]);
			}
           break;


		case dot2_Hz_sampling:
		#ifdef DEBUG
					alt_printf("0.5 sampling rate\n");

		#endif
					print_str(2*320/3 +5,210-60,5,"1st btn 5Hz");
					print_str(2*320/3 +5,210-30,5,"2nd btn 1Hz");
					print_str(2*320/3 +5,210,2,"3d btn .2Hz");
					for(int i = 0; i < 5; i++){
					TIMER_RESET;
					TIMER_START;

						while(TIMER_READ < 50000000);
						update_time(1);
					}


					for(int i = 0; i<3; i++){
						sensors[i].configure_time_base(10,&sensors[i]);
						sensors[i].read_sensor(sensors[i].q);
						sensors[i].update_graph(&sensors[i]);
					}
		           break;
		}
	}

	return 0;
}

void read_temp(QUEUE *q){
	ADC_INIT;
		alt_u32 temp = ADC_READ_TEMP;
		if(queue_enqueue(q,temp))
;
						else{
							queue_dequeue(q);
							queue_enqueue(q,temp);
						}
		//alt_printf("nu");
#ifdef DEBUG
		 queue_print(q);
#endif
}
void read_light(QUEUE *q){
	ADC_INIT;
			alt_u32 light = ADC_READ_PHOTO;
			if(queue_enqueue(q,light))
;
							else{
								queue_dequeue(q);
								queue_enqueue(q,light);
			//alt_printf("printar nu");
								}
#ifdef DEBUG
		 queue_print(q);
#endif
}
/************************************************/
void config_time_base(alt_u32 time_base, SENSOR_OBJECT* sensor_obj){
	 sensor_obj->time_base = time_base;
}
void init_measurement(SENSOR_OBJECT* sensor_obj){
	queue_init(sensor_obj->q);
	sensor_obj->draw_graph(sensor_obj);
}

void draw_graph(SENSOR_OBJECT* sensor_obj){
	print_char(sensor_obj->x_origo +55,sensor_obj->y_origo -3,7,0,'>');
		print_hline(sensor_obj->x_origo,sensor_obj->y_origo,60,7);

		print_char(sensor_obj->x_origo -4,sensor_obj->y_origo -45,7,0,'^');
		print_vline(sensor_obj->x_origo-1,sensor_obj->y_origo -45,45,7);

		print_str(sensor_obj->x_origo -20, sensor_obj->y_origo +10,7,sensor_obj->description);
}
void update_graph(SENSOR_OBJECT* sensor_obj){
#ifdef DEBUG
	queue_print(sensor_obj->q);
#endif
	queue_print_screen(sensor_obj->q,
			sensor_obj->x_origo,
			sensor_obj->y_origo,
			sensor_obj->normalization_factor,
			sensor_obj->offset,
			sensor_obj->rgb,sensor_obj );
}
void read_accelerometerX(QUEUE *q)
{
	ACCELEROMETER accel_data;
	alt_u8 spi_command_tx[2] = {0x0B, 0x00}; //, 0x00, 0x00}; // read one register from address 0x00
	alt_u8 spi_command_rx[4] = {0xB,0,0,0};
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


			alt_avalon_spi_command( ACCELEROMETER_SPI_BASE, 0,
								  2, spi_command_tx,
								  3, &accel_data,
								  0);
#ifdef DEBUG
			alt_printf("X:%d\t Y:%d\t Z:%d\t\n",accel_data.x, accel_data.y, accel_data.z);
#endif

		alt_32 accel_x = 0;
		accel_x = (alt_32)accel_data.x;

		if(queue_enqueue(q,accel_x))
;
				else{
					queue_dequeue(q);
					queue_enqueue(q,accel_x);
				}
}
/*
void read_accelerometerY(QUEUE *q)
{
	ACCELEROMETER accel_data;
	alt_u8 spi_command_tx[2] = {0x0B, 0x00}; //, 0x00, 0x00}; // read one register from address 0x00
	alt_u8 spi_command_rx[4] = {0xB,0,0,0};
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


			alt_avalon_spi_command( ACCELEROMETER_SPI_BASE, 0,
								  2, spi_command_tx,
								  3, &accel_data,
								  0);
#ifdef DEBUG
			alt_printf("X:%d\t Y:%d\t Z:%d\t\n",accel_data.x, accel_data.y, accel_data.z);
#endif

		alt_32 accel_y = 0;
		accel_y = (alt_32)accel_data.y;

		if(queue_enqueue(q,accel_y))
						;
				else{
					queue_dequeue(q);
					queue_enqueue(q,accel_y);
				}
}

void read_accelerometerZ(QUEUE *q)
{
	ACCELEROMETER accel_data;
	alt_u8 spi_command_tx[2] = {0x0B, 0x00}; //, 0x00, 0x00}; // read one register from address 0x00
	alt_u8 spi_command_rx[4] = {0xB,0,0,0};
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


			alt_avalon_spi_command( ACCELEROMETER_SPI_BASE, 0,
								  2, spi_command_tx,
								  3, &accel_data,
								  0);
#ifdef DEBUG
			alt_printf("X:%d\t Y:%d\t Z:%d\t\n",accel_data.x, accel_data.y, accel_data.z);
#endif

		alt_32 accel_z = 0;
		accel_z = (alt_32)accel_data.z;

		if(queue_enqueue(q,accel_z))
						;
				else{
					queue_dequeue(q);
					queue_enqueue(q,accel_z);
				}
}

*/
/*************************************************/

void print_pix(alt_u32 x,alt_u32 y,alt_u32 rgb)
{
	if(rgb <= 7 && rgb >= 0)
		set_pixel(x,y,rgb);
	else
		set_pixel(x,y,0);
}
void print_hline(alt_u32 x_start,alt_u32 y_start, alt_u32 len,alt_u32 RGB){



	if(x_start + len <= 320){
		for(int i = 0; i < len; i++){
			print_pix(x_start + i,y_start, RGB);
		}
	}
	else{

		for(int i = 0; i < (320-x_start); i++){
					print_pix(x_start + i,y_start, RGB);
		}
	}

}
void print_vline(alt_u32 x_start,alt_u32 y_start, alt_u32 len, alt_u32 RGB)
{
	if(y_start + len <= 240){
			for(int i = 0; i < len; i++){
				print_pix(x_start,y_start + i, RGB);
			}
		}
		else{

			for(int i = 0; i < (240-y_start); i++){
						print_pix(x_start,y_start +i , RGB);
			}
		}
}

void  print_char(alt_u32 x,alt_u32 y,alt_u32 rgb,alt_u32 BG_RGB,char Character)
{
	alt_u8 temp;
	int row;

	alt_u8  token = (alt_8)Character;
	for (row = 0; row <8; row++)
	{
			temp =  (alt_8)font8x8_basic[token][row];

			for (int pos = 0 ; pos != 8 ; pos++) {
			    if (temp & (1 << pos)) {
			        // Current bit is set to 1
			    	print_pix(x+pos,y+row,rgb);
			    } else {
			        // Current bit is set to 0
			    	print_pix(x+pos,y+row,BG_RGB);
			    }
			}
	}
}
void print_str(alt_u32 x_start, alt_u32 y_start,alt_u32 rgb,char *str)
{
	alt_u16 len = (alt_u16) strlen(str);
	alt_u16 i;
	for(i=0; i< len; i++){
		print_char(x_start + i*8,y_start,rgb,0,str[i]);
	}
}

void print_symmetry_dots_circle(alt_u32 x, alt_u32 y, alt_u32 x_centrum, alt_u32 y_centrum, alt_u32 rgb)
{
		print_pix(x+x_centrum,y + y_centrum,rgb);
		print_pix(x+x_centrum,-y + y_centrum,rgb);
		print_pix(-x+x_centrum,y + y_centrum,rgb);
		print_pix(-x+x_centrum,-y + y_centrum,rgb);

		print_pix(y + x_centrum,x+y_centrum,rgb);
		print_pix(-y + x_centrum,x+y_centrum,rgb);
		print_pix(y + x_centrum,-x+y_centrum,rgb);
		print_pix(-y + x_centrum,-x+y_centrum,rgb);

}
void print_empty_circle(alt_u32 radie, alt_u32 x_centrum, alt_u32 y_centrum, alt_u32 rgb)
{
		//unsigned int value;
		//unsigned int radius = radie;
		int x = 0;
		int y = radie;

		print_symmetry_dots_circle(x,y,x_centrum,y_centrum,rgb);
		int h = 1- radie;
		while(y > x){
			if (h < 0){//east point

				print_symmetry_dots_circle(x+1,y,x_centrum,y_centrum,rgb);
				h = h +2*x +3;
				x= x+1;

			}
			else{
				print_symmetry_dots_circle(x+1,y-1,x_centrum,y_centrum,rgb);
				h= h+2*(x-y)+5;
				y=y-1;
				x=x+1;
			}

		}
}
void print_circle(alt_u32 radie, alt_u32 x_centrum, alt_u32 y_centrum, alt_u32 rgb)
{
	unsigned int radius = radie;

	for(radius = radie; radius > 0; radius--)
		print_empty_circle(radius,x_centrum, y_centrum, rgb);
}
void clear_screen(alt_u32 rgb){
	for(int y = 0 ;y <240;y++)
		for(int x = 0; x < 320; x++)
			print_pix(x,y,rgb);
}

pixel_data read_pixel_ram_int(alt_u32 x_start, alt_u32 y_start)
{
	alt_u32 i = read_pixel(x_start, y_start);
	return (alt_u8) i;
}



void print_welcome_screen(){
	clear_screen(0);

	print_circle(10,  320/2, 240/2, 3);
	print_str(50,50,2,"Welcome to measurement station!");
	print_str(50,150,2,"Press any key to continue");
	alt_u32 key = 0;
	key = IORD_32DIRECT(KEY_INPUT_BASE,0);
	//alt_printf("key = %d", key);
	while(key == 7)
		key = IORD_32DIRECT(KEY_INPUT_BASE,0);




}

/***************************************************/

void queue_init(QUEUE *q)
{
    memset(q,0,sizeof(QUEUE));
    q->rindex = 0;//Probably un-necessay to do this, because of the above line
    q->windex = 0;
    q->numitems = 0;
}

int queue_enqueue(QUEUE *q,int item)
{
    int num_items = q->numitems;
    //int* temp_array;
    int temp_array[QUEUESIZE];
    int i;

    if(num_items < QUEUESIZE)           //Check to see if the queue is not filled
    {
        //temp_array = (int *)calloc(num_items,sizeof(int));//Allocate temp array with size equal to the content size of the queue
        for(i = 0; i < num_items; i++)
            temp_array[i]= q->items[i];       //Copy the queue to temp array

        q->items[0] = item;                    //Insert the new item in the queue, always at index 0

        for ( i = 1; i < num_items +1; i++)     //Copy back the items from temp array
            q->items[i] = temp_array[i-1];
        //free(temp_array);                             //Dispose the temp array
        q->numitems++;                          //increment the number of items
        q->rindex++;                            //Increment the queue pointer so it points to the next free position in the queue
        return 1;                               //The enqueue succeeded
    }
    else return 0;          //The queue is filled, cannot insert more

}

int queue_dequeue(QUEUE *q)
{
    int num_items = q->numitems;

    if (! num_items == 0)// The queue contains items
    {
        q->rindex--; //Decrement the end pointer because it points to a at the first empty position in the FIFO
         q->numitems--;                 //Decrement the number of items in the queue
         return 1;                      //Return success because the dequeing succeeded/ the queue was not empty
    }
    else return 0;          //The queue is empty, nothing can be dequeued


}


void queue_print(QUEUE *q)
{
    int i = 0;
    for (i = q->numitems-1; i>= 0; i--){  //print in FIFO order, oldest first
#ifdef DEBUG
        alt_printf("%d\n", q->items[i] );
#endif
    }
}
void queue_print_screen(QUEUE *q, int x_origo, int y_origo, int normalization, int offset, int rgb, SENSOR_OBJECT *sensor_obj){
	 int i = 0;
	 int j;
	 int mean = 0;
	 int value;
	    for (i = q->numitems-1; i>= 0; i--){  //print in FIFO order, oldest first
#ifdef DEBUG
	        alt_printf("Un-normalized %d\n normalized %d\n", q->items[i],q->items[i]/normalization );
#endif
	        j = q->numitems-1 -i;
	        mean = mean + q->items[i];
#ifdef DEBUG
	        alt_printf("j%d\n",j);
#endif

	        for(int k = 1; k < 55;k++)
	        	print_pix(x_origo + j, y_origo-k,0);//Blank out previous measurement
	        print_pix(x_origo + j, y_origo,7);//Fix ccordinate system line
	        sensor_obj->draw_graph(sensor_obj);
	        value = offset + q->items[i]/normalization;
	        if(offset + q->items[i]/normalization > 40)
	        	value = 40;
	        if (offset + q->items[i]/normalization < 0)
	          value = 0;

	        print_pix(x_origo + j,y_origo-value,rgb);
	        int  bcd = i2bcd(abs(q->items[0]));
	        if(q->items[0] & 0x80000000)//negative
	        	print_char(x_origo +24, y_origo+30,4,0,'-');

	        	char c = (char)((bcd & 0xF000) >>12);
	        	short s = (short) c +48;
	        	print_char(x_origo +30, y_origo+30,4,0,(char)s);

	        	c = (char)((bcd & 0x0F00) >>8);
	        	s = (short) c +48;
	        	print_char(x_origo +38, y_origo+30,4,0,(char)s);

	        	c = (char)((bcd & 0x0F0) >>4);
	        	s = (short) c +48;
	        	print_char(x_origo +46, y_origo+30,4,0,(char)s);

	        	c = (char)(bcd & 0x0F);
	        	s = (short) c +48;
	        	print_char(x_origo +54, y_origo+30,4,0,(char)s);


	    }
	    mean = mean/q->numitems;
	    int offs = 25 - mean/normalization;
	    sensor_obj->offset = offs;
}

unsigned int i2bcd(unsigned int i) {
    unsigned int binaryShift = 1;
    unsigned int digit;
    unsigned int bcd = 0;
    //alt_printf("%d\n",i);
    while (i > 0) {
        digit = i % 10;
        bcd += (digit << binaryShift);
        binaryShift += 4;
        i /= 10;
    }
    bcd = bcd >> 1;
#ifdef DEBUG
    unsigned int digit,n,mask;
    for (n = 0,mask = 0x80000000;mask != 0;mask>>= 1){
            if((n==4) || (n==8)|| (n == 12)|| n== 16 ||n==20|| n==24 ||n==28)
                putchar(' ');
            putchar((bcd & mask)? '1':'0');
            n++;
        }
    putchar('\n');
#endif

    return bcd;
}

void update_time(unsigned int i){

	static int hours = 0;
	static int minutes = 0;
	static int seconds = 0;
	//static int sub_seconds = 0;
	unsigned int bcd = 0;
	//unsigned int time[3]={hours, minutes,seconds};


		seconds++;


	if(seconds == 60)
	{
		seconds = 0;
		minutes++;
		if (minutes == 60)
		{
			minutes = 0;
			hours++;
		}
	}

	char c;
	short s;
	bcd = i2bcd(hours);
	c = (char)((bcd & 0x0F0) >>4);
	s = (short) c +48;
	print_char(2*320/3-50 +30 +46, 130,4,0,(char)s);

	c = (char)(bcd & 0x0F);
	s = (short) c +48;
	print_char(2*320/3 -50+30 +54,130,4,0,(char)s);

	print_char(2*320/3-50 +30 +54+8,130,4,0,':');

	bcd = i2bcd(minutes);
	c = (char)((bcd & 0x0F0) >>4);
	s = (short) c +48;
	print_char(2*320/3-50 +30 +54+8+8, 130,4,0,(char)s);

	c = (char)(bcd & 0x0F);
	s = (short) c +48;
	print_char(2*320/3-50 +30 +54+8+8+8,130,4,0,(char)s);

	print_char(2*320/3-50 +30 +54+8+8+8+8,130,4,0,':');


	bcd = i2bcd(seconds);
		c = (char)((bcd & 0x0F0) >>4);
		s = (short) c +48;
		print_char(2*320/3-50 +30 +54+40, 130,4,0,(char)s);

		c = (char)(bcd & 0x0F);
		s = (short) c +48;
		print_char(2*320/3-50 +30 +54+48,130,4,0,(char)s);
}

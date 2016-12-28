/*
 * queue.c
 *
 *  Created on: 4 dec 2016
 *      Author: root
 */

/*============================================================================
Name:          queue.c
Author:        Lasse Karagiannis
Compile with:  Code::Blocks 13.12 / MinGW 0.6.2
Date:          2015-12-15
Description:   Uppgift 5-x To file implements queue.h
======================================================================================*/





#include "queue.h"
#include <stdlib.h>

/*
typedef struct
{
    int items[QUEUESIZE];
    int rindex;
    int windex;
    int numitems;
} QUEUE;
*/

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
    int* temp_array;
    int i;

    if(num_items < QUEUESIZE)           //Check to see if the queue is not filled
    {
        temp_array = (int *)calloc(num_items,sizeof(int));//Allocate temp array with size equal to the content size of the queue
        for(i = 0; i < num_items; i++)
            temp_array[i]= q->items[i];       //Copy the queue to temp array

        q->items[0] = item;                    //Insert the new item in the queue, always at index 0

        for ( i = 1; i < num_items +1; i++)     //Copy back the items from temp array
            q->items[i] = temp_array[i-1];
        free(temp_array);                             //Dispose the temp array
        q->numitems++;                          //increment the number of items
        q->rindex++;                            //Increment the queue pointer so it points to the next free position in the queue
        return 1;                               //The enqueue succeeded
    }
    else return 0;          //The queue is filled, cannot insert more

}

int queue_dequeue(QUEUE *q,int *item)
{
    int num_items = q->numitems;

    if (! num_items == 0)// The queue contains items
    {
        *item = q->items[-- q->rindex]; //Decrement the end pointer because it points to a at the first empty position in the FIFO
         q->numitems--;                 //Decrement the number of items in the queue
         return 1;                      //Return success because the dequeing succeeded/ the queue was not empty
    }
    else return 0;          //The queue is empty, nothing can be dequeued


}

void queue_print(QUEUE *q)
{
    int i = 0;
    for (i = q->numitems-1; i>= 0; i--){  //print in FIFO order, oldest first
        printf("%d", q->items[i] );
    }
    printf("\n");
}
void queue_print_screen(QUEUE *q, int x_origo, int y_origo, int normalization){
	 int i = 0;
	    for (i = q->numitems-1; i>= 0; i--){  //print in FIFO order, oldest first
	        printf("%d", q->items[i] );
	    }
}


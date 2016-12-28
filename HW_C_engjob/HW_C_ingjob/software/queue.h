/*
 * queue.h
 *
 *  Created on: 4 dec 2016
 *      Author: root
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#define QUEUESIZE 100
#include <stdio.h>
#include <string.h>

typedef struct
{
    int items[QUEUESIZE];
    int rindex;
    int windex;
    int numitems;
} QUEUE;

void queue_init(QUEUE *q);
int queue_enqueue(QUEUE *q,int item);
int queue_dequeue(QUEUE *q,int *item);
void queue_print(QUEUE *q);
void queue_print_screen(QUEUE *q, int x_origo, int y_origo, int normalization);

#endif /* QUEUE_H_ */

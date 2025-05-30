#ifndef queue_H 
#define queue_H

#include "boolean.h"
#include "list1.h"  

#define NBElement 10  

typedef struct {
    NodeBus* Front;
} Queue;

typedef Queue QueueArray[NBElement];

void CreateQueue(Queue *Q);
boolean is_Empty(Queue Q);
void EnQueue(Queue *Q, DataBus X);
void DeQueue(Queue *Q, DataBus *X);
void PrintQueue(Queue Q);

#endif

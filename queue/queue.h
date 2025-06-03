#ifndef QUEUE_H
#define QUEUE_H

#include "boolean.h"
#include "list1.h"
#include "bus.h"
#define Front(Q) (Q).Front
#define Rear(Q) (Q).Rear

#define NBElement 10  // hanya relevan jika kamu pakai queue array

typedef struct {
    NodeBus* Front;
    NodeBus* Rear;
} Queue;

void CreateQueue(Queue *Q);
boolean is_Empty(Queue Q);
void EnQueue(Queue *Q, DataBus X);
void DeQueue(Queue *Q, DataBus *X);
void PrintQueue(Queue Q);

#endif

#ifndef QUEUE_H
#define QUEUE_H

#include "boolean.h"
#include "list1.h"
#include "bus.h"
#define Front(Q) (Q).Front
#define Rear(Q) (Q).Rear

#define NBElement 10  // hanya relevan jika kamu pakai queue array

#define Nil NULL
#define Info(P) (P)->info
#define Next(P) (P)->Next
#define First(Q) (Q).First
#define Rear(Q) (Q).Rear
#define NBElement 10

// Queue structure
typedef struct {
    NodeBus* Front;
    NodeBus* Rear;
} Queue;

void CreateQueue(Queue *Q);
boolean is_Empty(Queue Q);
boolean is_Full(Queue Q);
void EnQueue(Queue *Q, infotype X);
void deQueue(Queue *Q, infotype *X);
address Alokasi(infotype X);
void DeAlokasi(address P);
void InsertLast(Queue *Q, address P);
void DelFirst(Queue *Q, address *P);

#endif


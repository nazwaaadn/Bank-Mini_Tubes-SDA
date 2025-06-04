#ifndef QUEUE_H
#define QUEUE_H

#include "boolean.h"
#include "bus.h"

#define Front(Q) (Q).Front
#define Rear(Q) (Q).Rear
#define Next(P) (P)->next
#define Info(P) (P)->Info

#define NBElement 10  

// Queue structure
typedef struct {
    NodeBus* Front;
    NodeBus* Rear;
} Queue;

/* ===== Prototype Fungsi Queue ===== */
void CreateQueue(Queue *Q);
boolean is_Empty(Queue Q);
boolean is_Full(Queue Q);
void EnQueue(Queue *Q, DataBus X);
void deQueue(Queue *Q, DataBus *X);

/* ===== Fungsi Bantu Internal ===== */
void Alokasi(DataBus X, NodeBus **P);
void DeAlokasi(NodeBus *P);
void InsertLast(Queue *Q, NodeBus *P);
void DelFirst(Queue *Q, NodeBus **P); // Tambahkan ini karena dipakai di deQueue()

#endif

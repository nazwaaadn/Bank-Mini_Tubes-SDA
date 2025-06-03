#ifndef queue_H 
#define queue_H

#include "BOOLEAN.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bus.h"

#define NBElement 10  

#define Nil NULL
#define Info(P) (P)->info
#define Next(P) (P)->Next
#define First(Q) (Q).First
#define Rear(Q) (Q).Rear
#define NBElement 10

// Type for queue info
typedef char infotype[50];

// Node definition for queue
typedef struct tElmtQueue* address;
typedef struct tElmtQueue {
    infotype info;
    address Next;
} ElmtQueue;

// Queue structure
typedef struct {
    address First;
    address Rear;
} Queue;

// ========== Queue-related Function Prototypes ==========
void CreateQueue(Queue *Q);
boolean is_Empty(Queue Q);
boolean is_Full(Queue Q);
void EnQueue(Queue *Q, infotype X);
void deQueue(Queue *Q, infotype *X);
address Alokasi(infotype X);
void DeAlokasi(address P);
void InsertLast(Queue *Q, address P);
void DelFirst(Queue *Q, address *P);

// ========== (Optional) NodeBus list-related ==========
boolean ListEmpty(NodeBus *L);
void CreateList(NodeBus **L);
NodeBus* AlokasiBus(DataBus X);
void DeAlokasiBus(NodeBus *P);
NodeBus* Search(NodeBus *L, char id[]);
void InsVFirst(NodeBus **L, DataBus X);
void InsertFirst(NodeBus **L, NodeBus *P);
void DelP(NodeBus **L, char id[]);
void PrintInfo(NodeBus *L);

#endif


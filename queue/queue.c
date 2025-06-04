/*File body untuk ADT Queue*/
//Dibuat tanggal 28-3-2013

#include "queue.h"
#include <stdio.h>
#include "bus.h"

NodeBus* AlokasiQueue(DataBus X, NodeBus **P)
{
    NodeBus *P = (NodeBus *)malloc(sizeof(NodeBus));
    if (*P != NULL) {
        Info(*P) = X;
        Next(*P) = Nil;
    } else {
        printf("Alokasi Queue gagal\n");
    }
    return P;
}

void DeAlokasiQueue(NodeBus *P)
{
    free(P);
}

void InsertLast(Queue *Q, NodeBus *P)
{
    if (is_Empty(*Q))
    {
        Front(*Q) = P;
        Rear(*Q) = P;
    } else {
        Next(Rear(*Q)) = P; 
        Rear(*Q) = P;      
    }
    
}

void DelFirst(Queue *Q, NodeBus **P)
{
    *P = Front(*Q);
    if (Front(*Q) == Rear(*Q)) {
        Front(*Q) = Nil;
        Rear(*Q) = Nil;
    } else {
        Front(*Q) = Next(*P);
    }
    Next(*P) = Nil;
}

void CreateQueue (Queue *Q)
{
	Front(*Q) = Nil;
    Rear(*Q) = Nil;
}

/*Memeriksa apakah queue kosong */
boolean is_Empty (Queue Q)
{
	 return (Front(Q) == Nil && Rear(Q) == Nil);
}

/*Memeriksa apakah queue penuh */
boolean is_Full(Queue Q) {
    NodeBus* P = Front(Q);
    int count = 0;

    while (P != Nil && count < NBElement) {
        count++;
        P = Next(P);
    }

    return count == 10; // Jika elemen sudah mencapai 10, return true
}

void EnQueue(Queue *Q, DataBus X) {
    NodeBus *P;
    P = AlokasiQueue(X, &P);
    if(!is_Full(*Q)) {
            if (is_Empty(*Q)) {
                Front(*Q) = P;
                Rear(*Q) = P;
            } else {
                InsertLast(Q, P);   
            }
    }
}

/* Menghapus elemen dari depan queue */
void deQueue(Queue *Q, DataBus *X) {
    if (is_Empty(*Q)) {
        printf("Queue kosong\n");
        return;
    }

    NodeBus *P;
    DelFirst(Q, &P);  // Memanggil prosedur DelFirst
    *X = Info(P); 
    DeAlokasiQueue(P);
}


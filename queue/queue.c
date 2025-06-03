/*File body untuk ADT Queue*/
//Dibuat tanggal 28-3-2013

#include "queue.h"
#include <stdio.h>
#include "linked.h"

/*Membuat sebuah queue baru dengan First dan Rear NULL*/
void CreateQueue (Queue *Q)
{
	First(*Q) = Nil;
    Rear(*Q) = Nil;
}

/*Memeriksa apakah queue kosong */
boolean is_Empty (Queue Q)
{
	 return (First(Q) == Nil && Rear(Q) == Nil);
}

/*Memeriksa apakah queue penuh */
boolean is_Full(Queue Q) {
    address P = First(Q);
    int count = 0;

    while (P != Nil && count < NBElement) {
        count++;
        P = Next(P);
    }

    return count == 10; // Jika elemen sudah mencapai 10, return true
}

void EnQueue(Queue *Q, DataBus X) {
    address P;
    P = Alokasi(X);
    if(!is_Full(*Q)) {
            if (is_Empty(*Q)) {
                First(*Q) = P;
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

    address P;
    DelFirst(Q, &P);  // Memanggil prosedur DelFirst
    *X = Info(P); 
    DeAlokasi(P);
}


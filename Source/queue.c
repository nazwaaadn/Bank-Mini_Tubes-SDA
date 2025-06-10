#include "queue.h"
#include "admin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

NodeBus* AlokasiQueue(DataBus X) {
    NodeBus *P = (NodeBus *)malloc(sizeof(NodeBus));
    if (P != NULL) {
        Info(P) = X;
        Next(P) = NULL;
    } else {
        printf("Alokasi Queue gagal\n");
    }
    return P;
}

void DeAlokasiQueue(NodeBus *P) {
    free(P);
}

void InsertLast(Queue *Q, NodeBus *P) {
    if (is_Empty(*Q)) {
        Front(*Q) = P;
        Rear(*Q) = P;
    } else {
        Next(Rear(*Q)) = P; 
        Rear(*Q) = P;      
    }
}

void DelFirst(Queue *Q, NodeBus **P) {
    *P = Front(*Q);
    if (Front(*Q) == Rear(*Q)) {
        Front(*Q) = NULL;
        Rear(*Q) = NULL;
    } else {
        Front(*Q) = Next(*P);
    }
    Next(*P) = NULL;
}

void CreateQueue(Queue *Q) {
    Front(*Q) = NULL;
    Rear(*Q) = NULL;
}

boolean is_Empty(Queue Q) {
    return (Front(Q) == NULL && Rear(Q) == NULL);
}

boolean is_Full(Queue Q) {
    NodeBus* P = Front(Q);
    int count = 0;
    while (P != NULL && count < NBElement) {
        count++;
        P = Next(P);
    }
    return count == NBElement;
}

void EnQueue(Queue *Q, DataBus X) {
    if(is_Full(*Q)) {
        printf("Queue penuh, tidak bisa menambah bus!\n");
        return;
    }
    NodeBus *P = AlokasiQueue(X);
    if(P == NULL) {
        printf("Gagal alokasi memori!\n");
        return;
    }
    InsertLast(Q, P);
}

void deQueue(Queue *Q, DataBus *X) {
    if (is_Empty(*Q)) {
        printf("Queue kosong\n");
        return;
    }
    NodeBus *P;
    DelFirst(Q, &P);
    *X = Info(P); 
    DeAlokasiQueue(P);
}

void AntriBus(Queue *Q) {
    printf("\n=== Daftar Bus yang Tersedia untuk Antri ===\n");
    printAllBus();

    char idBus[20];
    printf("Masukkan ID Bus yang ingin diantrikan: ");
    fgets(idBus, sizeof(idBus), stdin);
    idBus[strcspn(idBus, "\n")] = '\0';
    // TRIM depan & belakang (gunakan utility trim yang sudah dijelaskan sebelumnya)
    trim(idBus);


    // Opsional: Hapus spasi depan & belakang juga
    int start = 0;
    while (isspace((unsigned char)idBus[start])) start++;
    if (start > 0) memmove(idBus, idBus + start, strlen(idBus + start) + 1);
    for (int i = strlen(idBus) - 1; i >= 0 && isspace((unsigned char)idBus[i]); --i)
        idBus[i] = '\0';

    NodeBus* busNode = searchBusByID(idBus);
    if (busNode == NULL) {
        printf("ID Bus tidak ditemukan!\n");
        return;
    }

    if (is_Full(*Q)) {
        printf("Antrian penuh! Tidak bisa menambah bus lagi.\n");
        free(busNode);
        return;
    }

    EnQueue(Q, busNode->Info);
    updateBusStatus(busNode->Info.idBus, 0);
    printf("Bus %s berhasil masuk ke antrian.\n", busNode->Info.idBus);
    free(busNode);
}

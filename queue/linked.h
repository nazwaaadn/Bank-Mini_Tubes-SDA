#ifndef linked_H
#define linked_H
#include "BOOLEAN.h"
#include <stdio.h>
#include <time.h>
#include "bus.h"
#define Nil NULL
#define Info(P) (P)->info
#define Next(P) (P)->Next
#define First(L) (L).First

boolean ListEmpty(NodeBus *L);
void CreateList(NodeBus **L);
NodeBus* Alokasi(DataBus X);
void DeAlokasi(NodeBus *P);
NodeBus* Search(NodeBus *L, char id[]);
void InsVFirst(NodeBus **L, DataBus X);
void InsertFirst(NodeBus **L, NodeBus *P);
void DelP(NodeBus **L, char id[]);
void PrintInfo(NodeBus *L);

#endif


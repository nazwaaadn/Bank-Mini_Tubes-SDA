#ifndef bus_H
#define bus_H
#include "BOOLEAN.h"
#include <stdio.h>
#include <time.h>
#define Nil NULL
#define Info(P) (P)->Info
#define Next(P) (P)->next

//Bus
typedef struct
{
    char idBus[10];               
    char platNomor[15];           
    char namaSupir[50];           
    char rute[100];               
    time_t keberangkatan;       
    time_t kedatangan;          
    int kapasitas;
	char kelas;
} DataBus;

typedef struct NodeBus
{
    DataBus Info;
    struct NodeBus *next;
} NodeBus;

extern NodeBus *HeadBus;

int isBusListEmpty();
void insertBus(DataBus busBaru);
void editBus(char idBus[], DataBus busBaru);
void printAllBus();
NodeBus* searchBusByID(char idBus[]);
int deleteBus(char idBus[]);

#endif
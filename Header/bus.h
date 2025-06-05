#ifndef BUS_H
#define BUS_H

#include "BOOLEAN.h"
#include "terminal.h"
#include <stdio.h>
#include <time.h>


#define Nil NULL
#define Info(P) (P)->Info
#define Next(P) (P)->next


// Struktur data utama untuk satu bus
typedef struct {
    char idBus[10];
    char platNomor[15];
    char namaSupir[50];
    char rute[100]; // Rute bus dalam format string
    time_t keberangkatan;
    time_t kedatangan;
    int kapasitas;
    char kelas;
} DataBus;

// Node dalam linked list daftar bus
typedef struct NodeBus {
    DataBus Info;
    struct NodeBus *next;
} NodeBus;

// Head pointer untuk list bus
extern NodeBus *HeadBus;

// Fungsi-fungsi manajemen bus
boolean isBusListEmpty();
NodeBus* alokasiNodeBus(DataBus busData);
void freeBus(NodeBus* busNode);
NodeBus* inputDataBus(terminalTree T, int root);
void insertBus(NodeBus *newNode);
void insertRute(NodeBus *busNode, char *rute);
void editRute(char idBus[], DataBus *busBaru);
void inputDataBusBaru(DataBus *busBaru);
void hapusNewline(char *str);
void editBus(char idBus[]);
void printAllBus(); 
NodeBus* searchBusByID(char idBus[]);
void deleteBus(char idBus[]);
void saveAllBusToFile();

#endif

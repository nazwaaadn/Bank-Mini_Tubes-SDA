#ifndef BUS_H
#define BUS_H

#include "BOOLEAN.h"
#include <stdio.h>
#include <time.h>

#define Nil NULL
#define Info(P) (P)->Info
#define Next(P) (P)->next

// Forward declaration untuk node dalam tree rute
typedef struct NodeRute NodeRute;

// Linked list untuk menyimpan rute dari bus
typedef struct RuteBus {
    NodeRute* halte;           // pointer ke node halte di tree
    struct RuteBus* next;
} RuteBus;

// Struktur data utama untuk satu bus
typedef struct {
    char idBus[10];
    char platNomor[15];
    char namaSupir[50];
    RuteBus* rute;             // pointer ke linked list rute
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
RuteBus* alokasiRute(NodeRute* halte);
void freeRute(RuteBus* rute);
void freeBus(NodeBus* busNode);
void inputDataBus(DataBus *busBaru);
void insertBus(DataBus busBaru);
void insertRute(NodeBus* bus, NodeRute* halte);
void editBus(char idBus[], DataBus busBaru);
void printAllBus(); 
NodeBus* searchBusByID(char idBus[]);
void deleteBus(char idBus[]);

#endif

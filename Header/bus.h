#ifndef BUS_H
#define BUS_H

#include "BOOLEAN.h"
#include "terminal.h"
#include <stdio.h>
#include <time.h>

#define Nil NULL
#define Info(P) (P)->Info
#define Next(P) (P)->next

typedef struct NodeRute {
    char namaTerminal[50];
    time_t waktubrgkt; 
    struct NodeRute* next;
} NodeRute;

// Struktur data utama untuk satu bus
typedef struct {
    char idBus[10];
    char platNomor[15];
    char namaSupir[50];
    NodeRute *rute; 
    time_t keberangkatan;
    time_t kedatangan;
    int kapasitas;
    char kelas;
    boolean status; // 0 untuk aktif, 1 untuk tidak aktif   
} DataBus;

// Node dalam linked list daftar bus
typedef struct NodeBus {
    DataBus Info;
    struct NodeBus *next;
} NodeBus;

extern NodeBus *HeadBus;
extern NodeRute *HeadRute;

// Fungsi-fungsi manajemen bus
boolean isBusListEmpty();
NodeBus* alokasiNodeBus(DataBus busData);
void freeBus(NodeBus* busNode);
NodeBus* inputDataBus(terminalTree T, int root);
void tambahRute(NodeRute** head, const char* nama);
void hapusRute(NodeRute** head);
void deleteBus(char idBus[]);
void ruteToString(NodeRute* rute, char* buffer, size_t bufferSize);
NodeRute* strToRute(char* rute);
void printAllBus(); 
boolean PreOrderToLinkedList(terminalTree T, address idx, char* tujuan, NodeRute** headRute, time_t waktuTu);
boolean PreOrder(terminalTree P, address idx, char* tujuan);

void trim(char* str);

#endif

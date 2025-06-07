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
} DataBus;

// Node dalam linked list daftar bus
typedef struct NodeBus {
    DataBus Info;
    struct NodeBus *next;
} NodeBus;


// Head pointer untuk list bus
extern NodeBus *HeadBus;
extern NodeRute *HeadRute;

// Fungsi-fungsi manajemen bus
boolean isBusListEmpty();
NodeBus* alokasiNodeBus(DataBus busData);
void freeBus(NodeBus* busNode);
NodeBus* inputDataBus(terminalTree T, int root);
void tambahRute(NodeRute** head, const char* nama);
void editRute(char idBus[], DataBus *busBaru);
void hapusRute(NodeRute** head);
void inputDataBusBaru(DataBus *busBaru);
void hapusNewline(char *str);
void editBus(char idBus[]);
void printAllBus(); 
NodeBus* searchBusByID(char idBus[]);
void deleteBus(char idBus[]);
void saveSingleBusToFile(DataBus bus);
boolean PreOrderToLinkedList(terminalTree T, address idx, char* tujuan, NodeRute** headRute, time_t waktuTu);
void ruteToString(NodeRute* rute, char* buffer, size_t bufferSize);
NodeRute* stringToRute(char* rute); // Fungsi baru untuk konversi string ke linked list rute
void loadAllBusFromFile(); // Fungsi untuk memuat semua bus dari file
void bersihkanDataBus(); // Fungsi untuk membersihkan data bus
boolean PreOrder(terminalTree P, address idx, char* tujuan);

#endif

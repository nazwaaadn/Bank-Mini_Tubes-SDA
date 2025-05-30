#ifndef tiket_H
#define tiket_H
#include "BOOLEAN.h"
#include <stdio.h>
#include <time.h>
#define Nil NULL
#define Info(P) (P)->Info
#define Next(P) (P)->next

//tiket
typedef struct
{
    char idTiket[10];         
    char namaPenumpang[50];   
    char tujuan[50];          
    int jumlahKursi;          
    int totalHarga;           
    char status[15]; 
} DataTiket;

typedef struct NodeTiket {
    DataTiket Info;               
    struct NodeBus* bus;      
} NodeTiket;

extern NodeTiket *HeadTiket;

int isTiketListEmpty();
void insertTiket(DataTiket tiketBaru, NodeBus* bus);
void editTiketByID(char idTiket[], DataTiket tiketBaru);
void printAllTiket();
NodeTiket* searchTiketByID(char idTiket[]);
int deleteTiketByID(char idTiket[]);
void deleteAllTiket();

#endif
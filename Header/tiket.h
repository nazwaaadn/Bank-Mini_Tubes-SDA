#ifndef TIKET_H
#define TIKET_H

#include <time.h>
#include "../Header/user.h"
#include "../Header/bus.h"


// Struktur Tiket
typedef struct {
    char idTiket[20];
    char namaPenumpang[50];
    char awal[50];
    char tujuan[50];
    char idBus[20];
    char status[20]; // aktif / cancelled / done
} DataTiket;

extern NodeUser* user; // Pointer ke pengguna yang sedang login

// Fungsi utama tiket (semua berbasis file, bukan linked list)
void muatDataBus();
void pesanTiket(NodeUser* user);
void batalkanTiket(const char* idTiket);
void simpanTiketKeFile(DataTiket tiket);
void printAllTiket();
void printTiketAktifByUser();
void printAllTiketByUser();
void bacaDataTiket(char* filename, DataTiket tiket[], int* tiketCount);
void bacaDataBus(char* filename, DataBus buses[], int* busCount);
void simulasiPerjalananUser(DataBus* bus, DataTiket* tiket, int tiketCount);
const char* cariIdBusByUser(NodeUser* user, DataTiket* tiket, int tiketCount);
void simpanDataTiketKeFile(DataTiket tiket[], int tiketCount);

#endif

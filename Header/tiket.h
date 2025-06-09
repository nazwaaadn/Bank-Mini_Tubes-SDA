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
    char status[20]; // aktif / cancelled
} DataTiket;

extern NodeUser* currentUser; // Pointer ke pengguna yang sedang login
// Forward declaration (karena NodeBus didefinisikan di tempat lain)
struct NodeBus;

typedef struct NodeTiket {
    DataTiket Info;
    struct NodeBus* bus;         
    struct NodeTiket* next;
} NodeTiket;

// Global Head
extern NodeTiket *HeadTiket;

// Prototipe fungsi
void muatDataBus();
int isTiketListEmpty();
void insertTiket(DataTiket tiketBaru, struct NodeBus* bus);
void printAllTiket();
NodeTiket* searchTiketByID(char idTiket[]);
NodeBus* searchBusByIDFromFile(const char* idTarget);
void pesanTiket(NodeUser* user);
int deleteTiketByID(char idTiket[]);
void deleteAllTiket();
void cetakTiket(char idTiket[]);
void batalkanTiket(const char* idTiket);
void simpanTiketKeFile(DataTiket tiket);
void printTiketAktifByUser();
void printAllTiketByUser();
void bacaDataTiket(char* filename, DataTiket tiket[], int* tiketCount);
void bacaDataBus(char* filename, DataBus buses[], int* busCount);
void simulasiPerjalananUser(DataBus* bus, DataTiket* tiket, int tiketCount, NodeUser* user);
const char* cariIdBusByUser(NodeUser* user, DataTiket* tiket, int tiketCount);

#endif

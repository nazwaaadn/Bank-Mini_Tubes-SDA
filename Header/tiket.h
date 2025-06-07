#ifndef tiket_H
#define tiket_H

#include "BOOLEAN.h"
#include <stdio.h>
#include <time.h>

#define Nil NULL
#define Info(P) (P)->Info
#define Next(P) (P)->next

// Struktur Tiket
typedef struct {
    char idTiket[10];          // ID unik tiket
    char idBus[20];            // ID Bus string
    char namaPenumpang[50];    // Nama pemilik akun
    char awal[50];             // Lokasi awal (dari) 
    char tujuan[50];           // Tujuan perjalanan
    int noKursi;               // Nomor kursi
    int jumlahKursi;           // Jumlah kursi (jika group)
    int totalHarga;            // Total harga tiket
    char status[15];           // Status tiket ("confirmed", "cancelled", etc)
} DataTiket;

// Forward declaration of NodeBus (karena didefinisikan di tempat lain)
struct NodeBus;

typedef struct NodeTiket {
    DataTiket Info;
    struct NodeBus* bus;         // Pointer ke data bus terkait
    struct NodeTiket* next;
} NodeTiket;

// Global pointer head
extern NodeTiket *HeadTiket;

// Fungsi-fungsi Tiket
int isTiketListEmpty();                                               // Mengecek apakah daftar tiket kosong
void insertTiket(DataTiket tiketBaru, struct NodeBus* bus);           // Menambahkan tiket baru
void editTiketByID(char idTiket[], DataTiket tiketBaru);              // Mengedit tiket berdasarkan ID
void printAllTiket();                                                 // Menampilkan semua tiket
NodeTiket* searchTiketByID(char idTiket[]);                           // Mencari tiket berdasarkan ID

void pesanTiket(NodeUser* user);                                  // Simulasi pemesanan tiket oleh user

int deleteTiketByID(char idTiket[]);                                 // Menghapus tiket berdasarkan ID
void deleteAllTiket();                                               // Menghapus semua tiket
void cetakTiket(char idTiket[]);                                     // Mencetak (menampilkan) detail tiket
void batalkanTiket(char idTiket[]);                                  // Mengubah status tiket jadi "cancelled" & mengembalikan kursi

void simpanTiketKeFile(DataTiket tiket);                                  // Menyimpan tiket ke file
void timeToString(time_t waktu, char* buffer);                            // Mengonversi waktu ke string
void muatDataBus();                                                       // Memuat data bus dari file


#endif

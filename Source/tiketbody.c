#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "tiket.h"
#include "bus.h"


NodeTiket *HeadTiket = NULL;

int isTiketListEmpty() {
    return HeadTiket == NULL;
}

void insertTiket(DataTiket tiketBaru, NodeBus* bus) {
    NodeTiket* newNode = (NodeTiket*)malloc(sizeof(NodeTiket));
    if (!newNode) {
        printf("Gagal mengalokasikan memori untuk tiket.\n");
        return;
    }

    newNode->Info = tiketBaru;
    newNode->bus = bus;
    newNode->next = NULL;

    if (HeadTiket == NULL) {
        HeadTiket = newNode;
    } else {
        NodeTiket* temp = HeadTiket;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    printf("Tiket berhasil ditambahkan untuk %s ke tujuan %s.\n", tiketBaru.namaPenumpang, tiketBaru.tujuan);
}


void editTiketByID(char idTiket[], DataTiket tiketBaru) {
    NodeTiket* tiket = searchTiketByID(idTiket);
    if (tiket == NULL) {
        printf("Tiket dengan ID %s tidak ditemukan.\n", idTiket);
        return;
    }

    tiket->Info = tiketBaru;
    printf("Tiket dengan ID %s berhasil diedit.\n", idTiket);
}


void cetakTiket(char idTiket[]) {
    NodeTiket* tiket = searchTiketByID(idTiket);
    if (tiket == NULL) {
        printf("Tiket dengan ID %s tidak ditemukan.\n", idTiket);
        return;
    }

    printf("\n======= CETAK TIKET =======\n");
    printf("ID Tiket     : %s\n", tiket->Info.idTiket);
    printf("Nama         : %s\n", tiket->Info.namaPenumpang);
    printf("Tujuan       : %s\n", tiket->Info.tujuan);
    printf("Jadwal       : %s\n", tiket->Info.jadwal);
    printf("No. Kursi    : %d\n", tiket->Info.noKursi);
    printf("Jumlah Kursi : %d\n", tiket->Info.jumlahKursi);
    printf("Total Harga  : Rp%d\n", tiket->Info.totalHarga);
    printf("Status       : %s\n", tiket->Info.status);
    printf("===========================\n");
}


void printAllTiket() {
    if (isTiketListEmpty()) {
        printf("Tidak ada tiket yang tersedia.\n");
        return;
    }

    NodeTiket* current = HeadTiket;
    printf("\n=== DAFTAR SEMUA TIKET ===\n");
    while (current != NULL) {
        printf("ID Tiket     : %s\n", current->Info.idTiket);
        printf("Nama         : %s\n", current->Info.namaPenumpang);
        printf("Tujuan       : %s\n", current->Info.tujuan);
        printf("Jadwal       : %s\n", current->Info.jadwal);
        printf("No Kursi     : %d\n", current->Info.noKursi);
        printf("Jumlah Kursi : %d\n", current->Info.jumlahKursi);
        printf("Total Harga  : %d\n", current->Info.totalHarga);
        printf("Status       : %s\n", current->Info.status);
        printf("-----------------------------\n");

        current = current->next;
    }
}


NodeTiket* searchTiketByID(char idTiket[]) {
    NodeTiket* current = HeadTiket;
    while (current != NULL) {
        if (strcmp(current->Info.idTiket, idTiket) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Fungsi simulasi pemesanan tiket oleh user
void pesanTiket(NodeUser* user) {
    if (user == NULL) {
        printf("Anda belum login.\n");
        return;
    }

    char tujuan[50], jadwal[20];
    int jumlahKursi, hargaPerKursi = 50000;

    printf("Masukkan tujuan       : ");
    scanf(" %[^\n]", tujuan);
    printf("Masukkan jadwal       : ");
    scanf(" %[^\n]", jadwal);
    printf("Jumlah kursi dipesan  : ");
    scanf("%d", &jumlahKursi);

    // Buat data tiket baru
    DataTiket tiketBaru;
    sprintf(tiketBaru.idTiket, "TK%ld", time(NULL));  // ID unik
    strcpy(tiketBaru.namaPenumpang, user->Info.nama);
    strcpy(tiketBaru.tujuan, tujuan);
    strcpy(tiketBaru.jadwal, jadwal);
    tiketBaru.noKursi = rand() % 40 + 1;  // Nomor kursi acak
    tiketBaru.jumlahKursi = jumlahKursi;
    tiketBaru.totalHarga = hargaPerKursi * jumlahKursi;
    strcpy(tiketBaru.status, "confirmed");

    NodeBus* bus = HeadBus; // Sementara: ambil bus pertama saja
    if (bus == NULL) {
        printf("Tidak ada bus tersedia.\n");
        return;
    }

    insertTiket(tiketBaru, bus);
    cetakTiket(tiketBaru.idTiket);
}


int deleteTiketByID(char idTiket[]) {
    NodeTiket *current = HeadTiket;
    NodeTiket *prev = NULL;

    while (current != NULL) {
        if (strcmp(current->Info.idTiket, idTiket) == 0) {
            if (prev == NULL) {
                HeadTiket = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            printf("Tiket dengan ID %s berhasil dihapus.\n", idTiket);
            return 1;
        }
        prev = current;
        current = current->next;
    }

    printf("Tiket dengan ID %s tidak ditemukan.\n", idTiket);
    return 0;
}


void deleteAllTiket() {
    NodeTiket* current = HeadTiket;
    while (current != NULL) {
        NodeTiket* temp = current;
        current = current->next;
        free(temp);
    }
    HeadTiket = NULL;
    printf("Semua tiket telah dihapus.\n");
}

void batalkanTiket(char idTiket[]) {
    NodeTiket* tiket = searchTiketByID(idTiket);
    if (tiket == NULL) {
        printf("Tiket tidak ditemukan.\n");
        return;
    }

    if (strcmp(tiket->Info.status, "cancelled") == 0) {
        printf("Tiket sudah dibatalkan sebelumnya.\n");
        return;
    }

    strcpy(tiket->Info.status, "cancelled");
    printf("Tiket dengan ID %s berhasil dibatalkan.\n", idTiket);
}

#include <stdio.h>
#include <string.h>
#include "queue.h"
#include "admin.h"
#include "bus.h"

int LoginAdmin() {
    char id[50];
    char password[50];

    printf("===== LOGIN ADMIN =====\n");
    printf("ID Admin     : ");
    scanf("%s", id);
    printf("Password     : ");
    scanf("%s", password);

    if (strcmp(id, "admin") == 0 && strcmp(password, "adminkece") == 0) {
        printf("Login berhasil!\n\n");
        return 1;
    } else {
        printf("ID atau Password salah!\n\n");
        return 0;
    }
}

void LihatBusDiSetiapTerminal(Queue* q) {
    NodeBus* curr = Front(*q);
    int i = 1;
    if(is_Empty(*q)) {
        printf("Antrian kosong.\n");
    } else {
        printf("\n=== Daftar Bus Dalam Antrian ===\n");
        while(curr != NULL) {
            printf("%d. ID Bus: %s | Supir: %s\n", i, curr->Info.idBus, curr->Info.namaSupir);
            curr = Next(curr);
            i++;
        }
    }
}

void ReleaseBusDariTerminal(Queue* q) {
    DataBus busKeluar;
    deQueue(q, &busKeluar);
    printf("Bus %s (%s) telah keluar dari antrian.\n", busKeluar.idBus, busKeluar.namaSupir);
}

void TambahBusBaru() {
    printf(">> Tambah Bus Baru (belum diimplementasi)\n");
}

void LihatDataTiketPelanggan() {
    printf(">> Lihat Data Tiket Pelanggan (belum diimplementasi)\n");
}

void RiwayatTransaksi() {
    printf(">> Riwayat Transaksi Tiket (belum diimplementasi)\n");
}

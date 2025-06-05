#include <stdio.h>
#include <string.h>
#include "../Header/admin.h"


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

// Implementasi placeholder (belum isi)
void LihatBusDiSetiapTerminal() {
    printf(">> Lihat Bus di Setiap Terminal (belum diimplementasi)\n");
}

void ReleaseBusDariTerminal() {
    printf(">> Release Bus dari Terminal (belum diimplementasi)\n");
}

void TambahBusBaru() {
    printf(">> Tambah Bus Baru (belum diimplementasi)\n");
}

void AturRuteBusKembali() {
    printf(">> Atur Rute Bus yang Telah Kembali (belum diimplementasi)\n");
}

void LihatDataTiketPelanggan() {
    printf(">> Lihat Data Tiket Pelanggan (belum diimplementasi)\n");
}

void LaporanPerjalananBus() {
    printf(">> Laporan Perjalanan Bus (belum diimplementasi)\n");
}

void KelolaTerminal() {
    printf(">> Manajemen Terminal (belum diimplementasi)\n");
}

void KelolaAkunPelanggan() {
    printf(">> Manajemen Akun Pelanggan (belum diimplementasi)\n");
}

void RiwayatTransaksi() {
    printf(">> Riwayat Transaksi Tiket (belum diimplementasi)\n");
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "tiket.h"
#include "bus.h"
#include "desain.h"

void menuPelanggan(NodeUser* user) {
    int pilihan;
    char buffer[10];
    char id[20];

    do {
        printf("\n=== MENU PELANGGAN - %s ===\n", user->Info.nama);
        printf("1. Lihat Bus & Pesan Tiket\n");
        printf("2. Lihat Tiket Aktif\n");
        printf("3. Batalkan Tiket\n");
        printf("4. Lihat Riwayat Tiket\n");
        printf("0. Keluar\n");
        printf("Pilihan: ");

        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &pilihan) != 1) {
            printf("Input tidak valid.\n");
            continue;
        }

        switch (pilihan) {
            case 1:
                printAllBus();
                pesanTiket(user);
                break;
            case 2:
                printTiketAktifByUser(user);
                break;
            case 3:
                printf("Masukkan ID Tiket yang ingin dibatalkan: ");
                fgets(id, sizeof(id), stdin);
                id[strcspn(id, "\n")] = '\0';
                batalkanTiket(id);
                break;
            case 4:
                printAllTiketByUser(user);
                break;
            case 0:
                printf("Logout berhasil.\n");
                break;
            default:
                printf("Pilihan tidak valid.\n");
        }
    } while (pilihan != 0);
}

int main() {
    SplashScreenUser();
    loadUsersFromFile("FileManajemen/userDatabase.txt");
    muatDataBus(); // <-- Tambahkan baris ini!


    int pilihan;
    char buffer[10];

    do {
        printf("\n=== MENU UTAMA ===\n");
        printf("1. Login\n");
        printf("2. Register\n");
        printf("3. Keluar\n");
        printf("Pilihan Anda: ");

        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &pilihan) != 1) {
            printf("Input tidak valid. Masukkan angka.\n");
            continue;
        }

        switch (pilihan) {
            case 1:
                Login();
                break;
            case 2:
                Register();
                break;
            case 3:
                printf("Terima kasih telah menggunakan layanan Translibe!\n");
                break;
            default:
                printf("Pilihan tidak valid.\n");
        }
    } while (pilihan != 3);

    return 0;
}

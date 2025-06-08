#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "tiket.h"
#include "bus.h"
#include "desain.h"

int main() {
    SplashScreenUser();
    loadUsersFromFile("FileManajemen/userDatabase.txt");
    muatDataBus(); // <-- Tambahkan baris ini!

    int pilihan;
    char buffer[10];
    NodeUser* user = NULL; // Variable untuk menyimpan user yang login

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
                user = Login();  // Login dan simpan user yang berhasil login
                if (user != NULL) {
                    UserMenu(user); // Panggil menu pelanggan setelah login berhasil
                }
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

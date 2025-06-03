#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"

DataUser *currentUser = NULL;

int main() {
    int pilihan;

    do {
        system("cls"); // Gunakan "clear" jika di Linux/Mac
        printf("=====================================\n");
        printf("|    SELAMAT DATANG DI T-JAKARTA    |\n");
        printf("=====================================\n");
        printf("1. Login\n");
        printf("2. Register\n");
        printf("3. Keluar\n");
        printf("=====================================\n");
        printf("Masukkan pilihan Anda: ");
        scanf("%d", &pilihan);
        getchar();

        switch (pilihan) {
            case 1:
                currentUser = LoginUser();
                if (currentUser != NULL) {
                    printf("Login berhasil. Selamat datang, %s!\n", currentUser->nama);
                } else {
                    printf("Login gagal. Silakan coba lagi.\n");
                }
                printf("Tekan Enter untuk kembali ke menu...\n");
                getchar();
                break;

            case 2:
                RegisterUser();
                printf("Registrasi selesai. Tekan Enter untuk kembali ke menu...\n");
                getchar();
                break;

            case 3:
                printf("Terima kasih telah menggunakan program ini.\n");
                break;

            default:
                printf("Pilihan tidak valid. Tekan Enter untuk lanjut...\n");
                getchar();
                break;
        }

    } while (pilihan != 3);

    return 0;
}

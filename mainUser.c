#include <stdio.h>
#include <string.h>
#include "user.h"
#include "bus.h"
#include "desain.h"

int main() {
    SplashScreenUser();  // Tampilkan tampilan awal Translibe
    loadUsersFromFile("FileManajemen/userDatabase.txt"); // Memuat data pengguna dari file

    int pilihan;
    do {
        printf("\n=== MENU PELANGGAN ===\n");
        printf("1. Login\n");
        printf("2. Register\n");
        printf("3. Keluar\n");
        printf("Pilihan Anda: ");
        scanf("%d", &pilihan);
        getchar();  // Untuk menangani newline

        switch (pilihan) {
            case 1:
                Login(); // Fungsi login (akan masuk ke menu pelanggan)
                break;
            case 2:
                Register(); // Fungsi registrasi
                break;
            case 3:
                printf("Terima kasih telah menggunakan layanan Translibe!\n");
                break;
            default:
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
        }
    } while (pilihan != 3);

    return 0;
}

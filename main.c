#include <stdio.h>
#include <string.h>
#include "user.h"
#include "bus.h"
#include "library.h"

int main() {
    SplashScreen();
    loadUsersFromFile("userDatabase.txt");

    int pilihan;
    do {
        printf("\nMasukkan pilihan Anda: ");
        scanf("%d", &pilihan);
        getchar(); // Flush newline

        switch (pilihan) {
            case 1:
                Login();
                break;
            case 2:
                printf("Belum ada fitur admin.\n");
                break;
            case 3:
                printf("Terima kasih telah menggunakan sistem kami!\n");
                break;
            default:
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
        }
    } while (pilihan != 3);

    return 0;
}
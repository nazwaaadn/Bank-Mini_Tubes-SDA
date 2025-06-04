#include <stdio.h>
#include "user.h"
#include "tiket.h"
#include "bus.h"

int main() {
    SplashScreen();
    printf("Masukkan pilihan Anda: ");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            
            UserMenu();
            int userChoice;
            scanf("%d", &userChoice);
            switch (userChoice) {
                case 1:
                    PesanTiket();
                    break;
                case 2:
                    LihatTiketAktif();
                    break;
                case 3:
                    RiwayatPerjalanan();
                    break;
                case 4:
                    BatalkanTiket();
                    break;
                case 5:
                    printf("Kembali ke menu utama...\n");
                    break;
                default:
                    printf("Pilihan tidak valid. Silakan coba lagi.\n");
            }
            break;
        case 2:
            AdminMenu();
            int adminChoice;
            scanf("%d", &adminChoice);
            switch (adminChoice) {
                case 1:
                    LihatBusDiTerminal();
                    break;
                case 2:
                    ReleaseBus();
                    break;
                case 3:
                    TambahBus();
                    break;
                case 4:
                    AturRuteBus();
                    break;
                case 5:
                    LihatDataTiketPelanggan();
                    break;
                case 6:
                    LaporanPerjalananBus();
                    break;
                case 7:
                    ManajemenTerminal();
                    break;
                case 8:
                    ManajemenAkunPelanggan();
                    break;
                case 9:
                    RiwayatTransaksiTiket();
                    break;
                case 10:
                    printf("Kembali ke menu utama...\n");
                    break;
                default:
                    printf("Pilihan tidak valid. Silakan coba lagi.\n");
            }
            break;
        case 3:
            printf("Terima kasih telah menggunakan sistem kami. Sampai jumpa!\n");
            break;
        default:
            printf("Pilihan tidak valid. Silakan coba lagi.\n");
            break;
    }
}

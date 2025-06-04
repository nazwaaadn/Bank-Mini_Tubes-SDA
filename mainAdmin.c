#include "desain.h"
#include "user.h"
#include "bus.h"
#include "terminal.h"
#include "tiket.h"
#include "admin.h"
#include <stdio.h>

int main() {
    SplashScreenAdmin();  // Tampilan awal

    if (!LoginAdmin()) {
        printf("Akses ditolak. Keluar dari program...\n");
        return 0;
    }

    loadUsersFromFile("userDatabase.txt");

    int pilihan;
    do {
        AdminMenu();  // Tampilan menu dari desain.h
        scanf("%d", &pilihan);
        getchar();  // Bersihkan newline

        switch (pilihan) {
            case 1:
                LihatBusDiSetiapTerminal();
                break;
            case 2:
                ReleaseBusDariTerminal();
                break;
            case 3:
                TambahBusBaru();
                break;
            case 4:
                AturRuteBusKembali();
                break;
            case 5:
                LihatDataTiketPelanggan();
                break;
            case 6:
                LaporanPerjalananBus();
                break;
            case 7:
                KelolaTerminal();
                break;
            case 8:
                KelolaAkunPelanggan();
                break;
            case 9:
                RiwayatTransaksi();
                break;
            case 10:
                printf("Keluar dari menu admin...\n");
                break;
            default:
                printf("Pilihan tidak valid.\n");
        }
    } while (pilihan != 10);

    return 0;
}

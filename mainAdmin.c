#include "Header/desain.h"
#include "Header/bus.h"
#include "Header/terminal.h"
#include "Header/admin.h"
#include <stdio.h>

int main() {
    SplashScreenAdmin();  // Tampilan awal

    if (!LoginAdmin()) {
        printf("Akses ditolak. Keluar dari program...\n");
        return 0;
    }

    // loadUsersFromFile("userDatabase.txt");

    int pilihan;
    do {
        AdminMenu();  // Tampilan menu dari desain.h
        scanf("%d", &pilihan);
        getchar();  // Bersihkan newline

        switch (pilihan) {
            // case 1:
            //     LihatBusDiSetiapTerminal();
            //     break;
            // case 2:
            //     ReleaseBusDariTerminal();
            //     break;
            case 1:
                ManajemenBus();
                printf("Masukkan pilihan Anda: ");
                int pilihMenuAdmin;
                scanf("%d", &pilihMenuAdmin);

                switch (pilihMenuAdmin) {
                    case 1: {
                        terminalTree T;
                        CreateTerminal(T);  // Inisialisasi terminalTree
                        int root = 0;  // Asumsi root adalah 0
                        NodeBus *busBaru = inputDataBus(T, root); // langsung NodeBus*
                        insertBus(busBaru); // tanpa &
                        printf("Data bus berhasil ditambahkan.\n");
                        break;
                    }

                    case 2: {
                        char idBus[10];
                        printf("Masukkan ID Bus yang ingin dihapus: ");
                        scanf("%s", &idBus);
                        deleteBus(idBus);
                        printf("Bus dengan ID %s berhasil dihapus.\n", idBus);
                        break;
                    }
                    case 3: {
                        char idBus[10];
                        printf("Masukkan ID Bus yang ingin diedit: ");
                        scanf("%s", &idBus);
                        editBus(idBus);
                        break;
                    }
                    case 4:
                        printAllBus();
                        break;
                    case 5:
                        printf("Kembali ke menu admin...\n");
                        break;
                    default:
                        printf("Pilihan tidak valid.\n");
                }
                break;
            // case 4:
            //     AturRuteBusKembali();
            //     break;
            // case 5:
            //     LihatDataTiketPelanggan();
            //     break;
            // case 6:
            //     LaporanPerjalananBus();
            //     break;
            // case 7:
            //     KelolaTerminal();
            //     break;
            // case 8:
            //     KelolaAkunPelanggan();
            //     break;
            // case 9:
            //     RiwayatTransaksi();
            //     break;
            case 2:
                printf("Keluar dari menu admin...\n");
                break;
            default:
                printf("Pilihan tidak valid.\n");
        }
    } while (pilihan != 10);

    return 0;
}

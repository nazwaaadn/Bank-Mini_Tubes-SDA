#include "Header/desain.h"
#include "Header/bus.h"
#include "Header/terminal.h"
#include "Header/admin.h"
#include "Header/tiket.h"
#include <stdio.h>

int main() {
    SplashScreenAdmin();  // Tampilan awal

    if (!LoginAdmin()) {
        printf("Akses ditolak. Keluar dari program...\n");
        return 0;
    }


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
            case 1: {
                int pilihMenuAdmin;
                do {
                    ManajemenBus(); // Tampilkan menu setiap kali loop
                    printf("Masukkan pilihan Anda: ");
                    scanf("%d", &pilihMenuAdmin);

                    switch (pilihMenuAdmin) {
                        case 1: {
                            terminalTree T;
                            CreateTerminal(T);  // Inisialisasi terminalTree
                            int root = 0;  // Asumsi root adalah 0
                            NodeBus *busBaru = inputDataBus(T, root); // langsung NodeBus*
                            printf("Data bus berhasil ditambahkan.\n");
                            break;
                        }

                        case 2: {
                            char idBus[10];
                            printf("Masukkan ID Bus yang ingin dihapus: ");
                            scanf("%s", idBus);
                            deleteBus(idBus);
                            break;
                        }

                        case 3: {
                            char idBus[10];
                            printf("Masukkan ID Bus yang ingin diedit: ");
                            scanf("%s", idBus);                     
                            editBus(idBus);
                            break;
                        }

                        case 4:
                            printAllBus();
                            break;

                        case 5:
                            printf("Keluar dari menu manajemen bus...\n");
                            break;

                        default:
                            printf("Pilihan tidak valid.\n");
                    }
                    printf("\n");
                } while (pilihMenuAdmin != 5);
                break;
            }

                break;
            case 3:
                terminalTree T;
                CreateTerminal(T); 
                tampilkanTerminal(T);
                break;
            case 4:
                printAllTiket();
                break;
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

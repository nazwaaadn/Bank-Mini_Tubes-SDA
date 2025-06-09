#include "Header/desain.h"
#include "Header/bus.h"
#include "Header/terminal.h"
#include "Header/admin.h"
#include "Header/queue.h"
#include "Header/user.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    SplashScreenAdmin();

    Queue queueBus;
    CreateQueue(&queueBus);    // Inisialisasi queue

    if (!LoginAdmin()) {
        printf("Akses ditolak. Keluar dari program...\n");
        return 0;
    }

    int pilihan;
    do {
        AdminMenu();
        scanf("%d", &pilihan); getchar();

        switch (pilihan) {
            case 1: {
                int pilihMenuAdmin;
                do {
                    ManajemenBus();
                    printf("Masukkan pilihan Anda: ");
                    scanf("%d", &pilihMenuAdmin); getchar();

                    switch (pilihMenuAdmin) {
                        case 1: {
                            terminalTree T;
                            CreateTerminal(T);
                            int root = 0;
                            NodeBus *busBaru = inputDataBus(T, root);
                            if (busBaru != NULL) {
                                // Setelah tambah bus, langsung masukkan ke antrian!
                                EnQueue(&queueBus, busBaru->Info);
                                printf("Data bus berhasil ditambahkan & masuk ke antrian.\n");
                                free(busBaru);
                            }
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
            case 2:
                ReleaseBusDariTerminal(&queueBus);
                break;
            case 3:
                LihatBusDiSetiapTerminal(&queueBus);
                break;
            case 4:
                printf("Fitur riwayat transaksi tiket belum diimplementasikan.\n");
                break;
            case 5:
                printf("Keluar dari menu admin...\n");
                break;
            default:
                printf("Pilihan tidak valid.\n");
        }
    } while (pilihan != 5);

    return 0;
}

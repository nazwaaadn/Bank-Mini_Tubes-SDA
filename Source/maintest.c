#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "user.h"
#include "tiket.h"
#include "bus.h"

// Simulasi user dan bus (data dummy)
NodeUser* loggedInUser = NULL;
NodeBus* HeadBus = NULL;

void dummyUser() {
    loggedInUser = (NodeUser*)malloc(sizeof(NodeUser));
    strcpy(loggedInUser->Info.password, "pass");
    strcpy(loggedInUser->Info.nama, "Budi");
}

void dummyBus() {
    HeadBus = (NodeBus*)malloc(sizeof(NodeBus));
    strcpy(HeadBus->Info.idBus, "B001");
    strcpy(HeadBus->Info.kelas, "Eksekutif");
    HeadBus->Info.kapasitas = 40;
    HeadBus->next = NULL;
}

void menu() {
    printf("\n=== MENU UTAMA ===\n");
    printf("1. Pesan Tiket\n");
    printf("2. Cetak Tiket\n");
    printf("3. Edit Tiket\n");
    printf("4. Hapus Tiket\n");
    printf("5. Batalkan Tiket\n");
    printf("6. Tampilkan Semua Tiket\n");
    printf("7. Hapus Semua Tiket\n");
    printf("0. Keluar\n");
    printf("==================\n");
}

int main() {
    dummyUser();  // Simulasikan user login
    dummyBus();   // Simulasikan data bus
    srand(time(NULL));

    int pilihan;
    char id[20];

    do {
        menu();
        printf("Pilih menu: ");
        scanf("%d", &pilihan);
        getchar(); // Flush newline

        switch (pilihan) {
            case 1:
                cetakTiket(loggedInUser);
                break;
            case 2:
                printf("Masukkan ID Tiket: ");
                scanf(" %[^\n]", id);
                cetakTiket(id);
                break;
            case 3: {
                printf("Masukkan ID Tiket yang ingin diedit: ");
                scanf(" %[^\n]", id);

                NodeTiket* tiket = searchTiketByID(id);
                if (!tiket) {
                    printf("Tiket tidak ditemukan.\n");
                    break;
                }

                DataTiket baru;
                strcpy(baru.idTiket, tiket->Info.idTiket);
                printf("Nama Penumpang [%s]: ", tiket->Info.namaPenumpang);
                scanf(" %[^\n]", baru.namaPenumpang);
                printf("Tujuan baru        : ");
                scanf(" %[^\n]", baru.tujuan);
                printf("Jadwal baru        : ");
                scanf(" %[^\n]", baru.jadwal);
                printf("Jumlah kursi baru  : ");
                scanf("%d", &baru.jumlahKursi);
                baru.noKursi = tiket->Info.noKursi;  // Tetap sama
                baru.totalHarga = baru.jumlahKursi * 50000;
                strcpy(baru.status, tiket->Info.status);

                editTiketByID(id, baru);
                break;
            }
            case 4:
                printf("Masukkan ID Tiket yang ingin dihapus: ");
                scanf(" %[^\n]", id);
                deleteTiketByID(id);
                break;
            case 5:
                printf("Masukkan ID Tiket yang ingin dibatalkan: ");
                scanf(" %[^\n]", id);
                batalkanTiket(id);
                break;
            case 6:
                printAllTiket();
                break;
            case 7:
                deleteAllTiket();
                break;
            case 0:
                printf("Terima kasih!\n");
                break;
            default:
                printf("Pilihan tidak valid!\n");
        }

    } while (pilihan != 0);

    deleteAllTiket();  // Clean up before exit
    free(HeadBus);
    free(loggedInUser);
    return 0;
}

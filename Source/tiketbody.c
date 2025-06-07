#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "tiket.h"
#include "bus.h"
#include "terminal.h"
#include <math.h>

time_t parseDatetime(const char* str) {
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    if (sscanf(str, "%d-%d-%d %d:%d",
               &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
               &tm.tm_hour, &tm.tm_min) != 5) {
        return (time_t)(-1); // Invalid format
    }

    tm.tm_year -= 1900; // struct tm expects years since 1900
    tm.tm_mon -= 1;     // struct tm expects months 0-11

    return mktime(&tm);
}

void timeToString(time_t waktu, char* buffer) {
    strftime(buffer, 30, "%Y-%m-%d %H:%M", localtime(&waktu));
}
//=====================================================================

NodeTiket *HeadTiket = NULL;

int isTiketListEmpty() {
    return HeadTiket == NULL;
}

void insertTiket(DataTiket tiketBaru, NodeBus* bus) {
    NodeTiket* newNode = (NodeTiket*)malloc(sizeof(NodeTiket));
    if (!newNode) {
        printf("Gagal mengalokasikan memori untuk tiket.\n");
        return;
    }

    newNode->Info = tiketBaru;
    newNode->bus = bus;
    newNode->next = NULL;

    if (HeadTiket == NULL) {
        HeadTiket = newNode;
    } else {
        NodeTiket* temp = HeadTiket;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    printf("Tiket berhasil ditambahkan untuk %s ke tujuan %s.\n", tiketBaru.namaPenumpang, tiketBaru.tujuan);
}


// void editTiketByID(char idTiket[], DataTiket tiketBaru) {
//     NodeTiket* tiket = searchTiketByID(idTiket);
//     if (tiket == NULL) {
//         printf("Tiket dengan ID %s tidak ditemukan.\n", idTiket);
//         return;
//     }

//     tiket->Info = tiketBaru;
//     printf("Tiket dengan ID %s berhasil diedit.\n", idTiket);
// }


void cetakTiket(char idTiket[]) {
    NodeTiket* tiket = searchTiketByID(idTiket);
    if (tiket == NULL) {
        printf("Tiket dengan ID %s tidak ditemukan.\n", idTiket);
        return;
    }

	printf("\n+===========================================+\n");
	printf("|          TIKET BUS TRANSJAKARTA           |\n");
	printf("+===========================================+\n");
	printf("| ID Tiket     : %-26s |\n", tiket->Info.idTiket);
	printf("| Nama         : %-26s |\n", tiket->Info.namaPenumpang);
	printf("| Tujuan       : %-26s |\n", tiket->Info.tujuan);
	printf("| Jadwal       : %-26s |\n", tiket->Info.jadwal);
	printf("| No. Kursi    : %-26d |\n", tiket->Info.noKursi);
	printf("| Jumlah Kursi : %-26d |\n", tiket->Info.jumlahKursi);
	printf("| Total Harga  : Rp%-24d |\n", tiket->Info.totalHarga);
	printf("| Status       : %-26s |\n", tiket->Info.status);
	printf("+===========================================+\n");
	printf("|     TERIMA KASIH TELAH MEMESAN TIKET!     |\n");
	printf("+===========================================+\n");

}


void printAllTiket() {
    if (isTiketListEmpty()) {
        printf("Tidak ada tiket yang tersedia.\n");
        return;
    }

    NodeTiket* current = HeadTiket;
    printf("\n=== DAFTAR SEMUA TIKET ===\n");
    while (current != NULL) {
		printf("\n+===========================================+\n");
		printf("|          TIKET BUS TRANSJAKARTA           |\n");
		printf("+===========================================+\n");
        printf("| ID Tiket     : %-26s |\n", current->Info.idTiket);
        printf("| Nama         : %-26s |\n", current->Info.namaPenumpang);
        printf("| Tujuan       : %-26s |\n", current->Info.tujuan);
        printf("| Jadwal       : %-26s |\n", current->Info.jadwal);
        printf("| No. Kursi    : %-26d |\n", current->Info.noKursi);
        printf("| Jumlah Kursi : %-26d |\n", current->Info.jumlahKursi);
        printf("| Total Harga  : Rp%-24d |\n", current->Info.totalHarga);
        printf("| Status       : %-26s |\n", current->Info.status);
		printf("+===========================================+\n");
		
        current = current->next;
    }
}


NodeTiket* searchTiketByID(char idTiket[]) {
    NodeTiket* current = HeadTiket;
    while (current != NULL) {
        if (strcmp(current->Info.idTiket, idTiket) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Fungsi simulasi pemesanan tiket oleh user
void pesanTiket(NodeUser* user) {
    if (user == NULL) {
        printf("Anda belum login.\n");
        return;
    }

    char tujuan[50], jadwalStr[25];
    struct tm tm_jadwal = {0};
    time_t jadwal = mktime(&tm_jadwal);
    int jumlahKursi;
    int hargaPerKursi = 50000;

    printf("Masukkan tujuan (terminal)       : ");
    scanf(" %[^\n]", tujuan);
    printf("Masukkan jadwal (YYYY-MM-DD HH:MM): ");
    scanf(" %[^\n]", jadwalStr);
    printf("Jumlah kursi dipesan  : ");
    scanf("%d", &jumlahKursi);
   
    // Convert jadwal string to time_t
    int tahun, bulan, hari, jam, menit;
    if (sscanf(jadwalStr, "%d-%d-%d %d:%d", &tahun, &bulan, &hari, &jam, &menit) != 5) {
        printf("Format jadwal salah.\n");
        return;
    }

    tm_jadwal.tm_year = tahun - 1900;
    tm_jadwal.tm_mon = bulan - 1;
    tm_jadwal.tm_mday = hari;
    tm_jadwal.tm_hour = jam;
    tm_jadwal.tm_min = menit;

    // Cari semua bus yang rutenya mengandung tujuan dan jadwal cocok
    NodeBus* current = HeadBus;
    NodeBus* foundBuses[100];
    int foundCount = 0;

    while (current != NULL) {
        // Cek apakah tujuan ada dalam rute
        NodeRute* r = current->Info.rute;
        int tujuanMatch = 0;
        while (r != NULL) {
            if (strcmp(r->namaTerminal, tujuan) == 0) {
                tujuanMatch = 1;
                break;
            }
            r = r->next;
        }

        // Cek jadwal keberangkatan cocok (toleransi 1 menit)
        if (tujuanMatch && fabs(difftime(current->Info.keberangkatan, jadwal)) < 60) {
            foundBuses[foundCount++] = current;
        }

        current = current->next;
    }

    if (foundCount == 0) {
        printf("Tidak ada bus dengan tujuan dan jadwal yang sesuai.\n");
        return;
    }

    // Tampilkan daftar bus ditemukan
    printf("\nBus tersedia:\n");
    for (int i = 0; i < foundCount; i++) {
        char waktuStr[30];
        strftime(waktuStr, sizeof(waktuStr), "%Y-%m-%d %H:%M", localtime(&foundBuses[i]->Info.keberangkatan));
        printf("[%d] ID: %s | Supir: %s | Kelas: %c | Kapasitas: %d | Jadwal: %s\n",
               i + 1,
               foundBuses[i]->Info.idBus,
               foundBuses[i]->Info.namaSupir,
               foundBuses[i]->Info.kelas,
               foundBuses[i]->Info.kapasitas,
               waktuStr);
    }

    int pilihan;
    printf("Pilih bus [1-%d]: ", foundCount);
    scanf("%d", &pilihan);
    if (pilihan < 1 || pilihan > foundCount) {
        printf("Pilihan tidak valid.\n");
        return;
    }

    NodeBus* selectedBus = foundBuses[pilihan - 1];

    // Buat data tiket
    DataTiket tiketBaru;
    sprintf(tiketBaru.idTiket, "TK%ld", time(NULL));
    strcpy(tiketBaru.namaPenumpang, user->Info.nama);
    strcpy(tiketBaru.tujuan, tujuan);
    strftime(tiketBaru.jadwal, sizeof(tiketBaru.jadwal), "%Y-%m-%d %H:%M", localtime(&selectedBus->Info.keberangkatan));
    tiketBaru.noKursi = rand() % selectedBus->Info.kapasitas + 1;
    tiketBaru.jumlahKursi = jumlahKursi;
    tiketBaru.totalHarga = hargaPerKursi * jumlahKursi;
    strcpy(tiketBaru.status, "confirmed");

    insertTiket(tiketBaru, selectedBus);
    simpanTiketKeFile(tiketBaru);
    cetakTiket(tiketBaru.idTiket);
}



int deleteTiketByID(char idTiket[]) {
    NodeTiket *current = HeadTiket;
    NodeTiket *prev = NULL;

    while (current != NULL) {
        if (strcmp(current->Info.idTiket, idTiket) == 0) {
            if (prev == NULL) {
                HeadTiket = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            printf("Tiket dengan ID %s berhasil dihapus.\n", idTiket);
            return 1;
        }
        prev = current;
        current = current->next;
    }

    printf("Tiket dengan ID %s tidak ditemukan.\n", idTiket);
    return 0;
}


void deleteAllTiket() {
    NodeTiket* current = HeadTiket;
    while (current != NULL) {
        NodeTiket* temp = current;
        current = current->next;
        free(temp);
    }
    HeadTiket = NULL;
    printf("Semua tiket telah dihapus.\n");
}

void batalkanTiket(char idTiket[]) {
    NodeTiket* tiket = searchTiketByID(idTiket);
    if (tiket == NULL) {
        printf("Tiket tidak ditemukan.\n");
        return;
    }

    if (strcmp(tiket->Info.status, "cancelled") == 0) {
        printf("Tiket sudah dibatalkan sebelumnya.\n");
        return;
    }

    strcpy(tiket->Info.status, "cancelled");
    printf("Tiket dengan ID %s berhasil dibatalkan.\n", idTiket);

    // Rewrite all tickets to tiket.txt with the updated status
    FILE* f = fopen("tiket.txt", "w");
    if (!f) {
        printf("Gagal membuka file tiket.txt untuk menulis ulang.\n");
        return;
    }

    NodeTiket* current = HeadTiket;
    while (current != NULL) {
        fprintf(f, "%s|%s|%s|%s|%d|%d|%d|%s\n",
            current->Info.idTiket,
            current->Info.namaPenumpang,
            current->Info.tujuan,
            current->Info.jadwal,
            current->Info.noKursi,
            current->Info.jumlahKursi,
            current->Info.totalHarga,
            current->Info.status
        );
        current = current->next;
    }

    fclose(f);
}



void simpanTiketKeFile(DataTiket tiket) {
    FILE* f = fopen("tiket.txt", "a");
    if (!f) {
        printf("Gagal membuka file tiket.txt\n");
        return;
    }

    fprintf(f, "%s|%s|%s|%s|%d|%d|%d|%s\n",
        tiket.idTiket,
        tiket.namaPenumpang,
        tiket.tujuan,
        tiket.jadwal,
        tiket.noKursi,
        tiket.jumlahKursi,
        tiket.totalHarga,
        tiket.status
    );

    fclose(f);
}


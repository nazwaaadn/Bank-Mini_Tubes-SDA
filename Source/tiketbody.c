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

// =========================================
void muatDataBus() {
    FILE *file = fopen("FileManajemen/dataBus.txt", "r");
    if (!file) {
        printf("Gagal membuka FileManajemen/dataBus.txt\n");
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        // Buat variabel sementara untuk baca dari file
        char idBus[20], platNomor[30], namaSupir[50], ruteStr[200], waktuBrk[6], waktuTba[6];
        int kapasitas;
        char kelas;

        // Hapus newline di akhir line
        line[strcspn(line, "\r\n")] = '\0';

        // int jumlahField = sscanf(line, "%19[^|]|%29[^|]|%49[^|]|%d|%c|%199[^|]|%5[^|]|%5[^|]",
        //                          idBus, platNomor, namaSupir, &kapasitas, &kelas, ruteStr, waktuBrk, waktuTba);
        // if (jumlahField != 8) {
        //     printf("Format data bus salah: %s\n", line);
        //     continue;  // Lewati baris yang rusak
        // }

        // Buat DataBus baru dan isi data
        DataBus bus;
        strcpy(bus.idBus, idBus);
        strcpy(bus.platNomor, platNomor);
        strcpy(bus.namaSupir, namaSupir);
        bus.kapasitas = kapasitas;
        bus.kelas = kelas;

        // Konversi rute string ke linked list rute
        bus.rute = stringToRute(ruteStr);

        // Parse waktu keberangkatan
        int jam, menit;
        struct tm waktu = {0};
        sscanf(waktuBrk, "%d:%d", &jam, &menit);
        waktu.tm_year = 123;  // tahun 2023 (tahun sejak 1900)
        waktu.tm_mon = 0;
        waktu.tm_mday = 1;
        waktu.tm_hour = jam;
        waktu.tm_min = menit;
        bus.keberangkatan = mktime(&waktu);

        // Parse waktu kedatangan
        sscanf(waktuTba, "%d:%d", &jam, &menit);
        waktu.tm_hour = jam;
        waktu.tm_min = menit;
        bus.kedatangan = mktime(&waktu);

        // Alokasi node bus baru
        NodeBus *baru = alokasiNodeBus(bus);
        if (baru == NULL) {
            printf("Gagal alokasi memori untuk bus %s\n", idBus);
            continue;
        }

        // Masukkan node baru di depan list HeadBus
        baru->next = HeadBus;
        HeadBus = baru;
    }

    fclose(file);
}

//=====================================================================

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
    printf("| Dari         : %-26s |\n", tiket->Info.awal); 
    printf("| Tujuan       : %-26s |\n", tiket->Info.tujuan);
    printf("| Jadwal       : %-26s |\n", tiket->Info.jadwal);
    printf("| ID Bus       : %-26s |\n", tiket->bus ? tiket->bus->Info.idBus : "N/A");
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
        printf("| Dari         : %-26s |\n", current->Info.awal); 
        printf("| Tujuan       : %-26s |\n", current->Info.tujuan);
        printf("| Jadwal       : %-26s |\n", current->Info.jadwal);
        printf("| ID Bus       : %-26s |\n", current->bus ? current->bus->Info.idBus : "N/A");
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

    char awal[50], tujuan[50], jadwalStr[25];
    char idBus[10];
    struct tm tm_jadwal = {0};
    int jumlahKursi;
    int hargaPerKursi = 50000;

    printf("Masukkan ID Bus (misal: Bus1): ");
    scanf(" %[^\n]", idBus);
    idBus[strcspn(idBus, "\n")] = '\0'; // Bersihkan newline

    printf("Masukkan Lokasi Awal (terminal)       : ");
    scanf(" %[^\n]", awal);
    awal[strcspn(awal, "\n")] = '\0';

    printf("Masukkan Lokasi Tujuan (terminal)     : ");
    scanf(" %[^\n]", tujuan);
    tujuan[strcspn(tujuan, "\n")] = '\0';

    printf("Masukkan jadwal (YYYY-MM-DD HH:MM): ");
    scanf(" %[^\n]", jadwalStr);
    jadwalStr[strcspn(jadwalStr, "\n")] = '\0';

    printf("Jumlah kursi dipesan  : ");
    scanf("%d", &jumlahKursi);

    // Cari bus berdasarkan ID
    NodeBus* selectedBus = NULL;
    NodeBus* current = HeadBus;

    while (current != NULL) {
        printf("[DEBUG] Cek ID Bus: '%s'\n", current->Info.idBus);
        if (strcmp(current->Info.idBus, idBus) == 0) {
            selectedBus = current;
            break;
        }
        current = current->next;
    }

    if (selectedBus == NULL) {
        printf("Bus dengan ID %s tidak ditemukan.\n", idBus);
        return;
    }

    // Buat data tiket baru
    DataTiket tiketBaru;
    sprintf(tiketBaru.idTiket, "TK%ld", time(NULL));
    strcpy(tiketBaru.namaPenumpang, user->Info.nama);
    strcpy(tiketBaru.awal, awal);
    strcpy(tiketBaru.tujuan, tujuan);     // berdasarkan input user
    strcpy(tiketBaru.jadwal, jadwalStr);  // berdasarkan input user
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


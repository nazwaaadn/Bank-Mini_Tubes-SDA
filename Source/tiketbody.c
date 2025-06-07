#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "tiket.h"
#include "bus.h"
#include "terminal.h"
#include <math.h>
int lastTiketID = 0;


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

        int jumlahField = sscanf(line, "%19[^|]|%29[^|]|%49[^|]|%d|%c|%199[^|]|%5[^|]|%5[^|]",
                                 idBus, platNomor, namaSupir, &kapasitas, &kelas, ruteStr, waktuBrk, waktuTba);
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
//=====================================================================

NodeTiket *HeadTiket = NULL;

int isTiketListEmpty() {
    return HeadTiket == NULL;
}



int insertTiket(DataTiket tiketBaru, NodeBus* bus) {
    NodeTiket* newNode = (NodeTiket*)malloc(sizeof(NodeTiket));
    if (!newNode) {
        printf("Gagal mengalokasikan memori untuk tiket.\n");
        return -1;
    }

    lastTiketID++;
    tiketBaru.idTiket = lastTiketID;

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
    return tiketBaru.idTiket;
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


void cetakTiket(int idTiket, char* namaPenumpang) {
    loadTiketDariFile();
    NodeTiket* tiket = searchTiketByID(idTiket, (char*)namaPenumpang);

    if (tiket == NULL) {
        printf("Tiket dengan ID %d tidak ditemukan.\n", idTiket);
        return;
    }

    // Handle null bus pointer
    char idBusDisplay[30];
    if (tiket->bus != NULL) {
        strcpy(idBusDisplay, tiket->bus->Info.idBus);
    } else {
        strcpy(idBusDisplay, tiket->Info.idBus); // fallback ke string idBus
    }

    printf("\n+===========================================+\n");
    printf("|          TIKET BUS TRANSJAKARTA           |\n");
    printf("+===========================================+\n");
    printf("| ID Tiket     : %-26d |\n", tiket->Info.idTiket);
    printf("| Nama         : %-26s |\n", tiket->Info.namaPenumpang);
    printf("| Dari         : %-26s |\n", tiket->Info.awal); 
    printf("| Tujuan       : %-26s |\n", tiket->Info.tujuan);
    printf("| ID Bus       : %-26s |\n", idBusDisplay);
    printf("| Jumlah Kursi : %-26d |\n", tiket->Info.jumlahKursi);
    printf("| Total Harga  : Rp%-24d |\n", tiket->Info.totalHarga);
    printf("| Status       : %-26s |\n", tiket->Info.status);
    printf("+===========================================+\n");
    printf("|     TERIMA KASIH TELAH MEMESAN TIKET!     |\n");
    printf("+===========================================+\n");
}




void printAllTiket() {
    // Load tickets from file before printing
    FILE* f = fopen("tiket.txt", "r");
    if (!f) {
        printf("Gagal membuka file tiket.txt\n");
        return;
    }

    // Clear existing list if necessary (optional safety)
    NodeTiket* current = HeadTiket;
    while (current) {
        NodeTiket* temp = current;
        current = current->next;
        free(temp);
    }
    HeadTiket = NULL;

    char line[512];
    while (fgets(line, sizeof(line), f)) {
        DataTiket tiket;

        sscanf(line, "%d|%[^|]|%[^|]|%[^|]|%[^|]|%d|%d|%d|%s",
            &tiket.idTiket,
            tiket.namaPenumpang,
            tiket.awal,
            tiket.tujuan,
            tiket.idBus, 
            &tiket.noKursi,
            &tiket.jumlahKursi,
            &tiket.totalHarga,
            tiket.status
        );

        NodeTiket* newNode = (NodeTiket*)malloc(sizeof(NodeTiket));
        if (!newNode) {
            printf("Gagal mengalokasikan memori untuk tiket.\n");
            continue;
        }

        newNode->Info = tiket;
        newNode->bus = NULL; // Hubungan dengan struct Bus tidak dibangun ulang di sini
        newNode->next = HeadTiket;
        HeadTiket = newNode;
    }

    fclose(f);

    // Now print all loaded tickets
    if (isTiketListEmpty()) {
        printf("Tidak ada tiket yang tersedia.\n");
        return;
    }

    current = HeadTiket;
    printf("\n=== DAFTAR SEMUA TIKET ===\n");
    while (current != NULL) {
        printf("\n+===========================================+\n");
        printf("|          TIKET BUS TRANSJAKARTA           |\n");
        printf("+===========================================+\n");
        printf("| ID Tiket     : %-26d |\n", current->Info.idTiket);
        printf("| Nama         : %-26s |\n", current->Info.namaPenumpang);
        printf("| Dari         : %-26s |\n", current->Info.awal); 
        printf("| Tujuan       : %-26s |\n", current->Info.tujuan);
        printf("| ID Bus       : %-26s |\n", current->Info.idBus ? current->bus->Info.idBus : "N/A");
        printf("| Jumlah Kursi : %-26d |\n", current->Info.jumlahKursi);
        printf("| Total Harga  : Rp%-24d |\n", current->Info.totalHarga);
        printf("| Status       : %-26s |\n", current->Info.status);
        printf("+===========================================+\n");
        
        current = current->next;
    }
}



NodeTiket* searchTiketByID(int idTiket, char* namaPenumpang) {

    NodeTiket* temp = HeadTiket;
    while (temp != NULL) {
        if (temp->Info.idTiket == idTiket &&
            strcmp(temp->Info.namaPenumpang, namaPenumpang) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}


// Fungsi simulasi pemesanan tiket oleh user
void pesanTiket(NodeUser* user) {
    if (user == NULL) {
        printf("Anda belum login.\n");
        return;
    }

    // Pastikan lastTiketID sesuai data tiket.txt
    updateLastTiketIDFromFile();

    char awal[50], tujuan[50], jadwalStr[25];
    struct tm tm_jadwal = {0};
    int jumlahKursi;
    int hargaPerKursi = 50000;
    int idTiket;

    printf("=== PEMESANAN TIKET ===\n");

    printf("Masukkan Lokasi Awal (terminal)       : ");
    scanf(" %[^\n]", awal);
    awal[strcspn(awal, "\n")] = '\0';

    printf("Masukkan Lokasi Tujuan (terminal)     : ");
    scanf(" %[^\n]", tujuan);
    tujuan[strcspn(tujuan, "\n")] = '\0';

    printf("Jumlah kursi dipesan  : ");
    scanf("%d", &jumlahKursi);


    NodeBus* current = HeadBus;
    NodeBus* busList[100]; // simpan pointer ke bus yang cocok
    int count = 0;

    printf("\nBus yang tersedia untuk rute %s -> %s:\n", awal, tujuan);
    printf("---------------------------------------------------------------------------------------------------------\n");
    printf("| ID Bus | Plat Nomor     | Nama Supir         | Terminal Awal      | Terminal Tujuan    | Kapasitas |\n");
    printf("---------------------------------------------------------------------------------------------------------\n");

    while (current != NULL) {

        NodeRute* ruteNode = current->Info.rute;
        int foundAwal = 0, foundTujuan = 0;
        int indexAwal = -1, indexTujuan = -1;
        int index = 0;

                
        while (ruteNode != NULL) {
            if (strcmp(ruteNode->namaTerminal, awal) == 0 && !foundAwal) {
                foundAwal = 1;
                indexAwal = index;
            }
            if (strcmp(ruteNode->namaTerminal, tujuan) == 0 && foundAwal && !foundTujuan) {
                foundTujuan = 1;
                indexTujuan = index;
                break; // tujuan ditemukan setelah awal, cukup
            }
            ruteNode = ruteNode->next;
            index++;
        }
        // Bandingkan dengan input
        if (foundAwal && foundTujuan && indexAwal < indexTujuan) {
            printf("| %-6s | %-15s | %-18s | %-18s | %-18s | %-9d |\n",
                current->Info.idBus, current->Info.platNomor, current->Info.namaSupir,
                awal, tujuan, current->Info.kapasitas);

            busList[count++] = current;
        }

        current = current->next;
    }

    if (count == 0) {
        printf("Maaf, tidak ada bus untuk rute tersebut.\n");
        return;
    }

    printf("---------------------------------------------------------------------------------------------------------\n");

    // Pilih ID Bus
    char idBus[10];
    printf("Masukkan ID Bus yang ingin Anda pilih: ");
    scanf(" %[^\n]", idBus);

    // Cari bus berdasarkan ID
    NodeBus* selectedBus = NULL;
    for (int i = 0; i < count; i++) {
        if (strcmp(busList[i]->Info.idBus, idBus) == 0) {
            selectedBus = busList[i];
            break;
        }
    }

    if (selectedBus == NULL) {
        printf("Bus dengan ID %s tidak ditemukan.\n", idBus);
        return;
    }

    if (jumlahKursi > selectedBus->Info.kapasitas) {
        printf("Jumlah kursi melebihi kapasitas bus.\n");
        return;
    }

    // Buat data tiket baru
    DataTiket tiketBaru;
    lastTiketID++; // Increments ticket id for new ticket
    tiketBaru.idTiket = lastTiketID; 
    strcpy(tiketBaru.namaPenumpang, user->Info.nama);
    strcpy(tiketBaru.awal, awal);
    strcpy(tiketBaru.tujuan, tujuan);     // berdasarkan input user
    strcpy(tiketBaru.idBus, idBus);         // ID Bus yang dipilih
    tiketBaru.noKursi = rand() % selectedBus->Info.kapasitas + 1;
    tiketBaru.jumlahKursi = jumlahKursi;
    tiketBaru.totalHarga = hargaPerKursi * jumlahKursi;
    strcpy(tiketBaru.status, "confirmed");


    insertTiket(tiketBaru, selectedBus);
    simpanTiketKeFile(tiketBaru);
    printf("+===========================================+\n");
    printf("|          TIKET BUS TRANSJAKARTA           |\n");
    printf("+===========================================+\n");
    printf("| ID Tiket     : %-26d |\n", tiketBaru.idTiket);
    printf("| Nama         : %-26s |\n", tiketBaru.namaPenumpang);
    printf("| Dari         : %-26s |\n", tiketBaru.awal);
    printf("| Tujuan       : %-26s |\n", tiketBaru.tujuan);
    printf("| ID Bus       : %-26s |\n", tiketBaru.idBus);
    printf("| Jumlah Kursi : %-26d |\n", tiketBaru.jumlahKursi);
    printf("| Total Harga  : Rp%-24d |\n", tiketBaru.totalHarga);
    printf("| Status       : %-26s |\n", tiketBaru.status);
    printf("+===========================================+\n");
    printf("|     TERIMA KASIH TELAH MEMESAN TIKET!     |\n");
    printf("+===========================================+\n");

    printf("Tiket berhasil dipesan untuk %s sebanyak %d kursi.\nTotal harga: %d\n",
       selectedBus->Info.idBus, jumlahKursi, tiketBaru.totalHarga);
}




int deleteTiketByID(int idTiket) {
    NodeTiket *current = HeadTiket;
    NodeTiket *prev = NULL;

    while (current != NULL) {
        if (current->Info.idTiket == idTiket) {
            if (prev == NULL) {
                HeadTiket = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            printf("Tiket dengan ID %d berhasil dihapus.\n", idTiket);
            return 1;
        }
        prev = current;
        current = current->next;
    }

    printf("Tiket dengan ID %d tidak ditemukan.\n", idTiket);
    return 0;
}



// void deleteAllTiket() {
//     NodeTiket* current = HeadTiket;
//     while (current != NULL) {
//         NodeTiket* temp = current;
//         current = current->next;
//         free(temp);
//     }
//     HeadTiket = NULL;
//     printf("Semua tiket telah dihapus.\n");
// }

void batalkanTiket(int idTiket, char* namaPenumpang) {
    NodeTiket* tiket = searchTiketByID(idTiket, (char*)namaPenumpang);
    if (tiket == NULL) {
        printf("Tiket dengan ID %d tidak ditemukan.\n", idTiket);
        return;
    }

    if (strcmp(tiket->Info.status, "cancelled") == 0) {
        printf("Tiket sudah dibatalkan sebelumnya.\n");
        return;
    }

    strcpy(tiket->Info.status, "cancelled");
    printf("Tiket dengan ID %d berhasil dibatalkan.\n", idTiket);

    // Rewrite all tickets to tiket.txt with the updated status
    FILE* f = fopen("tiket.txt", "w");
    if (!f) {
        printf("Gagal membuka file tiket.txt untuk menulis ulang.\n");
        return;
    }

    NodeTiket* current = HeadTiket;
    while (current != NULL) {
        fprintf(f, "%d|%s|%s|%s|%d|%d|%d|%s\n",
            current->Info.idTiket,
            current->Info.namaPenumpang,
            current->Info.tujuan,
            current->Info.idBus,
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
    FILE *f = fopen("tiket.txt", "a");
    if (!f) {
        printf("Gagal membuka file tiket.txt untuk menyimpan.\n");
        return;
    }

    fprintf(f, "%d|%s|%s|%s|%s|%d|%d|%d|%s\n",
        tiket.idTiket,
        tiket.namaPenumpang,
        tiket.awal,
        tiket.tujuan,
        tiket.idBus,
        tiket.noKursi,
        tiket.jumlahKursi,
        tiket.totalHarga,
        tiket.status
    );

    fclose(f);
}


void updateLastTiketIDFromFile() {
    FILE *file = fopen("tiket.txt", "r");
    if (!file) return;

    char line[512];
    int maxID = 0;
    while (fgets(line, sizeof(line), file)) {
        int id;
        sscanf(line, "%d|", &id);
        if (id > maxID) maxID = id;
    }

    lastTiketID = maxID;
    fclose(file);
}

void loadTiketDariFile() {
    FILE* file = fopen("tiket.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file tiket.txt\n");
        return;
    }

    HeadTiket = NULL;  // reset linked list sebelum load baru
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        NodeTiket* newNode = (NodeTiket*)malloc(sizeof(NodeTiket));
        if (newNode == NULL) {
            printf("Gagal mengalokasikan memori.\n");
            fclose(file);
            return;
        }

        sscanf(line, "%d|%[^|]|%[^|]|%[^|]|%[^|]|%d|%d|%d|%s",
               &newNode->Info.idTiket,
               newNode->Info.namaPenumpang,
               newNode->Info.awal,
               newNode->Info.tujuan,
               newNode->Info.idBus,
               &newNode->Info.noKursi,
               &newNode->Info.jumlahKursi,
               &newNode->Info.totalHarga,
               newNode->Info.status);

        newNode->next = HeadTiket;
        newNode->bus = NULL;  // optional: bisa dipasang nanti
        HeadTiket = newNode;
    }

    fclose(file);
}




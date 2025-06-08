#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Header/user.h"
#include "../Header/tiket.h"
#include "../Header/bus.h"
#include "../Header/terminal.h"
#include <math.h>
#include <ctype.h>

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
        char idBus[20], platNomor[30], namaSupir[50], ruteStr[200], waktuBrk[6], waktuTba[6];
        int kapasitas;
        char kelas;

        line[strcspn(line, "\r\n")] = '\0';

        int jumlahField = sscanf(line, " %19[^|] | %29[^|] | %49[^|] | %d | %c | %199[^|] | %5[^|] | %5[^|]",
                                 idBus, platNomor, namaSupir, &kapasitas, &kelas,
                                 ruteStr, waktuBrk, waktuTba);
        if (jumlahField != 8) continue;

        // Hapus spasi depan & belakang idBus (dan field lain kalau mau)
        int start = 0;
        while (isspace((unsigned char)idBus[start])) start++;
        if (start > 0) memmove(idBus, idBus + start, strlen(idBus + start) + 1);
        for (int i = strlen(idBus) - 1; i >= 0 && isspace((unsigned char)idBus[i]); --i)
            idBus[i] = '\0';

        DataBus bus;
        strcpy(bus.idBus, idBus);
        strcpy(bus.platNomor, platNomor);
        strcpy(bus.namaSupir, namaSupir);
        bus.kapasitas = kapasitas;
        bus.kelas = kelas;
        bus.rute = stringToRute(ruteStr);

        int jam, menit;
        struct tm waktu = {0};
        sscanf(waktuBrk, "%d:%d", &jam, &menit);
        waktu.tm_year = 123;
        waktu.tm_mon = 0;
        waktu.tm_mday = 1;
        waktu.tm_hour = jam;
        waktu.tm_min = menit;
        bus.keberangkatan = mktime(&waktu);

        sscanf(waktuTba, "%d:%d", &jam, &menit);
        waktu.tm_hour = jam;
        waktu.tm_min = menit;
        bus.kedatangan = mktime(&waktu);

        NodeBus *baru = alokasiNodeBus(bus);
        if (!baru) {
            printf("Gagal alokasi memori untuk bus %s\n", idBus);
            continue;
        }

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


void cetakTiket(char idTiket[]) {
    NodeTiket* tiket = searchTiketByID(idTiket);
    if (!tiket) {
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
    printf("| ID Bus       : %-26s |\n", tiket->bus ? tiket->bus->Info.idBus : tiket->Info.idBus);
    printf("| Status       : %-26s |\n", tiket->Info.status);
    printf("+===========================================+\n");
    printf("|     TERIMA KASIH TELAH MEMESAN TIKET!     |\n");
    printf("+===========================================+\n");
}



void printAllTiket() {
    FILE* f = fopen("tiket.txt", "r");
    if (!f) {
        printf("Gagal membuka file tiket.txt\n");
        return;
    }

    char line[256];
    char idTiket[20], namaPenumpang[50], awal[50], tujuan[50], idBus[20], status[20];
    int count = 0;

    printf("\n+=========== Daftar Tiket ===========+\n");

    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]",
                   idTiket, namaPenumpang, awal, tujuan, idBus, status) == 6) {

            // Tampilkan hanya yang aktif (bisa juga pakai strcasecmp jika ingin bebas huruf besar/kecil)
                printf("\n+============================+\n");
                printf(" ID Tiket  : %s\n", idTiket);
                printf(" Nama      : %s\n", namaPenumpang);
                printf(" Dari      : %s\n", awal);
                printf(" Tujuan    : %s\n", tujuan);
                printf(" Bus       : %s\n", idBus);
                printf(" Status    : %s\n", status);
                printf("+============================+\n");
        }
    }

    fclose(f);
}

NodeBus* searchBusByIDFromFile(const char* idTargetRaw) {
    FILE *file = fopen("FileManajemen/dataBus.txt", "r");
    if (!file) {
        printf("Gagal membuka FileManajemen/dataBus.txt\n");
        return NULL;
    }

    char idTarget[20];
    strncpy(idTarget, idTargetRaw, sizeof(idTarget));
    idTarget[sizeof(idTarget) - 1] = '\0';

    // Hapus spasi belakang
    for (int i = strlen(idTarget) - 1; i >= 0 && isspace((unsigned char)idTarget[i]); --i)
        idTarget[i] = '\0';
    // Hapus spasi depan
    int start3 = 0;
    while (isspace((unsigned char)idTarget[start3])) start3++;
    if (start3 > 0) memmove(idTarget, idTarget + start3, strlen(idTarget + start3) + 1);

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        char idBus[20], platNomor[30], namaSupir[50], ruteStr[200], waktuBrk[6], waktuTba[6];
        int kapasitas;
        char kelas;

        line[strcspn(line, "\r\n")] = '\0';

        int jumlahField = sscanf(line, " %19[^|] | %29[^|] | %49[^|] | %d | %c | %199[^|] | %5[^|] | %5[^|]",
                                 idBus, platNomor, namaSupir, &kapasitas, &kelas,
                                 ruteStr, waktuBrk, waktuTba);
        if (jumlahField != 8) continue;

        // Hapus spasi depan & belakang idBus
        int start2 = 0;
        while (isspace((unsigned char)idBus[start2])) start2++;
        if (start2 > 0) memmove(idBus, idBus + start2, strlen(idBus + start2) + 1);
        for (int i = strlen(idBus) - 1; i >= 0 && isspace((unsigned char)idBus[i]); --i)
            idBus[i] = '\0';

        if (strcmp(idBus, idTarget) == 0) {
            DataBus bus;
            strcpy(bus.idBus, idBus);
            strcpy(bus.platNomor, platNomor);
            strcpy(bus.namaSupir, namaSupir);
            bus.kapasitas = kapasitas;
            bus.kelas = kelas;
            bus.rute = stringToRute(ruteStr);

            struct tm waktu = {0};
            int jam, menit;
            sscanf(waktuBrk, "%d:%d", &jam, &menit);
            waktu.tm_year = 123;
            waktu.tm_mon = 0;
            waktu.tm_mday = 1;
            waktu.tm_hour = jam;
            waktu.tm_min = menit;
            bus.keberangkatan = mktime(&waktu);

            sscanf(waktuTba, "%d:%d", &jam, &menit);
            waktu.tm_hour = jam;
            waktu.tm_min = menit;
            bus.kedatangan = mktime(&waktu);

            NodeBus* node = alokasiNodeBus(bus);
            fclose(file);
            return node;
        }
    }

    fclose(file);
    return NULL;
}



// Fungsi simulasi pemesanan tiket oleh user
void pesanTiket(NodeUser* user) {
    char idBus[20];
    char awal[50], tujuan[50];
    
    printf("\nMasukkan ID Bus yang ingin dipesan: ");
    fgets(idBus, sizeof(idBus), stdin);
    idBus[strcspn(idBus, "\r\n")] = '\0';  // Bersihkan newline dan trailing space

    // Meminta input terminal awal dan tujuan
    printf("Masukkan terminal awal: ");
    fgets(awal, sizeof(awal), stdin);
    awal[strcspn(awal, "\r\n")] = '\0';

    printf("Masukkan terminal tujuan: ");
    fgets(tujuan, sizeof(tujuan), stdin);
    tujuan[strcspn(tujuan, "\r\n")] = '\0';

    NodeBus* bus = searchBusByIDFromFile(idBus);
    if (bus == NULL) {
        printf("Bus dengan ID '%s' tidak ditemukan.\n", idBus);
        return;
    }

    DataTiket tiketBaru;
    snprintf(tiketBaru.idTiket, sizeof(tiketBaru.idTiket), "TKT%ld", time(NULL));
    strcpy(tiketBaru.idBus, bus->Info.idBus);
    strcpy(tiketBaru.namaPenumpang, user->Info.nama);
    strcpy(tiketBaru.awal, awal);  // Gunakan input terminal awal
    strcpy(tiketBaru.tujuan, tujuan);  // Gunakan input terminal tujuan
    strcpy(tiketBaru.status, "aktif");

    insertTiket(tiketBaru, bus);
    simpanTiketKeFile(tiketBaru);

    printf("Tiket berhasil dipesan!\n");
    printf("ID Tiket: %s\n", tiketBaru.idTiket);
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

void batalkanTiket(const char* idTiket) {
    FILE* in = fopen("tiket.txt", "r");
    FILE* out = fopen("tiket_temp.txt", "w");
    if (!in || !out) {
        printf("Gagal membuka file.\n");
        return;
    }

    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), in)) {
        DataTiket t;
        sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]",
            t.idTiket, t.namaPenumpang, t.awal, t.tujuan, t.idBus, t.status);

        if (strcmp(t.idTiket, idTiket) == 0) {
            strcpy(t.status, "cancelled");
            found = 1;
        }

        fprintf(out, "%s|%s|%s|%s|%s|%s\n",
            t.idTiket, t.namaPenumpang, t.awal, t.tujuan, t.idBus, t.status);
    }

    fclose(in);
    fclose(out);

    remove("tiket.txt");
    rename("tiket_temp.txt", "tiket.txt");

    if (found)
        printf("Tiket %s berhasil dibatalkan.\n", idTiket);
    else
        printf("Tiket tidak ditemukan.\n");
}



void simpanTiketKeFile(DataTiket tiket) {
    FILE* f = fopen("tiket.txt", "a");
    if (!f) {
        printf("Gagal membuka file tiket.txt\n");
        return;
    }
    fprintf(f, "%s|%s|%s|%s|%s|%s\n",
        tiket.idTiket,
        tiket.namaPenumpang,
        tiket.awal,
        tiket.tujuan,
        tiket.idBus,
        tiket.status);
    fclose(f);
}



//tambahan
void printTiketAktifByUser() {
    if (currentUser == NULL) {
        printf("Anda belum login.\n");
        return;
    }

    FILE* f = fopen("tiket.txt", "r");
    if (!f) {
        printf("Gagal membuka file tiket.txt\n");
        return;
    }

    char idTiket[20], namaPenumpang[50], awal[50], tujuan[50], idBus[20], status[20];
    int found = 0;

    printf("\n--- Riwayat Tiket Anda (%s) ---\n", currentUser->Info.nama);

    printf("\n--- Tiket Aktif Anda ---\n");
    while (fscanf(f, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n",
                  idTiket, namaPenumpang, awal, tujuan, idBus, status) == 6) {
        if (strcmp(namaPenumpang, currentUser->Info.nama) == 0 &&
            strcmp(status, "aktif") == 0) {
            printf("ID Tiket : %s\n", idTiket);
            printf("Bus      : %s\n", idBus);
            printf("Dari     : %s\n", awal);
            printf("Tujuan   : %s\n", tujuan);
            printf("Status   : %s\n", status);
            printf("-----------------------------\n");
            found = 1;
        }
    }

    if (!found) {
        printf("Tidak ada tiket aktif.\n");
    }

    fclose(f);
}

void printAllTiketByUser() {
    if (currentUser == NULL) {
        printf("Anda belum login.\n");
        return;
    }

    FILE* f = fopen("tiket.txt", "r");
    if (!f) {
        printf("Gagal membuka file tiket.txt\n");
        return;
    }

    char idTiket[20], namaPenumpang[50], awal[50], tujuan[50], idBus[20], status[20];
    int found = 0;

    printf("\n--- Riwayat Tiket Anda (%s) ---\n", currentUser->Info.nama);

    while (fscanf(f, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n",
                  idTiket, namaPenumpang, awal, tujuan, idBus, status) == 6) {
        if (strcmp(namaPenumpang, currentUser->Info.nama) == 0) {
            printf("ID Tiket : %s\n", idTiket);
            printf("Bus      : %s\n", idBus);
            printf("Dari     : %s\n", awal);
            printf("Tujuan   : %s\n", tujuan);
            printf("Status   : %s\n", status);
            printf("-----------------------------\n");
            found = 1;
        }
    }

    if (!found) {
        printf("Tidak ada riwayat tiket.\n");
    }

    fclose(f);
}

void UserMenu(NodeUser* user)
{
    int pilihan;
    char buffer[10];
    char id[20];

    do {
        printf("===================================\n");
        printf("|        MENU UTAMA PELANGGAN     |\n");
        printf("===================================\n");
        printf("1. Pesan Tiket\n");
        printf("2. Lihat Tiket Aktif\n");
        printf("3. Riwayat Perjalanan\n");
        printf("4. Batalkan Tiket\n");
        printf("5. Keluar\n");
        printf("===================================\n");
        printf("Masukkan pilihan Anda: ");
        
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &pilihan) != 1) {
            printf("Input tidak valid.\n");
            continue;
        }

        switch (pilihan) {
            case 1:
                printAllBus();   // Menampilkan semua bus
                pesanTiket(user); // Fungsi untuk memesan tiket
                break;
            case 2:
                printTiketAktifByUser(); // Menampilkan tiket aktif pengguna
                break;
            case 3:
                printAllTiketByUser();   // Menampilkan riwayat tiket
                break;
            case 4:
                printf("Masukkan ID Tiket yang ingin dibatalkan: ");
                fgets(id, sizeof(id), stdin);
                id[strcspn(id, "\n")] = '\0';
                batalkanTiket(id);   // Membatalkan tiket berdasarkan ID
                break;
            case 5:
                printf("Keluar dari menu pelanggan.\n");
                return;  // Keluar dari menu pelanggan dan kembali ke menu utama
            default:
                printf("Pilihan tidak valid.\n");
        }
    } while (1); // Loop terus hingga pengguna memilih untuk keluar
}

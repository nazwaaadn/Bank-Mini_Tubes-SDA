#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Header/user.h"
#include "../Header/tiket.h"
#include "../Header/bus.h"
#include "../Header/terminal.h"
#include "../Header/stack.h"
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
        bus.rute = strToRute(ruteStr);

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



void printAllTiket() {
    FILE* f = fopen("FileManajemen/tiket.txt", "r");
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

    // NodeBus* bus = searchBusByIDFromFile(idBus);
    // if (bus == NULL) {
    //     printf("Bus dengan ID '%s' tidak ditemukan.\n", idBus);
    //     return;
    // }

    DataTiket tiketBaru;
    snprintf(tiketBaru.idTiket, sizeof(tiketBaru.idTiket), "TKT%ld", time(NULL));
    strcpy(tiketBaru.idBus, idBus);
    strcpy(tiketBaru.namaPenumpang, currentUser->Info.nama);
    strcpy(tiketBaru.awal, awal);  // Gunakan input terminal awal
    strcpy(tiketBaru.tujuan, tujuan);  // Gunakan input terminal tujuan
    strcpy(tiketBaru.status, "aktif");

    // insertTiket(tiketBaru, bus);
    simpanTiketKeFile(tiketBaru);

    printf("Tiket berhasil dipesan!\n");
    printf("ID Tiket: %s\n", tiketBaru.idTiket);
}

void batalkanTiket(const char* idTiket) {
    FILE* in = fopen("FileManajemen/tiket.txt", "r");
    FILE* out = fopen("FileManajemen/tiket_temp.txt", "w");
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

    remove("FileManajemen/tiket.txt");
    rename("FileManajemen/tiket_temp.txt", "FileManajemen/tiket.txt");

    if (found)
        printf("Tiket %s berhasil dibatalkan.\n", idTiket);
    else
        printf("Tiket tidak ditemukan.\n");
}

void simpanTiketKeFile(DataTiket tiket) {
    FILE* f = fopen("FileManajemen/tiket.txt", "a");
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

    FILE* f = fopen("FileManajemen/tiket.txt", "r");
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

    FILE* f = fopen("FileManajemen/tiket.txt", "r");
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

NodeUser* user = NULL;
void UserMenu(NodeUser* user) {
    int pilihan;
    char buffer[10];
    char id[20];
    DataBus buses[10];  // Array untuk menampung data bus
    DataTiket tiket[100];  // Array untuk menampung data tiket
    int tiketCount = 0, busCount = 0;

    // Load bus data
    bacaDataBus("FileManajemen/dataBus.txt", buses, &busCount);
    
    // Load tiket data
    bacaDataTiket("FileManajemen/tiket.txt", tiket, &tiketCount);

    do {
        printf("===================================\n");
        printf("|        MENU UTAMA PELANGGAN     |\n");
        printf("===================================\n");
        printf("1. Pesan Tiket\n");
        printf("2. Lihat Tiket Aktif\n");
        printf("3. Lihat Semua Tiket\n");
        printf("4. Batalkan Tiket\n");
        printf("5. Perjalanan Anda\n");
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
                printf("Simulasi perjalanan Anda:\n");
                user = currentUser;
                printf("Memulai simulasi perjalanan untuk pengguna: %s\n", user->Info.nama);

                const char* idBus = cariIdBusByUser(user, tiket, tiketCount);

                if (idBus == NULL) {
                    printf("Tiket aktif tidak ditemukan untuk pengguna %s.\n", currentUser->Info.nama);
                    return;
                }

                // Mencari bus yang sesuai dengan idBus
                DataBus* bus = NULL;
                for (int i = 0; i < busCount; i++) {
                    if (strcmp(buses[i].idBus, idBus) == 0) {
                        bus = &buses[i];
                        break;
                    }
                }

                if (bus == NULL) {
                    printf("Bus dengan ID %s tidak ditemukan.\n", idBus);
                    return;
                }

                // Melanjutkan simulasi perjalanan dengan bus yang ditemukan
                simulasiPerjalananUser(bus, tiket, tiketCount);  // Start simulation
                break;
            case 6:
                printf("Terima kasih telah menggunakan layanan kami!\n");
                return; // Keluar dari menu pelanggan
            default:
                printf("Pilihan tidak valid.\n");
        }
    } while (1); // Loop terus hingga pengguna memilih untuk keluar
}

const char* cariIdBusByUser(NodeUser* currentUser, DataTiket* tiket, int tiketCount) {
    
    for (int i = 0; i < tiketCount; i++) {
        // Debugging untuk memastikan tiket yang dicocokkan
        
        // Jika nama penumpang sesuai dan status tiket aktif
        if (strcmp(tiket[i].namaPenumpang, currentUser->Info.nama) == 0 && 
            strcmp(tiket[i].status, "aktif") == 0) {
            return tiket[i].idBus;  // Mengembalikan ID Bus dari tiket yang ditemukan
        }
    }

    return NULL;  // Jika tidak ditemukan tiket aktif untuk pengguna
}


void simulasiPerjalananUser(DataBus* buses, DataTiket* tiket, int tiketCount) {
    user = currentUser;
    const char* idBus = cariIdBusByUser(user, tiket, tiketCount);

    if (idBus == NULL) {
        printf("Tidak ada tiket aktif untuk pengguna %s.\n", user->Info.nama);
        return;
    }

    DataBus* bus = NULL;
    for (int i = 0; i < 10; i++) {
        if (strcmp(buses[i].idBus, idBus) == 0) {
            bus = &buses[i];
            break;
        }
    }

    if (bus == NULL) {
        printf("Bus dengan ID %s tidak ditemukan.\n", idBus);
        return;
    }

    if (bus->status != 1) {
        printf("Bus %s tidak dapat melakukan perjalanan karena status tidak aktif.\n", bus->idBus);
        return;
    }

    Stack terminalStack;
    initStack(&terminalStack);

    printf("Simulasi perjalanan bus ID: %s\n", bus->idBus);

    NodeRute* currentRute = bus->rute;
    int userReachedDestination = 0;

    while (currentRute != NULL) {
        // Kalau terminal mengandung "(pergi)", push ke stack
        if (strstr(currentRute->namaTerminal, "(pergi)") != NULL) {
            push(&terminalStack, currentRute->namaTerminal);
            printf("Terminal %s\n", currentRute->namaTerminal);
        }

        int penumpangNaik = 0;
        int penumpangTurun = 0;

        for (int i = 0; i < tiketCount; i++) {
            if (strcmp(tiket[i].idBus, bus->idBus) != 0 || strcmp(tiket[i].status, "aktif") != 0)
                continue;

            // Cek naik
            if (strcmp(tiket[i].awal, currentRute->namaTerminal) == 0) {
                penumpangNaik++;
                if (strcmp(tiket[i].namaPenumpang, currentUser->Info.nama) == 0)
                    printf("Anda naik di terminal %s.\n", currentRute->namaTerminal);
            }

            // Cek turun
            if (strcmp(tiket[i].tujuan, currentRute->namaTerminal) == 0) {
                penumpangTurun++;
                strcpy(tiket[i].status, "done");

                if (strcmp(tiket[i].namaPenumpang, currentUser->Info.nama) == 0) {
                    printf("Anda turun di terminal %s.\n", currentRute->namaTerminal);
                    userReachedDestination = 1;
                }
            }
        }

        printf("Bus %s berhenti di terminal %s, %d penumpang turun, %d penumpang naik.\n\n",
               bus->idBus, currentRute->namaTerminal, penumpangTurun, penumpangNaik);

        if (userReachedDestination) {
            printf("Perjalanan selesai. Anda sudah mencapai tujuan di terminal %s.\n", currentRute->namaTerminal);
            break;
        }

        // Jika terminal mengandung "(pulang)", pop dari stack
        if (strstr(currentRute->namaTerminal, "(pulang)") != NULL) {
            char* poppedTerminal = pop(&terminalStack);
            if (poppedTerminal) {
                printf("Terminal %s\n", poppedTerminal);
                free(poppedTerminal);
            }
        }

        currentRute = currentRute->next;
    }

    simpanDataTiketKeFile(tiket, tiketCount); // Simpan perubahan tiket ke file

    if (!userReachedDestination) {
        printf("Perjalanan selesai, bus mencapai tujuan akhir tanpa menemukan pengguna.\n");
    }
}

// Fungsi untuk membaca data tiket dari file
void bacaDataTiket(char* filename, DataTiket tiket[], int* tiketCount) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        // Abaikan baris kosong atau tidak valid
        if (line[0] == '*' || line[0] == '\n') continue;

        // Komponen data dari file
        char idTiket[20], namaPengguna[50], terminalNaik[50], terminalTurun[50], idBus[20], status[20];

        // Parsing dari baris file sesuai format yang diberikan
        int jumlahParsed = sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%s",
                                   idTiket, namaPengguna, terminalNaik, terminalTurun, idBus, status);

        if (jumlahParsed != 6) {
            printf("Baris tidak valid, dilewati: %s\n", line);
            continue;
        }

        // Simpan data tiket ke array tiket
        strcpy(tiket[*tiketCount].idTiket, idTiket);
        strcpy(tiket[*tiketCount].namaPenumpang, namaPengguna);
        strcpy(tiket[*tiketCount].awal, terminalNaik);
        strcpy(tiket[*tiketCount].tujuan, terminalTurun);
        strcpy(tiket[*tiketCount].idBus, idBus);
        strcpy(tiket[*tiketCount].status, status);

        (*tiketCount)++;
    }

    fclose(file);
}

// Fungsi untuk membaca data bus dari file
void bacaDataBus(char* filename, DataBus buses[], int* busCount) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '*' || line[0] == '\n') continue;

        // Buffer untuk parsing
        char idBus[10], platNomor[15], namaSupir[50], rute[512];
        char strKeberangkatan[10], strKedatangan[10], strStatus[5];
        int kapasitas;
        char kelas;

        int jumlahParsed = sscanf(line, "%[^|]|%[^|]|%[^|]|%d|%c|%[^|]|%[^|]|%[^|]|%s",
                                  idBus, platNomor, namaSupir, &kapasitas, &kelas,
                                  rute, strKeberangkatan, strKedatangan, strStatus);

        if (jumlahParsed != 9) {
            printf("Baris tidak valid, dilewati: %s\n", line);
            continue;
        }

        // Simpan ke struct
        strcpy(buses[*busCount].idBus, idBus);
        strcpy(buses[*busCount].platNomor, platNomor);
        strcpy(buses[*busCount].namaSupir, namaSupir);
        buses[*busCount].kapasitas = kapasitas;
        buses[*busCount].kelas = kelas;
        buses[*busCount].rute = strToRute(rute);
        buses[*busCount].status = atoi(strStatus);  // <-- Tambahan penting

        // Waktu keberangkatan
        struct tm waktu = {0};
        int jam, menit;

        if (sscanf(strKeberangkatan, "%d:%d", &jam, &menit) == 2) {
            waktu.tm_hour = jam;
            waktu.tm_min = menit;
            buses[*busCount].keberangkatan = mktime(&waktu);
        } else {
            printf("Format waktu keberangkatan tidak valid: %s\n", strKeberangkatan);
            buses[*busCount].keberangkatan = 0;
        }

        // Waktu kedatangan
        waktu = (struct tm){0};
        if (sscanf(strKedatangan, "%d:%d", &jam, &menit) == 2) {
            waktu.tm_hour = jam;
            waktu.tm_min = menit;
            buses[*busCount].kedatangan = mktime(&waktu);
        } else {
            printf("Format waktu kedatangan tidak valid: %s\n", strKedatangan);
            buses[*busCount].kedatangan = 0;
        }

        (*busCount)++;
    }

   fclose(file);
}

void simpanDataTiketKeFile(DataTiket* tiket, int tiketCount) {
    FILE* fTemp = fopen("FileManajemen/tempTiket.txt", "w");
    if (fTemp == NULL) {
        printf("Gagal membuka file sementara.\n");
        return;
    }

    for (int i = 0; i < tiketCount; i++) {
        fprintf(fTemp, "%s|%s|%s|%s|%s\n",
                tiket[i].idBus,
                tiket[i].namaPenumpang,
                tiket[i].awal,
                tiket[i].tujuan,
                tiket[i].status);
    }

    fclose(fTemp);

    // Ganti file lama dengan yang baru
    remove("FileManajemen/tiket.txt");
    rename("FileManajemen/tempTiket.txt", "FileManajemen/tiket.txt");
}

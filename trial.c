#include "../Header/bus.h"
#include "../Header/tiket.h"
#include "trial.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

// Fungsi untuk membaca data bus dari file
void bacaDataBus(char* filename, DataBus buses[], int* busCount) {
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
        char idBus[10], platNomor[15], namaSupir[50], rute[512], strKeberangkatan[10], strKedatangan[10];
        int kapasitas;
        char kelas;

        // Parsing dari baris file
        int jumlahParsed = sscanf(line, "%[^|]|%[^|]|%[^|]|%d|%c|%[^|]|%[^|]|%s",
                                   idBus, platNomor, namaSupir, &kapasitas, &kelas,
                                   rute, strKeberangkatan, strKedatangan);

        if (jumlahParsed != 8) {
            printf("Baris tidak valid, dilewati: %s\n", line);
            continue;
        }

        // Simpan data dasar
        strcpy(buses[*busCount].idBus, idBus);
        strcpy(buses[*busCount].platNomor, platNomor);
        strcpy(buses[*busCount].namaSupir, namaSupir);
        buses[*busCount].kapasitas = kapasitas;
        buses[*busCount].kelas = kelas;

        // Fungsi untuk mengonversi rute menjadi linked list (dari string)
        buses[*busCount].rute = stringToRute(rute);

        // Konversi waktu keberangkatan
        struct tm waktu = {0};
        int jam, menit;

        // Parsing keberangkatan
        if (sscanf(strKeberangkatan, "%d:%d", &jam, &menit) == 2) {
            waktu.tm_hour = jam;
            waktu.tm_min = menit;
            waktu.tm_sec = 0;
            buses[*busCount].keberangkatan = mktime(&waktu);
        } else {
            printf("Format waktu keberangkatan tidak valid: %s\n", strKeberangkatan);
            buses[*busCount].keberangkatan = 0;
        }

        // Reset waktu dan parsing kedatangan
        waktu = (struct tm){0};
        if (sscanf(strKedatangan, "%d:%d", &jam, &menit) == 2) {
            waktu.tm_hour = jam;
            waktu.tm_min = menit;
            waktu.tm_sec = 0;
            buses[*busCount].kedatangan = mktime(&waktu);
        } else {
            printf("Format waktu kedatangan tidak valid: %s\n", strKedatangan);
            buses[*busCount].kedatangan = 0;
        }

        (*busCount)++;
    }

    fclose(file);
}


// Fungsi untuk membaca data tiket dari file
void BacaDataTiket(char* filename, DataTiket tiket[], int* tiketCount) {
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
        char idTiket[10], idBus[20], namaPenumpang[50], awal[50], tujuan[50], status[15];
        int noKursi, jumlahKursi, totalHarga;

        // Parsing dari baris file
        int jumlahParsed = sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|%d|%d|%s",
                                   idTiket, idBus, namaPenumpang, awal, tujuan,
                                   &noKursi, &jumlahKursi, &totalHarga, status);

        if (jumlahParsed != 9) {
            printf("Baris tidak valid, dilewati: %s\n", line);
            continue;
        }

        // Simpan data tiket
        strcpy(tiket[*tiketCount].idTiket, idTiket);
        strcpy(tiket[*tiketCount].idBus, idBus);
        strcpy(tiket[*tiketCount].namaPenumpang, namaPenumpang);
        strcpy(tiket[*tiketCount].awal, awal);
        strcpy(tiket[*tiketCount].tujuan, tujuan);
        tiket[*tiketCount].noKursi = noKursi;
        tiket[*tiketCount].jumlahKursi = jumlahKursi;
        tiket[*tiketCount].totalHarga = totalHarga;
        strcpy(tiket[*tiketCount].status, status);

        (*tiketCount)++;
    }
}

// Fungsi untuk mensimulasikan perjalanan bus dengan data tiket
void simulasiPerjalanan(DataBus* bus, DataTiket tiket[], int tiketCount) {
    NodeRute* currentRute = bus->rute;
    int terminalCount = 0;
    char traversalPath[512] = "";

    printf("Simulasi perjalanan bus ID: %s\n", bus->idBus);
    // Mengabaikan terminal pertama (misalnya Dago)
    currentRute = currentRute->next;

    // Lakukan perjalanan untuk setiap terminal dalam rute
    while (currentRute != NULL) {
        printf("Bus berangkat dari terminal: %s\n", currentRute->namaTerminal);
        
        // Mengecek penumpang yang naik dan turun
        int totalNaik = 0;
        int totalTurun = 0;

        // Mengecek penumpang yang naik di terminal ini
        for (int i = 0; i < tiketCount; i++) {
            if (strcmp(tiket[i].idBus, bus->idBus) == 0 && 
                strcmp(tiket[i].awal, currentRute->namaTerminal) == 0 &&
                strcmp(tiket[i].status, "confirmed") == 0) {
                
                // Tampilkan penumpang yang naik
                printf("Penumpang %s naik di terminal %s\n", tiket[i].namaPenumpang, currentRute->namaTerminal);
                
                // Menambah jumlah kursi yang tersedia di bus
                bus->kapasitas -= tiket[i].jumlahKursi;
                totalNaik += tiket[i].jumlahKursi;
                
                // Mengubah status tiket menjadi "done" setelah naik
                strcpy(tiket[i].status, "done");
            }
        }

        // Mengecek penumpang yang turun di terminal ini
        for (int i = 0; i < tiketCount; i++) {
            if (strcmp(tiket[i].idBus, bus->idBus) == 0 && 
                strcmp(tiket[i].tujuan, currentRute->namaTerminal) == 0 &&
                strcmp(tiket[i].status, "confirmed") == 0) {
                
                // Tampilkan penumpang yang turun
                printf("Penumpang %s turun di terminal %s\n", tiket[i].namaPenumpang, currentRute->namaTerminal);
                
                // Menambah jumlah kursi yang tersedia di bus
                bus->kapasitas += tiket[i].jumlahKursi;
                totalTurun += tiket[i].jumlahKursi;
                
                // Mengubah status tiket menjadi "done" setelah turun
                strcpy(tiket[i].status, "done");
            }
        }

        // Menampilkan total penumpang yang naik dan turun di terminal ini
        if (totalNaik > 0) {
            printf("Total penumpang naik di terminal %s: %d\n", currentRute->namaTerminal, totalNaik);
        }
        if (totalTurun > 0) {
            printf("Total penumpang turun di terminal %s: %d\n", currentRute->namaTerminal, totalTurun);
        }

        currentRute = currentRute->next;
    }
    printf("Perjalanan selesai, bus mencapai tujuan akhir.\n");
}

int main() {
    DataBus buses[20]; // Array untuk menampung data bus
    DataTiket tiket[40]; // Array untuk menampung data tiket
    int busCount = 0;
    int tiketCount = 0;

    // Membaca data bus dan tiket dari file
    bacaDataBus("FileManajemen/dataBus.txt", buses, &busCount);
    BacaDataTiket("FileManajemen/tiket.txt", tiket, &tiketCount);

    // Input ID bus yang ingin disimulasikan
    char idBusInput[10];
    printf("Masukkan ID Bus yang ingin disimulasikan: ");
    scanf("%s", idBusInput);

    // Cari bus yang sesuai dengan ID bus yang dimasukkan
    DataBus* busDipilih = NULL;
    for (int i = 0; i < busCount; i++) {
        if (strcmp(buses[i].idBus, idBusInput) == 0) {
            busDipilih = &buses[i];
            break;
        }
    }

    if (busDipilih == NULL) {
        printf("Bus dengan ID %s tidak ditemukan.\n", idBusInput);
        return 0;
    }

    // Menampilkan informasi rute bus yang dipilih
    printf("Rute untuk bus ID: %s\n", busDipilih->idBus);
    NodeRute* rute = busDipilih->rute;
    while (rute != NULL) {
        printf("%s ", rute->namaTerminal);
        rute = rute->next;
    }
    printf("\n");

    // Simulasi perjalanan bus yang dipilih
    simulasiPerjalanan(busDipilih, tiket, tiketCount);

    return 0;
}
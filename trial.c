#include "../Header/bus.h"
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

// Fungsi untuk mensimulasikan perjalanan bus tanpa total penumpang
void simulasiPerjalanan(DataBus* bus) {
    NodeRute* currentRute = bus->rute;
    int terminalCount = 0;
    char traversalPath[512] = "";

    printf("Simulasi perjalanan bus ID: %s\n", bus->idBus);
    // Mengabaikan terminal pertama
    currentRute = currentRute->next;

    // Lakukan perjalanan untuk setiap terminal dalam rute
    while (currentRute != NULL) {
        // Menambahkan terminal ke dalam traversalPath
        if (terminalCount > 0) {
            strcat(traversalPath, " -> ");
        }
        strcat(traversalPath, currentRute->namaTerminal);
        
        // Menanyakan berapa penumpang yang turun di terminal ini
        int penumpang;
        printf("Berapa banyak penumpang yang turun di terminal %s? ", currentRute->namaTerminal);
        scanf("%d", &penumpang);

        // Menampilkan proses traversal
        printf("Proses traversal: %s\n", traversalPath);
        printf("Bus %s berhenti di terminal %s, %d penumpang turun.\n", bus->idBus, currentRute->namaTerminal, penumpang);

        currentRute = currentRute->next;  // Lanjutkan ke terminal berikutnya
        terminalCount++;
    }
    printf("Perjalanan selesai, bus mencapai tujuan akhir.\n");
}

int main() {
    DataBus buses[10]; // Array untuk menampung data bus
    int busCount = 0;

    // Membaca data bus dari file
    bacaDataBus("FileManajemen/dataBus.txt", buses, &busCount);

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
    simulasiPerjalanan(busDipilih);

    return 0;
}
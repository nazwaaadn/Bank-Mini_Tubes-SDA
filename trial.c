#include "../Header/bus.h"
#include "../Header/tiket.h"
#include "trial.h"
#include <stdio.h>
#include <string.h>
#include <time.h>




// Fungsi untuk mengonversi string waktu ke struktur waktu
time_t convertToTimeT(const char* waktuStr) {
    struct tm waktu = {0};
    int jam, menit;
    
    // Parsing waktu dalam format "HH:MM"
    if (sscanf(waktuStr, "%d:%d", &jam, &menit) == 2) {
        waktu.tm_hour = jam;
        waktu.tm_min = menit;
        waktu.tm_sec = 0;
        return mktime(&waktu); // Mengonversi ke time_t
    }
    return 0; // Jika format tidak valid, kembalikan waktu 0
}

// Fungsi untuk menampilkan waktu dalam format yang mudah dibaca
void printTime(time_t waktu) {
    char buffer[26];
    struct tm* tm_info;

    tm_info = localtime(&waktu);
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    printf("%s", buffer);
}

// Fungsi utama untuk simulasi perjalanan
void simulasiPerjalanan(DataBus* bus, DataTiket* tiket, int tiketCount) {
    NodeRute* currentRute = bus->rute;
    int terminalCount = 0;

    printf("Simulasi perjalanan bus ID: %s\n", bus->idBus);

    // Loop untuk setiap terminal dalam rute bus
    while (currentRute != NULL) {

        // Tanyakan berapa penumpang yang turun di terminal ini berdasarkan tiket
        int penumpangTurun = 0;
        int penumpangNaik = 0;

        // Mengecek penumpang yang turun dan naik
        for (int i = 0; i < tiketCount; i++) {
            // Penumpang yang turun (tujuan sesuai dengan terminal saat ini)
            if (strcmp(tiket[i].tujuan, currentRute->namaTerminal) == 0 &&
                strcmp(tiket[i].idBus, bus->idBus) == 0 &&
                strcmp(tiket[i].status, "aktif") == 0) {
                penumpangTurun++;  // Hitung jumlah penumpang yang turun di terminal ini
                // Update status tiket setelah turun
                strcpy(tiket[i].status, "done");
                printf("Penumpang turun: %s \n", tiket[i].namaPenumpang);
            }

            // Penumpang yang naik (awal sesuai dengan terminal saat ini)
            if (strcmp(tiket[i].awal, currentRute->namaTerminal) == 0 &&
                strcmp(tiket[i].idBus, bus->idBus) == 0 &&
                strcmp(tiket[i].status, "aktif") == 0) {
                penumpangNaik++;  // Hitung jumlah penumpang yang naik di terminal ini
                // Update status tiket setelah naik
                printf("Penumpang naik: %s \n", tiket[i].namaPenumpang);
            }
        }

        // Menampilkan proses traversal dan penumpang naik/turun
        printf("Bus %s berhenti di terminal %s, %d penumpang turun, %d penumpang naik.\n\n", 
                bus->idBus, currentRute->namaTerminal, penumpangTurun, penumpangNaik);

        // Melanjutkan ke terminal berikutnya
        currentRute = currentRute->next; 
        terminalCount++;
    }

    // Menginformasikan perjalanan selesai
    printf("Perjalanan selesai, bus mencapai tujuan akhir.\n");
}

int main() {
    DataBus buses[10]; // Array untuk menampung data bus
    int busCount = 0;

    // Membaca data bus dari file
    bacaDataBus("FileManajemen/dataBus.txt", buses, &busCount);

    DataTiket tiket[100]; // Array untuk menampung data tiket
    int tiketCount = 0;

    // Membaca data tiket dari file
    bacaDataTiket("tiket.txt", tiket, &tiketCount);

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
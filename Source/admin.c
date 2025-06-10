#include <stdio.h>
#include <string.h>
#include "queue.h"
#include "admin.h"
#include "bus.h"

// --- Fungsi Login Admin ---
int LoginAdmin() {
    char id[50];
    char password[50];

    printf("===== LOGIN ADMIN =====\n");
    printf("ID Admin     : ");
    scanf("%s", id);
    printf("Password     : ");
    scanf("%s", password);

    if (strcmp(id, "admin") == 0 && strcmp(password, "adminkece") == 0) {
        printf("Login berhasil!\n\n");
        return 1;
    } else {
        printf("ID atau Password salah!\n\n");
        return 0;
    }
}

// --- Lihat Antrian Bus di Terminal ---
void LihatBusDiSetiapTerminal(Queue* q) {
    NodeBus* curr = Front(*q);
    int i = 1;
    if(is_Empty(*q)) {
        printf("Antrian kosong.\n");
    } else {
        printf("\n=== Daftar Bus Dalam Antrian ===\n");
        while(curr != NULL) {
            printf("%d. ID Bus: %s | Supir: %s\n", i, curr->Info.idBus, curr->Info.namaSupir);
            curr = Next(curr);
            i++;
        }
    }
}

// --- Release Bus dari Terminal + Update Status Bus ke Tidak Aktif (1) ---
void ReleaseBusDariTerminal(Queue* q) {
    DataBus busKeluar;
    deQueue(q, &busKeluar);
    printf("Bus %s (%s) telah keluar dari antrian.\n", busKeluar.idBus, busKeluar.namaSupir);

    // Update status bus menjadi tidak aktif (1)
    updateBusStatus(busKeluar.idBus, 1);
}

// --- Fungsi untuk Update Status Bus di File ---
void updateBusStatus(const char *idBus, int statusBaru) {
    FILE *in = fopen("FileManajemen/dataBus.txt", "r");
    FILE *out = fopen("FileManajemen/temp.txt", "w");
    if (!in || !out) {
        printf("Gagal membuka file untuk update status bus.\n");
        if (in) fclose(in);
        if (out) fclose(out);
        return;
    }
    char line[2048];
    while (fgets(line, sizeof(line), in)) {
        char id[20], plat[30], nama[50], rute[2000], kelas;
        int kapasitas;
        char waktuBrk[6], waktuTba[6];
        int statusOld;
        // Ambil status (kolom terakhir)
        int jumlahField = sscanf(line, "%19[^|]|%29[^|]|%49[^|]|%d|%c|%1999[^|]|%5[^|]|%5[^|]|%d",
                                 id, plat, nama, &kapasitas, &kelas, rute, waktuBrk, waktuTba, &statusOld);

        if (jumlahField == 9 && strcmp(id, idBus) == 0) {
            // Tulis baris dengan status baru
            fprintf(out, "%s|%s|%s|%d|%c|%s|%s|%s|%d\n",
                    id, plat, nama, kapasitas, kelas, rute, waktuBrk, waktuTba, statusBaru);
        } else {
            // Tulis baris lain seperti biasa
            fprintf(out, "%s", line);
            if (line[strlen(line)-1] != '\n') fprintf(out, "\n");
        }
    }
    fclose(in);
    fclose(out);
    remove("FileManajemen/dataBus.txt");
    rename("FileManajemen/temp.txt", "FileManajemen/dataBus.txt");
}

void RiwayatTransaksi() {
    FILE* file = fopen("FileManajemen/tiket.txt", "r");
    if (!file) {
        printf("Gagal membuka file tiket.txt\n");
        return;
    }

    printf("\n========== RIWAYAT TRANSAKSI TIKET ==========\n");
    char idTiket[20], namaPenumpang[50], awal[50], tujuan[50], idBus[20], status[20];
    int count = 0;
    while (fscanf(file, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n",
                  idTiket, namaPenumpang, awal, tujuan, idBus, status) == 6) {
        printf("[%d]\n", ++count);
        printf("ID Tiket  : %s\n", idTiket);
        printf("Nama      : %s\n", namaPenumpang);
        printf("Dari      : %s\n", awal);
        printf("Tujuan    : %s\n", tujuan);
        printf("ID Bus    : %s\n", idBus);
        printf("Status    : %s\n", status);
        printf("---------------------------------------------\n");
    }
    if (count == 0) {
        printf("Belum ada transaksi tiket.\n");
    }
    fclose(file);
}


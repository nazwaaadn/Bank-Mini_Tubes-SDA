#include <limits.h>
#include <malloc.h>
#include "bus.h"
#include "terminal.h"
/********** BODY SUB PROGRAM ***********/
/**** Predikat untuk test keadaan LIST ****/
NodeBus *HeadBus = Nil;
boolean isBusListEmpty()
/* Mengirim true jika List Kosong */
{
	if (HeadBus == Nil)
    {
        return true;
    }else {
        return false;
    }
}

NodeBus* alokasiNodeBus(DataBus busData) {
    NodeBus* newNode = (NodeBus*)malloc(sizeof(NodeBus));
    if (newNode != NULL) {
        newNode->Info = busData;  
        newNode->next = NULL;
    }
    return newNode;
}

void freeBus(NodeBus* busNode) {
    if (busNode != NULL) {
        free(busNode);   
    }
}

void inputDataBus(DataBus *busBaru)
{
    terminalTree T;
    int root = 0;
    printf("Masukkan ID Bus: ");
    scanf("%s", busBaru->idBus);
    printf("Masukkan Plat Nomor: ");
    scanf("%s", busBaru->platNomor);
    printf("Masukkan Nama Supir: ");
    scanf(" %[^\n]", busBaru->namaSupir);
    printf("Masukkan Kapasitas: ");
    scanf("%d", &busBaru->kapasitas);
    printf("Masukkan Kelas (A/B/C): ");
    scanf(" %c", &busBaru->kelas);

    char terminalAwal[50];
    printf("Masukkan Terminal Awal: ");
    scanf(" %[^\n]", terminalAwal);

    char terminalTujuan[50];
    printf("Masukkan Terminal Tujuan: ");
    scanf(" %[^\n]", terminalTujuan);

    char bufferRute[100] = ""; // buat buffer kosong untuk menyimpan rute
    address awal = findRuteAwal(T, root, terminalAwal);
    if (awal != nil) {
        boolean akhir = PreOrderSimpanRute(T, awal, terminalTujuan, bufferRute);
        if (akhir) {
            strcpy(busBaru->rute, bufferRute);
        } else {
            printf("Terminal tujuan tidak ditemukan dari rute awal.\n");
            strcpy(busBaru->rute, "-");
        }
    } else {
        printf("Terminal awal tidak ditemukan.\n");
        strcpy(busBaru->rute, "-");
    }


    // Waktu keberangkatan
    struct tm waktuBerangkat = {0};
    printf("Masukkan Waktu Keberangkatan (HH MM): ");
    scanf("%d %d", &waktuBerangkat.tm_hour, &waktuBerangkat.tm_min);
    waktuBerangkat.tm_year = 123; // Tahun dummy: 2023
    waktuBerangkat.tm_mon = 0;    // Bulan dummy: Januari
    waktuBerangkat.tm_mday = 1;
    busBaru->keberangkatan = mktime(&waktuBerangkat);

    // Waktu kedatangan
    struct tm waktuTiba = {0};
    printf("Masukkan Waktu Kedatangan (HH MM): ");
    scanf("%d %d", &waktuTiba.tm_hour, &waktuTiba.tm_min);
    waktuTiba.tm_year = 123;
    waktuTiba.tm_mon = 0;
    waktuTiba.tm_mday = 1;
    busBaru->kedatangan = mktime(&waktuTiba);

    insertBus(busBaru); // cukup kirim pointer
}


void insertBus(DataBus *busBaru)
{
    NodeBus *newNode = alokasiNodeBus(*busBaru);
    if (isBusListEmpty()) {
        HeadBus = newNode;
    } else {
        NodeBus *current = HeadBus;
        while (current->next != Nil) {
            current = current->next;
        }
        current->next = newNode;
    }

    insertRute(newNode, busBaru->rute);

    FILE *file = fopen("data_bus.txt", "a");
    if (file != NULL) {
        // Konversi waktu ke struct tm kembali untuk dicetak
        struct tm *wktBrk = localtime(&busBaru->keberangkatan);
        struct tm *wktTba = localtime(&busBaru->kedatangan);

        fprintf(file, "%s|%s|%s|%d|%c|%02d:%02d|%02d:%02d\n",
            busBaru->idBus,
            busBaru->platNomor,
            busBaru->namaSupir,
            busBaru->rute,
            busBaru->kapasitas,
            busBaru->kelas,
            wktBrk->tm_hour, wktBrk->tm_min,
            wktTba->tm_hour, wktTba->tm_min
        );

        fprintf(file, "\n");
        fclose(file);
    } else {
        printf("Gagal membuka file untuk menyimpan data bus.\n");
    }
}

void inputDataBusBaru(DataBus *busBaru) {
    printf("Masukkan Nama Supir Baru: ");
    scanf(" %[^\n]", busBaru->namaSupir);

    printf("Masukkan Kapasitas Baru: ");
    scanf("%d", &busBaru->kapasitas);

    printf("Masukkan Kelas Baru (A/B/C): ");
    scanf(" %c", &busBaru->kelas);

    // Waktu keberangkatan
    struct tm waktuBerangkat = {0};
    printf("Masukkan Waktu Keberangkatan Baru (HH MM): ");
    scanf("%d %d", &waktuBerangkat.tm_hour, &waktuBerangkat.tm_min);
    waktuBerangkat.tm_year = 123;
    waktuBerangkat.tm_mon = 0;
    waktuBerangkat.tm_mday = 1;
    busBaru->keberangkatan = mktime(&waktuBerangkat);

    // Waktu kedatangan
    struct tm waktuTiba = {0};
    printf("Masukkan Waktu Kedatangan Baru (HH MM): ");
    scanf("%d %d", &waktuTiba.tm_hour, &waktuTiba.tm_min);
    waktuTiba.tm_year = 123;
    waktuTiba.tm_mon = 0;
    waktuTiba.tm_mday = 1;
    busBaru->kedatangan = mktime(&waktuTiba);
}

void tampilkanDataBus(DataBus bus) {
    printf("\n--- Data Bus ---\n");
    printf("ID Bus: %s\n", bus.idBus);
    printf("Plat Nomor: %s\n", bus.platNomor);
    printf("Nama Supir: %s\n", bus.namaSupir);
    printf("Kapasitas: %d\n", bus.kapasitas);
    printf("Kelas: %c\n", bus.kelas);
    printf("Rute: %s\n", bus.rute);

    struct tm *wkt = localtime(&bus.keberangkatan);
    printf("Keberangkatan: %02d:%02d\n", wkt->tm_hour, wkt->tm_min);

    wkt = localtime(&bus.kedatangan);
    printf("Kedatangan: %02d:%02d\n", wkt->tm_hour, wkt->tm_min);
}


void editBus(char idBus[]) {
    NodeBus *current = HeadBus;
    while (current != Nil) {
        if (strcmp(current->Info.idBus, idBus) == 0) {
            printf("\nData sebelumnya:\n");
            tampilkanDataBus(current->Info);

            DataBus busBaru;
            strcpy(busBaru.idBus, current->Info.idBus);  // ID tetap sama
            strcpy(busBaru.platNomor, current->Info.platNomor);  // Plat tetap

            // Input data baru
            inputDataBusBaru(&busBaru);

            // Tanya apakah ingin mengubah rute
            char jawab;
            printf("Apakah ingin mengubah rute? (y/n): ");
            scanf(" %c", &jawab);

            if (jawab == 'y' || jawab == 'Y') {
                editRute(idBus, &busBaru); // Update field rute
            } else {
                strcpy(busBaru.rute, current->Info.rute); // Tetap pakai rute lama
            }

            // Simpan perubahan
            current->Info = busBaru;
            printf("\nData bus berhasil diperbarui!\n");
            return;
        }
        current = current->next;
    }

    printf("Bus dengan ID %s tidak ditemukan.\n", idBus);
}


void editRute(char idBus[], DataBus *busBaru)
{
    terminalTree T;

    char terminalAwal[50];
    printf("Masukkan Terminal Awal: ");
    scanf(" %[^\n]", terminalAwal);

    char terminalTujuan[50];
    printf("Masukkan Terminal Tujuan: ");
    scanf(" %[^\n]", terminalTujuan);

    char bufferRute[100] = ""; // buat buffer kosong untuk menyimpan rute
    address awal = findRuteAwal(T, root, terminalAwal);
    if (awal != nil) {
        boolean ketemu = PreOrderSimpanRute(T, awal, terminalTujuan, bufferRute);
        if (ketemu) {
            strcpy(busBaru->rute, bufferRute);
        } else {
            printf("Terminal tujuan tidak ditemukan dari rute awal.\n");
            strcpy(busBaru->rute, "-");
        }
    } else {
        printf("Terminal awal tidak ditemukan.\n");
        strcpy(busBaru->rute, "-");
    }
}

void deleteBus(char idBus[])
{
    NodeBus *current = HeadBus;
    while (current != Nil) {
        if (strcmp(current->Info.idBus, idBus) == 0) {
            freeBus(current);  
            if (current == HeadBus) {
                HeadBus = current->next;  
            } else {
                NodeBus *prev = HeadBus;
                while (prev->next != current) {
                    prev = prev->next;  
                }
                prev->next = current->next;  
            }  
            return;
        }
        current = current->next;
    }
}

void printAllBus() 
{
    NodeBus *current = HeadBus;
    if (isBusListEmpty()) {
        printf("Tidak ada data bus yang tersedia.\n");
        return;
    }
    LihatBusDiTerminal();
    printf("=========================================================================================================================\n");
    printf("| %-8s | %-12s | %-20s | %-9s | %-5s | %-20s | %-20s |\n",
        "ID Bus", "Plat", "Supir", "Kapasitas", "Kelas", "Keberangkatan", "Kedatangan");
    printf("=========================================================================================================================\n");

    NodeBus* current = HeadBus;
    while (current != Nil) {
        char *waktuBerangkat = ctime(&current->Info.keberangkatan);
        char *waktuTiba = ctime(&current->Info.kedatangan);

        // Hapus newline (\n) di akhir waktu
        waktuBerangkat[strcspn(waktuBerangkat, "\n")] = 0;
        waktuTiba[strcspn(waktuTiba, "\n")] = 0;

        printf("| %-8s | %-12s | %-20s | %-9d | %-5c | %-20s | %-20s |\n",
            current->Info.idBus,
            current->Info.platNomor,
            current->Info.namaSupir,
            current->Info.kapasitas,
            current->Info.kelas,
            waktuBerangkat,
            waktuTiba);

        current = current->next;
    }
    printf("=========================================================================================================================\n");

    // Cetak rute di bawah tabel
    printf("\nDetail Rute per Bus:\n");
    current = HeadBus;
    while (current != Nil) {
        printf("- Rute untuk Bus ID %s: ", current->Info.idBus);
        printRute(current->Info.rute); // Asumsikan ini mencetak dalam format: halte1 -> halte2 -> halte3
        current = current->next;
    }

}

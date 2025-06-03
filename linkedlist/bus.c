#include <limits.h>
#include <malloc.h>
#include "bus.h"
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

RuteBus* alokasiRute(NodeRute* halte) {
    RuteBus* newRute = (RuteBus*)malloc(sizeof(RuteBus));
    if (newRute != NULL) {
        newRute->halte = halte;
        newRute->next = NULL;
    }
    return newRute;
}

void freeRute(RuteBus* rute) {
    RuteBus* temp;
    while (rute != NULL) {
        temp = rute;
        rute = rute->next;
        free(temp);
    }
}

void freeBus(NodeBus* busNode) {
    if (busNode != NULL) {
        freeRute(busNode->Info.rute);  // bebaskan linked list rute
        free(busNode);                 // bebaskan node bus
    }
}

void inputDataBus(DataBus *busBaru)
{
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

    busBaru->rute = inputRute();
    insertBus(&busBaru); // cukup kirim pointer
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
            busBaru->kapasitas,
            busBaru->kelas,
            wktBrk->tm_hour, wktBrk->tm_min,
            wktTba->tm_hour, wktTba->tm_min
        );

        RuteBus *currentRute = busBaru->rute;
        while (currentRute != NULL) {
            if (currentRute->halte != NULL) {
                fprintf(file, "%s", currentRute->halte);
                if (currentRute->next != NULL) {
                    fprintf(file, "->");
                }
            }
            currentRute = currentRute->next;
        }
        fprintf(file, "\n");
        fclose(file);
    } else {
        printf("Gagal membuka file untuk menyimpan data bus.\n");
    }
}


void insertRute(NodeBus* bus, NodeRute* halte)
{
    if (bus != NULL)
    {
        RuteBus* newRute = alokasiRute(halte);
        if (bus->Info.rute == NULL) {
            bus->Info.rute = newRute;  
        } else {
            RuteBus* current = bus->Info.rute;
            while (current->next != NULL) {
                current = current->next;  
            }
            current->next = newRute;  
        }
    }
}

void editBus(char idBus[], DataBus busBaru)
{
    NodeBus *current = HeadBus;
    while (current != Nil) {
        if (strcmp(current->Info.idBus, idBus) == 0) {
            current->Info = busBaru;  
            freeRute(current->Info.rute);  
            current->Info.rute = busBaru.rute;  
            return;
        }
        current = current->next;
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
            freeRute(current->Info.rute);  
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

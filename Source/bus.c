#include <limits.h>
#include <malloc.h>
#include "../Header/bus.h"
#include "../Header/terminal.h"
#include <string.h>
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

NodeBus* inputDataBus(terminalTree T, int root) {
    NodeBus* busBaru = (NodeBus*)malloc(sizeof(NodeBus));
    if (busBaru == NULL) {
        printf("Gagal alokasi memori.\n");
        return NULL;
    }

    printf("Masukkan ID Bus: ");
    scanf("%9s", busBaru->Info.idBus);

    printf("Masukkan Plat Nomor: ");
    scanf("%14s", busBaru->Info.platNomor);

    printf("Masukkan Nama Supir: ");
    scanf(" %[^\n]", busBaru->Info.namaSupir);

    printf("Masukkan Kapasitas: ");
    scanf("%d", &busBaru->Info.kapasitas);

    printf("Masukkan Kelas (A/B/C): ");
    scanf(" %c", &busBaru->Info.kelas);

    char terminalAwal[50], terminalTujuan[50];
    printf("Masukkan Terminal Awal: ");
    scanf(" %[^\n]", terminalAwal);

    printf("Masukkan Terminal Tujuan: ");
    scanf(" %[^\n]", terminalTujuan);

    // Cari rute
    char bufferRute[100] = "";
    address awal = findRuteAwal(T, root, terminalAwal);
    if (awal != nil) {
        if (PreOrder(T, awal, terminalTujuan, bufferRute)) {
            strcpy(busBaru->Info.rute, bufferRute);
        } else {
            printf("Terminal tujuan tidak ditemukan dari rute awal.\n");
            strcpy(busBaru->Info.rute, "-");
        }
    } else {
        printf("Terminal awal tidak ditemukan.\n");
        strcpy(busBaru->Info.rute, "-");
    }

    // Waktu keberangkatan
    int jam, menit;
    printf("Masukkan Waktu Keberangkatan (HH MM): ");
    scanf("%d %d", &jam, &menit);

    time_t now = time(NULL);
    struct tm waktuBerangkat = *localtime(&now);
    waktuBerangkat.tm_hour = jam;
    waktuBerangkat.tm_min = menit;
    waktuBerangkat.tm_sec = 0;
    waktuBerangkat.tm_mday = 1;
    waktuBerangkat.tm_mon = 0;
    waktuBerangkat.tm_year = 123;
    busBaru->Info.keberangkatan = mktime(&waktuBerangkat);

    // Waktu kedatangan
    printf("Masukkan Waktu Kedatangan (HH MM): ");
    scanf("%d %d", &jam, &menit);
    struct tm waktuTiba = waktuBerangkat;
    waktuTiba.tm_hour = jam;
    waktuTiba.tm_min = menit;
    busBaru->Info.kedatangan = mktime(&waktuTiba);

    busBaru->next = NULL;
    return busBaru;
}


void insertBus(NodeBus *newNode) {
    if (newNode == NULL) return;

    if (isBusListEmpty()) {
        HeadBus = newNode;
    } else {
        NodeBus *curr = HeadBus;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = newNode;
    }

    saveAllBusToFile();  // Simpan ke file setiap kali ada penambahan bus
}


void inputDataBusBaru(DataBus *busBaru) {
    printf("Masukkan Nama Supir Baru: ");
    scanf(" %[^\n]", busBaru->namaSupir);

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


void hapusNewline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

void editBus(char idBus[]) {
    FILE *file = fopen("FileManajemen/dataBus.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file dataBus.txt\n");
        return;
    }

    FILE *temp = fopen("FileManajemen/temp.txt", "w");
    if (temp == NULL) {
        printf("Gagal membuka file sementara\n");
        fclose(file);
        return;
    }

    char line[256];
    boolean found = false;

    while (fgets(line, sizeof(line), file)) {
        hapusNewline(line);  // hapus newline supaya string bersih

        DataBus bus;
        int jamBrk, menitBrk, jamTba, menitTba;

        int jumlahField = sscanf(line, "%[^|]|%[^|]|%[^|]|%d|%c|%[^|]|%d:%d|%d:%d",
               bus.idBus,
               bus.platNomor,
               bus.namaSupir,
               &bus.kapasitas,
               &bus.kelas,
               bus.rute,
               &jamBrk, &menitBrk,
               &jamTba, &menitTba);

        if (jumlahField != 10) {
            // Format salah atau baris rusak, skip saja tulis ulang ke file temp agar data tidak hilang
            fprintf(temp, "%s\n", line);
            continue;
        }

        // Set waktu keberangkatan
        struct tm waktu = {0};
        waktu.tm_year = 123;  // 2023
        waktu.tm_mon = 0;
        waktu.tm_mday = 1;
        waktu.tm_hour = jamBrk;
        waktu.tm_min = menitBrk;
        bus.keberangkatan = mktime(&waktu);

        // Set waktu kedatangan
        waktu.tm_hour = jamTba;
        waktu.tm_min = menitTba;
        bus.kedatangan = mktime(&waktu);

        if (strcmp(bus.idBus, idBus) == 0) {
            found = true;

            // printf("Data sebelumnya:\n");
            // tampilkanDataBus(bus);

            inputDataBusBaru(&bus);

            char jawab;
            while ((getchar()) != '\n'); // flush input buffer
            printf("Apakah ingin mengubah rute? (y/n): ");
            scanf(" %c", &jawab);


            if (jawab == 'y' || jawab == 'Y') {
                editRute(idBus, &bus);
            }

            // Tulis data bus yang sudah diedit ke file temp
            struct tm *wktBrk = localtime(&bus.keberangkatan);
            struct tm *wktTba = localtime(&bus.kedatangan);

            fprintf(temp, "%s|%s|%s|%d|%c|%s|%02d:%02d|%02d:%02d\n",
                    bus.idBus,
                    bus.platNomor,
                    bus.namaSupir,
                    bus.kapasitas,
                    bus.kelas,
                    bus.rute,
                    wktBrk->tm_hour, wktBrk->tm_min,
                    wktTba->tm_hour, wktTba->tm_min);
            
            
        } else {
            // Baris lain ditulis ulang tanpa perubahan
            fprintf(temp, "%s\n", line);
        }
    }

    fclose(file);
    fclose(temp);

    if (found) {
    remove("FileManajemen/dataBus.txt");
    rename("FileManajemen/temp.txt", "FileManajemen/dataBus.txt");
    printf("Data bus berhasil diperbarui langsung di file!\n");
    } else {
        remove("FileManajemen/temp.txt");
        printf("Bus dengan ID %s tidak ditemukan.\n", idBus);
    }

}


void editRute(char idBus[], DataBus *busBaru)
{
    terminalTree T;
    CreateTerminal(T); // <- ini penting!

    char terminalAwal[50];
    printf("Masukkan Terminal Awal: ");
    scanf(" %[^\n]", terminalAwal);
    
    char terminalTujuan[50];
    printf("Masukkan Terminal Tujuan: ");
    scanf(" %[^\n]", terminalTujuan);

    int root = 0;
    char bufferRute[255] = "";
    address awal = findRuteAwal(T, root, terminalAwal);
    if (awal != nil) {
        boolean ketemu = PreOrder(T, awal, terminalTujuan, bufferRute);
        if (ketemu) {
            strncpy(busBaru->rute, bufferRute, sizeof(busBaru->rute) - 1);
            busBaru->rute[sizeof(busBaru->rute) - 1] = '\0';
        } else {
            strcpy(busBaru->rute, "-");
        }

    } else {
        printf("Terminal awal tidak ditemukan.\n");
        strcpy(busBaru->rute, "-");
    }
    
}

void deleteBus(char idBus[]) {
    FILE *file = fopen("FileManajemen/dataBus.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file dataBus.txt\n");
        return;
    }

    FILE *temp = fopen("FileManajemen/temp.txt", "w");
    if (temp == NULL) {
        printf("Gagal membuka file sementara\n");
        fclose(file);
        return;
    }

    char line[256];
    boolean found = false;

    while (fgets(line, sizeof(line), file)) {
        DataBus bus;
        hapusNewline(line);

        int jamBrk, menitBrk, jamTba, menitTba;
        int jumlahField = sscanf(line, "%[^|]|%[^|]|%[^|]|%d|%c|%[^|]|%d:%d|%d:%d",
               bus.idBus,
               bus.platNomor,
               bus.namaSupir,
               &bus.kapasitas,
               &bus.kelas,
               bus.rute,
               &jamBrk, &menitBrk,
               &jamTba, &menitTba);

        if (jumlahField != 10) {
            // Format salah → tetap tulis agar data tidak hilang
            fprintf(temp, "%s\n", line);
            continue;
        }

        if (strcmp(bus.idBus, idBus) == 0) {
            found = true;
            // Skip baris ini: tidak ditulis ke temp (artinya dihapus)
            continue;
        }

        // Tulis ulang ke file temp
        fprintf(temp, "%s\n", line);
    }

    fclose(file);
    fclose(temp);

    if (found) {
        remove("FileManajemen/dataBus.txt");
        rename("FileManajemen/temp.txt", "FileManajemen/dataBus.txt");
        printf("Bus dengan ID %s berhasil dihapus dari file!\n", idBus);
    } else {
        remove("FileManajemen/temp.txt");
        printf("Bus dengan ID %s tidak ditemukan di file.\n", idBus);
    }
}


void saveAllBusToFile() {
    FILE *file = fopen("FileManajemen/dataBus.txt", "a");
    if (file == NULL) {
        printf("Gagal membuka file untuk menyimpan data.\n");
        return;
    }

    NodeBus *curr = HeadBus;
    while (curr != NULL) {
        struct tm *wktBrk = localtime(&curr->Info.keberangkatan);
        struct tm *wktTba = localtime(&curr->Info.kedatangan);

        fprintf(file, "%s|%s|%s|%d|%c|%s|%02d:%02d|%02d:%02d\n",
            curr->Info.idBus,
            curr->Info.platNomor,
            curr->Info.namaSupir,
            curr->Info.kapasitas,
            curr->Info.kelas,
            curr->Info.rute,
            wktBrk->tm_hour, wktBrk->tm_min,
            wktTba->tm_hour, wktTba->tm_min
        );

        curr = curr->next;
    }

    fclose(file);
}


void printAllBus() {
    FILE *file = fopen("FileManajemen/dataBus.txt", "r");
    if (file == NULL) {
        printf("File dataBus.txt tidak ditemukan atau gagal dibuka.\n");
        return;
    }

    char line[256];
    printf("=========================================================================================================================\n");
    printf("| %-8s | %-12s | %-20s | %-50s | %-9s | %-5s | %-20s | %-20s |\n",
        "ID Bus", "Plat", "Supir", "Rute", "Kapasitas", "Kelas", "Keberangkatan", "Kedatangan");
    printf("=========================================================================================================================\n");

    while (fgets(line, sizeof(line), file)) {
        // Contoh baris:
        // Bus01|ahjak|ajkaj|30|C|DagoSimpang DagoCiumbuleuitPunclutLembang|13:20|13:20

        // Pisah line pakai strtok berdasarkan '|'
        char *idBus = strtok(line, "|");
        char *platNomor = strtok(NULL, "|");
        char *namaSupir = strtok(NULL, "|");
        char *kapasitasStr = strtok(NULL, "|");
        char *kelasStr = strtok(NULL, "|");
        char *rute = strtok(NULL, "|");
        char *keberangkatan = strtok(NULL, "|");
        char *kedatangan = strtok(NULL, "|\n");

        if (!idBus || !platNomor || !namaSupir || !kapasitasStr || !kelasStr || !rute || !keberangkatan || !kedatangan) {
            continue; // baris tidak lengkap, skip
        }

        int kapasitas = atoi(kapasitasStr);
        char kelas = kelasStr[0];

        printf("| %-8s | %-12s | %-20s | %-50s | %-9d | %-5c | %-20s | %-20s |\n",
            idBus,
            platNomor,
            namaSupir,
            rute,
            kapasitas,
            kelas,
            keberangkatan,
            kedatangan);
    }

    printf("=========================================================================================================================\n");
    fclose(file);
}


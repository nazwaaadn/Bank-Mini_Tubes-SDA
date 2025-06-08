#include <limits.h>
#include <malloc.h>
#include "../Header/bus.h"
#include "../Header/terminal.h"
#include <string.h>
/********** BODY SUB PROGRAM ***********/
/**** Predikat untuk test keadaan LIST ****/
NodeBus *HeadBus = Nil;
NodeRute *HeadRute = Nil;
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

    // Input data dasar
    printf("Masukkan ID Bus: ");
    scanf("%9s", busBaru->Info.idBus);
    getchar();

    printf("Masukkan Plat Nomor: ");
    scanf("%14s", busBaru->Info.platNomor);
    getchar();

    printf("Masukkan Nama Supir: ");
    fgets(busBaru->Info.namaSupir, sizeof(busBaru->Info.namaSupir), stdin);
    busBaru->Info.namaSupir[strcspn(busBaru->Info.namaSupir, "\n")] = '\0';

    printf("Masukkan Kapasitas: ");
    scanf("%d", &busBaru->Info.kapasitas);
    getchar();

    printf("Masukkan Kelas (A/B/C): ");
    scanf(" %c", &busBaru->Info.kelas);
    getchar();

    // Input waktu keberangkatan bus
    int jam, menit;
    printf("Masukkan Waktu Keberangkatan (HH MM): ");
    scanf("%d %d", &jam, &menit);

    time_t now = time(NULL);
    struct tm waktuBerangkat = *localtime(&now);
    waktuBerangkat.tm_hour = jam;
    waktuBerangkat.tm_min = menit;
    waktuBerangkat.tm_sec = 0;
    busBaru->Info.keberangkatan = mktime(&waktuBerangkat);

    // Input waktu kedatangan bus
    printf("Masukkan Waktu Kedatangan (HH MM): ");
    scanf("%d %d", &jam, &menit);

    struct tm waktuTiba = waktuBerangkat;
    waktuTiba.tm_hour = jam;
    waktuTiba.tm_min = menit;
    busBaru->Info.kedatangan = mktime(&waktuTiba);

    getchar(); // Bersihkan newline sebelum fgets berikutnya

    // Input terminal awal dan tujuan
    char terminalAwal[50], terminalTujuan[50];
    printf("Masukkan Terminal Awal: ");
    fgets(terminalAwal, sizeof(terminalAwal), stdin);
    terminalAwal[strcspn(terminalAwal, "\n")] = '\0';

    printf("Masukkan Terminal Tujuan: ");
    fgets(terminalTujuan, sizeof(terminalTujuan), stdin);
    terminalTujuan[strcspn(terminalTujuan, "\n")] = '\0';

    // Cari rute dari awal ke tujuan
    NodeRute* headRute = NULL;
    NodeRute* tailRute = NULL;
    address awal = findRuteAwal(T, root, terminalAwal);

    if (awal != nil) {
        if (!PreOrderToLinkedList(T, awal, terminalTujuan, &headRute, busBaru->Info.kedatangan)) {
            printf("❌ Terminal tujuan tidak ditemukan dari %s.\n", terminalAwal);
        } else {
            NodeRute* curr = headRute;
            printf("Rute dari %s ke %s:\n", terminalAwal, terminalTujuan);
            while (curr != NULL) {
                struct tm* jam = localtime(&curr->waktubrgkt);
                printf("- %s (%02d:%02d)\n", curr->namaTerminal, jam->tm_hour, jam->tm_min);
                curr = curr->next;
            }
            busBaru->Info.rute = headRute;
        }
    } else {
        printf("❌ Terminal awal tidak ditemukan.\n");
    }

    busBaru->next = NULL;
    saveSingleBusToFile(busBaru->Info); // Simpan ke file
    return busBaru;
}



void tambahRute(NodeRute** head, const char* nama) {
    NodeRute* baru = (NodeRute*)malloc(sizeof(NodeRute));
    if (baru == NULL) {
        printf("Gagal alokasi memori untuk NodeRute.\n");
        return;
    }
    strcpy(baru->namaTerminal, nama);
    baru->next = *head;      // Node baru menunjuk ke head lama
    *head = baru;            // Head diperbarui ke node baru
}

void inputDataBusBaru(DataBus *busBaru) {
    while ((getchar()) != '\n'); // flush newline

    printf("Masukkan Nama Supir Baru: ");
    fgets(busBaru->namaSupir, sizeof(busBaru->namaSupir), stdin);
    busBaru->namaSupir[strcspn(busBaru->namaSupir, "\n")] = '\0';

    printf("Masukkan Kelas Baru (A/B/C): ");
    scanf(" %c", &busBaru->kelas);

    struct tm waktuBrk = {0};
    printf("Masukkan Waktu Keberangkatan Baru (HH MM): ");
    scanf("%d %d", &waktuBrk.tm_hour, &waktuBrk.tm_min);
    waktuBrk.tm_year = 123; waktuBrk.tm_mon = 0; waktuBrk.tm_mday = 1;
    busBaru->keberangkatan = mktime(&waktuBrk);

    struct tm waktuTba = {0};
    printf("Masukkan Waktu Kedatangan Baru (HH MM): ");
    scanf("%d %d", &waktuTba.tm_hour, &waktuTba.tm_min);
    waktuTba.tm_year = 123; waktuTba.tm_mon = 0; waktuTba.tm_mday = 1;
    busBaru->kedatangan = mktime(&waktuTba);
}


void tampilkanDataBus(DataBus bus) {
    printf("\n--- Data Bus ---\n");
    printf("ID Bus: %s\n", bus.idBus);
    printf("Plat Nomor: %s\n", bus.platNomor);
    printf("Nama Supir: %s\n", bus.namaSupir);
    printf("Kapasitas: %d\n", bus.kapasitas);
    printf("Kelas: %c\n", bus.kelas);

    // Menampilkan rute dan waktu keberangkatan setiap terminal
    char ruteBuffer[1024] = "";
    NodeRute* curr = bus.rute;
    while (curr != NULL) {
        struct tm *wktTerminal = localtime(&curr->waktubrgkt);
        char waktuStr[10];
        sprintf(waktuStr, "%02d:%02d", wktTerminal->tm_hour, wktTerminal->tm_min);
        strcat(ruteBuffer, curr->namaTerminal);
        strcat(ruteBuffer, " (");
        strcat(ruteBuffer, waktuStr);
        strcat(ruteBuffer, ")");
        if (curr->next != NULL) strcat(ruteBuffer, "->");
        curr = curr->next;
    }

    printf("Rute: %s\n", ruteBuffer);

    // Menampilkan waktu keberangkatan dan kedatangan bus
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
    NodeBus* busNode = searchBusByID(idBus);
    if (busNode == NULL) {
        printf("Bus dengan ID %s tidak ditemukan.\n", idBus);
        return;
    }

    // Salin data yang tidak akan diubah
    DataBus busBaru;
    strcpy(busBaru.idBus, busNode->Info.idBus);
    strcpy(busBaru.platNomor, busNode->Info.platNomor);
    busBaru.kapasitas = busNode->Info.kapasitas;

    // Ambil data editan dari user
    inputDataBusBaru(&busBaru);

    // Hapus rute lama
    hapusRute(&busBaru.rute);

    // --- Input rute baru ---
    terminalTree T;
    CreateTerminal(T);  // Inisialisasi terminal
    int root = 0;

    char terminalAwal[50], terminalTujuan[50];
    printf("Masukkan Terminal Awal: ");
    while ((getchar()) != '\n');  // flush stdin
    fgets(terminalAwal, sizeof(terminalAwal), stdin);
    terminalAwal[strcspn(terminalAwal, "\n")] = '\0';

    printf("Masukkan Terminal Tujuan: ");
    fgets(terminalTujuan, sizeof(terminalTujuan), stdin);
    terminalTujuan[strcspn(terminalTujuan, "\n")] = '\0';

    address awal = findRuteAwal(T, root, terminalAwal);
    NodeRute* tailRute = NULL;

    if (awal != nil && PreOrderToLinkedList(T, awal, terminalTujuan, &busBaru.rute, busBaru.kedatangan)) {
        printf("✅ Rute berhasil dibentuk.\n");
    } else {
        printf("❌ Rute tidak valid atau tidak ditemukan.\n");
        busBaru.rute = NULL;
    }

    // Salin ulang isi file tanpa baris yang diedit
    FILE *src = fopen("FileManajemen/dataBus.txt", "r");
    FILE *temp = fopen("FileManajemen/temp.txt", "w");
    if (!src || !temp) {
        printf("Gagal membuka file.\n");
        if (src) fclose(src);
        if (temp) fclose(temp);
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), src)) {
        char id[20];
        sscanf(line, "%19[^|]", id);
        if (strcmp(id, idBus) != 0) {
            fputs(line, temp);
        }
    }

    // Tambahkan data baru ke file temp
    char ruteStr[512] = "";
    ruteToString(busBaru.rute, ruteStr, sizeof(ruteStr));

    struct tm *wktBrk = localtime(&busBaru.keberangkatan);
    struct tm *wktTba = localtime(&busBaru.kedatangan);

    fprintf(temp, "%s|%s|%s|%d|%c|%s|%02d:%02d|%02d:%02d\n",
        busBaru.idBus,
        busBaru.platNomor,
        busBaru.namaSupir,
        busBaru.kapasitas,
        busBaru.kelas,
        ruteStr,
        wktBrk->tm_hour, wktBrk->tm_min,
        wktTba->tm_hour, wktTba->tm_min
    );

    fclose(src);
    fclose(temp);
    remove("FileManajemen/dataBus.txt");
    rename("FileManajemen/temp.txt", "FileManajemen/dataBus.txt");

    printf("✅ Data bus berhasil diperbarui!\n");
    tampilkanDataBus(busBaru);
}

NodeBus* searchBusByID(char idBus[]) {
    FILE *file = fopen("FileManajemen/dataBus.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file dataBus.txt\n");
        return NULL;
    }

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        char id[20], plat[30], nama[50], rute[200], kelas;
        int kapasitas;
        char waktuBrk[6], waktuTba[6];

        line[strcspn(line, "\r\n")] = '\0';

        int jumlahField = sscanf(line, "%19[^|]|%29[^|]|%49[^|]|%d|%c|%199[^|]|%5[^|]|%5[^|]",
                                 id, plat, nama, &kapasitas, &kelas, rute, waktuBrk, waktuTba);

        if (jumlahField != 8) continue;

        if (strcmp(id, idBus) == 0) {
            DataBus temp;
            strcpy(temp.idBus, id);
            strcpy(temp.platNomor, plat);
            strcpy(temp.namaSupir, nama);
            temp.kapasitas = kapasitas;
            temp.kelas = kelas;
            temp.rute = stringToRute(rute);

            int jam, menit;
            struct tm waktu = {0};
            sscanf(waktuBrk, "%d:%d", &jam, &menit);
            waktu.tm_year = 123; waktu.tm_mon = 0; waktu.tm_mday = 1;
            waktu.tm_hour = jam; waktu.tm_min = menit;
            temp.keberangkatan = mktime(&waktu);

            sscanf(waktuTba, "%d:%d", &jam, &menit);
            waktu.tm_hour = jam; waktu.tm_min = menit;
            temp.kedatangan = mktime(&waktu);

            NodeBus* result = alokasiNodeBus(temp);
            fclose(file);
            return result;  // LANGSUNG RETURN!
        }
    }

    fclose(file);
    return NULL;
}



void hapusRute(NodeRute** head) {
    NodeRute* curr = *head;
    while (curr != NULL) {
        NodeRute* temp = curr;
        curr = curr->next;
        free(temp);
    }
    *head = NULL;
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

    char line[512];
    char id[20], plat[30], nama[50], rute[200], kelas;
    int kapasitas;
    char waktuBrk[6], waktuTba[6];
    boolean found = false;

    while (fgets(line, sizeof(line), file)) {
        // Hapus newline di akhir baris
        line[strcspn(line, "\r\n")] = '\0';

        int jumlahField = sscanf(line, "%19[^|]|%29[^|]|%49[^|]|%d|%c|%199[^|]|%5[^|]|%5[^|]",
                                 id, plat, nama, &kapasitas, &kelas, rute, waktuBrk, waktuTba);

        if (jumlahField != 8) {
            // Format tidak sesuai, tetap tulis
            fprintf(temp, "%s\n", line);
            continue;
        }

        if (strcmp(id, idBus) == 0) {
            found = true;
            // Lewati penulisan (baris dihapus)
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


// Fungsi baru: konversi dari linked list rute ke string
void ruteToString(NodeRute* rute, char* buffer, size_t bufferSize) {
    buffer[0] = '\0';
    while (rute != NULL) {
        // Pakai strncat dengan sisa kapasitas buffer
        strncat(buffer, rute->namaTerminal, bufferSize - strlen(buffer) - 1);
        if (rute->next != NULL) {
            strncat(buffer, "->", bufferSize - strlen(buffer) - 1);
        }
        rute = rute->next;
    }
}

// Fungsi baru: konversi dari string ke linked list rute
// Pseudocode for stringToRute function fix
NodeRute* stringToRute(char* rute) {
    NodeRute* head = NULL;
    NodeRute* tail = NULL;

    char* terminal = strtok(rute, "->");
    while (terminal != NULL) {
        // Extract terminal name and time
        char terminalName[100];
        char time[10];
        sscanf(terminal, "%[^ ] %s", terminalName, time);

        NodeRute* newNode = (NodeRute*)malloc(sizeof(NodeRute));
        strcpy(newNode->namaTerminal, terminalName);
        newNode->next = NULL;

        if (tail == NULL) {
            head = newNode;
        } else {
            tail->next = newNode;
        }
        tail = newNode;

        terminal = strtok(NULL, "->");
    }
    return head;
}

void saveSingleBusToFile(DataBus bus) {
    if (strlen(bus.idBus) == 0 || strlen(bus.platNomor) == 0 || strlen(bus.namaSupir) == 0) {
        printf("❌ Data tidak lengkap. Gagal menyimpan bus!\n");
        return;
    }

    if (!(bus.kelas == 'A' || bus.kelas == 'B' || bus.kelas == 'C')) {
        printf("❌ Kelas bus tidak valid. Gagal menyimpan bus!\n");
        return;
    }

    FILE *file = fopen("FileManajemen/dataBus.txt", "a");
    if (file == NULL) {
        printf("Gagal membuka file untuk menyimpan data.\n");
        return;
    }

    struct tm wktBrkStruct = *localtime(&bus.keberangkatan);
    struct tm wktTbaStruct = *localtime(&bus.kedatangan);

    // Menyusun rute dengan waktu keberangkatan terminal
    char ruteStr[1024] = "";
    NodeRute* curr = bus.rute;
    while (curr != NULL) {
        struct tm *wktTerminal = localtime(&curr->waktubrgkt);  // Waktu keberangkatan setiap terminal
        char waktuStr[10];
        sprintf(waktuStr, "%02d:%02d", wktTerminal->tm_hour, wktTerminal->tm_min);  // Format waktu
        strcat(ruteStr, curr->namaTerminal);
        strcat(ruteStr, " (");
        strcat(ruteStr, waktuStr);
        strcat(ruteStr, ")");
        if (curr->next != NULL) strcat(ruteStr, "->");
        curr = curr->next;
    }

    // Simpan data bus dan rutenya ke file
    fprintf(file, "%s|%s|%s|%d|%c|%s|%02d:%02d|%02d:%02d\n",
        bus.idBus,
        bus.platNomor,
        bus.namaSupir,
        bus.kapasitas,
        bus.kelas,
        ruteStr,
        wktBrkStruct.tm_hour, wktBrkStruct.tm_min,
        wktTbaStruct.tm_hour, wktTbaStruct.tm_min
    );

    fclose(file);
}

void printAllBus() {
    FILE *file = fopen("FileManajemen/dataBus.txt", "r");
    if (file == NULL) {
        printf("File dataBus.txt tidak ditemukan atau gagal dibuka.\n");
        return;
    }

    char line[256];
    printf("=============================================================================================================================\n");
    printf("| %-6s | %-12s | %-15s | %-50s | %-9s | %-5s | %-10s | %-10s |\n",
           "ID", "Plat", "Supir", "Rute", "Kapasitas", "Kelas", "Brkt", "Tiba");
    printf("=============================================================================================================================\n");


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

boolean PreOrder(terminalTree P, address idx, char* tujuan) {
    if (idx == nil) return false;

    if (strcmp(P[idx].info, tujuan) == 0) {
        return true; // Terminal ditemukan
    }

    address child = P[idx].ps_fs;
    while (child != nil) {
        if (PreOrder(P, child, tujuan)) {
            return true; // Terminal ditemukan di subtree
        }
        child = P[child].ps_nb;
    }

    return false; // Terminal tidak ditemukan
}

boolean PreOrderToLinkedList(terminalTree T, address idx, char* tujuan, NodeRute** rute, time_t waktuTujuan) {
    static int offsetPergi = 0;
    static int offsetPulang = 0;
    static boolean found = false;

    if (idx == nil) return false;

    // Jika terminal tujuan ditemukan
    if (strcmp(T[idx].info, tujuan) == 0) {
        // Tambahkan node untuk perjalanan pergi
        NodeRute* newNode = (NodeRute*)malloc(sizeof(NodeRute));
        if (newNode == NULL) {
            printf("Gagal alokasi memori untuk rute.\n");
            return false;
        }

        // Salin nama terminal dan hitung waktu keberangkatan untuk pergi
        strcpy(newNode->namaTerminal, T[idx].info);
        struct tm tempWaktu = *localtime(&waktuTujuan);
        tempWaktu.tm_min -= offsetPergi;  // Mengurangi waktu untuk setiap terminal
        newNode->waktubrgkt = mktime(&tempWaktu);
        newNode->next = *rute;
        *rute = newNode;

        offsetPergi += 40;  // Menambah offset untuk terminal berikutnya
        found = true;

        // Sekarang buat rute balik dari terminal ini
        NodeRute* balikNode = (NodeRute*)malloc(sizeof(NodeRute));
        if (balikNode == NULL) {
            printf("Gagal alokasi memori untuk rute balik.\n");
            return true;  // Rute pergi sudah selesai, lanjutkan
        }

        // Salin nama terminal untuk rute balik
        strcpy(balikNode->namaTerminal, T[idx].info);
        struct tm waktuBalik = *localtime(&waktuTujuan);
        waktuBalik.tm_min += offsetPulang;  // Menambahkan waktu untuk perjalanan balik
        balikNode->waktubrgkt = mktime(&waktuBalik);
        balikNode->next = NULL;

        // Taruh di ujung list
        NodeRute* temp = *rute;
        while (temp->next != NULL) temp = temp->next;
        temp->next = balikNode;

        offsetPulang += 40;  // Menambah offset untuk rute balik selanjutnya
        return true;
    }

    // Traversal untuk mencari terminal tujuan pada pohon
    address child = T[idx].ps_fs;
    while (child != nil) {
        if (PreOrderToLinkedList(T, child, tujuan, rute, waktuTujuan)) {
            // Tambahkan node untuk perjalanan pergi
            NodeRute* newNode = (NodeRute*)malloc(sizeof(NodeRute));
            if (newNode == NULL) {
                printf("Gagal alokasi memori untuk rute.\n");
                return true;
            }

            strcpy(newNode->namaTerminal, T[idx].info);
            struct tm tempWaktu = *localtime(&waktuTujuan);
            tempWaktu.tm_min -= offsetPergi;
            newNode->waktubrgkt = mktime(&tempWaktu);
            newNode->next = *rute;
            *rute = newNode;

            offsetPergi += 40;

            // Tambahkan node balik jika terminal tujuan sudah ditemukan
            if (found) {
                NodeRute* balikNode = (NodeRute*)malloc(sizeof(NodeRute));
                if (balikNode == NULL) {
                    printf("Gagal alokasi memori untuk rute balik.\n");
                    return true;
                }

                strcpy(balikNode->namaTerminal, T[idx].info);
                struct tm waktuBalik = *localtime(&waktuTujuan);
                waktuBalik.tm_min += offsetPulang;
                balikNode->waktubrgkt = mktime(&waktuBalik);
                balikNode->next = NULL;

                // Taruh di ujung list
                NodeRute* temp = *rute;
                while (temp->next != NULL) temp = temp->next;
                temp->next = balikNode;

                offsetPulang += 40;
            }

            return true;
        }
        child = T[child].ps_nb;
    }

    return false;
}
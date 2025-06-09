#include <limits.h>
#include <malloc.h>
#include "../Header/bus.h"
#include "../Header/terminal.h"
#include <string.h>

NodeBus *HeadBus = Nil;
NodeRute *HeadRute = Nil;

boolean isBusListEmpty() {
    return (HeadBus == Nil);
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

    busBaru->Info.status = 0; // Status bus aktif
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
    baru->next = *head;
    *head = baru;
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
            temp.rute = strToRute(rute);

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
            return result;
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

    char line[2048];
    char id[20], plat[30], nama[50], rute[2000], kelas;
    int kapasitas;
    char waktuBrk[6], waktuTba[6];
    int statusInt;
    boolean status;
    boolean found = false;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = '\0';

        int jumlahField = sscanf(line, "%19[^|]|%29[^|]|%49[^|]|%d|%c|%1999[^|]|%5[^|]|%5[^|]|%d",
                                 id, plat, nama, &kapasitas, &kelas, rute, waktuBrk, waktuTba, &statusInt);
        status = (statusInt != 0);

        if (jumlahField != 9) {
            fprintf(temp, "%s\n", line);
            continue;
        }

        if (strcmp(id, idBus) == 0) {
            found = true;
            continue;
        }

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

void ruteToString(NodeRute* rute, char* buffer, size_t bufferSize) {
    buffer[0] = '\0';
    while (rute != NULL) {
        strncat(buffer, rute->namaTerminal, bufferSize - strlen(buffer) - 1);
        if (rute->next != NULL) {
            strncat(buffer, "->", bufferSize - strlen(buffer) - 1);
        }
        rute = rute->next;
    }
}

NodeRute* strToRute(char* rute) {
    NodeRute* head = NULL;
    NodeRute* tail = NULL;

    char* terminal = strtok(rute, "->");
    while (terminal != NULL) {
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

    char ruteStr[1024] = "";
    NodeRute* curr = bus.rute;
    while (curr != NULL) {
        struct tm *wktTerminal = localtime(&curr->waktubrgkt);
        char waktuStr[10];
        sprintf(waktuStr, "%02d:%02d", wktTerminal->tm_hour, wktTerminal->tm_min);
        strcat(ruteStr, curr->namaTerminal);
        strcat(ruteStr, " (");
        strcat(ruteStr, waktuStr);
        strcat(ruteStr, ")");
        if (curr->next != NULL) strcat(ruteStr, "->");
        curr = curr->next;
    }

        fprintf(file, "%s|%s|%s|%d|%c|%s|%02d:%02d|%02d:%02d|%d\n",
        bus.idBus,
        bus.platNomor,
        bus.namaSupir,
        bus.kapasitas,
        bus.kelas,
        ruteStr,
        wktBrkStruct.tm_hour, wktBrkStruct.tm_min,
        wktTbaStruct.tm_hour, wktTbaStruct.tm_min,
        bus.status 
    );


    fclose(file);
}

void printAllBus() {
    FILE *file = fopen("FileManajemen/dataBus.txt", "r");
    if (file == NULL) {
        printf("File dataBus.txt tidak ditemukan atau gagal dibuka.\n");
        return;
    }

    char line[1024];
    printf("===========================================================================================================================================================================\n");
    printf("| %-6s | %-10s | %-15s | %-9s | %-5s | %-60s | %-8s | %-8s | %-5s |\n",
           "ID", "Plat", "Supir", "Kapasitas", "Kelas", "Rute", "Brkt", "Tiba");
    printf("===========================================================================================================================================================================\n");
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;

        char *idBus = strtok(line, "|");
        char *platNomor = strtok(NULL, "|");
        char *namaSupir = strtok(NULL, "|");
        char *kapasitasStr = strtok(NULL, "|");
        char *kelasStr = strtok(NULL, "|");
        char *rute = strtok(NULL, "|");
        char *keberangkatan = strtok(NULL, "|");
        char *kedatangan = strtok(NULL, "|");
        char *status = strtok(NULL, "|");


        if (!idBus || !platNomor || !namaSupir || !kapasitasStr || !kelasStr || !rute || !keberangkatan || !kedatangan || !status) {
            continue;
        }

        int kapasitas = atoi(kapasitasStr);
        char kelas = kelasStr[0];

        printf("| %-6s | %-10s | %-15s | %-9d | %-5c | %-60s | %-8s | %-8s | %-5s |\n",
               idBus,
               platNomor,
               namaSupir,
               kapasitas,
               kelas,
               rute,
               keberangkatan,
               kedatangan,
               status);
        printf("\n");
    }

    printf("===========================================================================================================================================================================\n");
    fclose(file);
}

boolean PreOrder(terminalTree P, address idx, char* tujuan) {
    if (idx == nil) return false;

    if (strcmp(P[idx].info, tujuan) == 0) {
        return true;
    }

    address child = P[idx].ps_fs;
    while (child != nil) {
        if (PreOrder(P, child, tujuan)) {
            return true;
        }
        child = P[child].ps_nb;
    }

    return false;
}

boolean PreOrderToLinkedList(terminalTree T, address idx, char* tujuan, NodeRute** rute, time_t waktuTujuan) {
    static int offsetPergi = 0;
    static int offsetPulang = 0;
    static boolean found = false;

    if (idx == nil) return false;

    if (strcmp(T[idx].info, tujuan) == 0) {
        NodeRute* newNode = (NodeRute*)malloc(sizeof(NodeRute));
        if (newNode == NULL) {
            printf("Gagal alokasi memori untuk rute.\n");
            return false;
        }
        strcpy(newNode->namaTerminal, T[idx].info);
        strcat(newNode->namaTerminal, "(pergi)");
        struct tm tempWaktu = *localtime(&waktuTujuan);
        tempWaktu.tm_min -= offsetPergi;
        newNode->waktubrgkt = mktime(&tempWaktu);
        newNode->next = *rute;
        *rute = newNode;

        offsetPergi += 40;
        found = true;

        NodeRute* balikNode = (NodeRute*)malloc(sizeof(NodeRute));
        if (balikNode == NULL) {
            printf("Gagal alokasi memori untuk rute balik.\n");
            return true;
        }
        strcpy(balikNode->namaTerminal, T[idx].info);
        strcat(balikNode->namaTerminal, "(pulang)");
        struct tm waktuBalik = *localtime(&waktuTujuan);
        waktuBalik.tm_min += offsetPulang;
        balikNode->waktubrgkt = mktime(&waktuBalik);
        balikNode->next = NULL;

        NodeRute* temp = *rute;
        while (temp->next != NULL) temp = temp->next;
        temp->next = balikNode;

        offsetPulang += 40;
        return true;
    }

    address child = T[idx].ps_fs;
    while (child != nil) {
        if (PreOrderToLinkedList(T, child, tujuan, rute, waktuTujuan)) {
            NodeRute* newNode = (NodeRute*)malloc(sizeof(NodeRute));
            if (newNode == NULL) {
                printf("Gagal alokasi memori untuk rute.\n");
                return true;
            }
            strcpy(newNode->namaTerminal, T[idx].info);
            strcat(newNode->namaTerminal, " (pergi)");
            struct tm tempWaktu = *localtime(&waktuTujuan);
            tempWaktu.tm_min -= offsetPergi;
            newNode->waktubrgkt = mktime(&tempWaktu);
            newNode->next = *rute;
            *rute = newNode;

            offsetPergi += 40;

            if (found) {
                NodeRute* balikNode = (NodeRute*)malloc(sizeof(NodeRute));
                if (balikNode == NULL) {
                    printf("Gagal alokasi memori untuk rute balik.\n");
                    return true;
                }
                strcpy(balikNode->namaTerminal, T[idx].info);
                strcat(balikNode->namaTerminal, " (pulang)");
                struct tm waktuBalik = *localtime(&waktuTujuan);
                waktuBalik.tm_min += offsetPulang;
                balikNode->waktubrgkt = mktime(&waktuBalik);
                balikNode->next = NULL;

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

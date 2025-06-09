#include <limits.h>
#include <malloc.h>
#include "../Header/terminal.h"
#include <string.h>


void CreateTerminal(terminalTree X)
{
    X[0].info = "Dago";           X[0].ps_fs = 1;  X[0].ps_nb = nil; X[0].ps_pr = nil;
    X[1].info = "Simpang Dago";   X[1].ps_fs = 2;  X[1].ps_nb = 6;   X[1].ps_pr = 0;

    // Jalur kiri (Lembang)
    X[2].info = "Ciumbuleuit";    X[2].ps_fs = 3;  X[2].ps_nb = 4;   X[2].ps_pr = 1;
    X[3].info = "Punclut";        X[3].ps_fs = 5;  X[3].ps_nb = nil; X[3].ps_pr = 2;
    X[4].info = "Setiabudi";      X[4].ps_fs = 7;  X[4].ps_nb = nil; X[4].ps_pr = 1;
    X[5].info = "Lembang";        X[5].ps_fs = nil; X[5].ps_nb = nil; X[5].ps_pr = 3;
    // Jalur kiri-tengah
    X[6].info = "Ledeng";         X[6].ps_fs = 8;  X[6].ps_nb = nil; X[6].ps_pr = 4;
    X[7].info = "Parongpong";     X[7].ps_fs = nil; X[7].ps_nb = nil; X[7].ps_pr = 6;

    // Jalur tengah (KBP)
    X[8].info = "Pasteur";        X[8].ps_fs = 9;  X[8].ps_nb = 15;  X[8].ps_pr = 0;
    X[9].info = "Sukajadi";       X[9].ps_fs = 10; X[9].ps_nb = nil; X[9].ps_pr = 8;
    X[10].info = "Pajajaran";     X[10].ps_fs = 11; X[10].ps_nb = nil; X[10].ps_pr = 9;
    X[11].info = "Cicendo";       X[11].ps_fs = 12; X[11].ps_nb = nil; X[11].ps_pr = 10;
    X[12].info = "Cimindi";       X[12].ps_fs = 13; X[12].ps_nb = nil; X[12].ps_pr = 11;
    X[13].info = "Cimahi Tengah"; X[13].ps_fs = 14; X[13].ps_nb = nil; X[13].ps_pr = 12;
    X[14].info = "Cimahi Utara";  X[14].ps_fs = 16; X[14].ps_nb = nil; X[14].ps_pr = 13;
    X[15].info = "KBP";           X[15].ps_fs = nil; X[15].ps_nb = nil; X[15].ps_pr = 14;

    // Jalur kanan (Cibiru)
    X[16].info = "Gasibu";            X[16].ps_fs = 17; X[16].ps_nb = nil; X[16].ps_pr = 0;
    X[17].info = "Cibeunying Kaler";  X[17].ps_fs = 18; X[17].ps_nb = nil; X[17].ps_pr = 16;
    X[18].info = "Antapani";          X[18].ps_fs = 19; X[18].ps_nb = nil; X[18].ps_pr = 17;
    X[19].info = "Ujung Berung";      X[19].ps_fs = 20; X[19].ps_nb = nil; X[19].ps_pr = 18;
    X[20].info = "Cibiru";            X[20].ps_fs = nil; X[20].ps_nb = nil; X[20].ps_pr = 19;
}


void tampilkanTerminal(terminalTree T) {
    printf("Daftar Terminal:\n");
    for (int i = 0; i < jml_maks; i++) {
        if (T[i].info != NULL && strlen(T[i].info) > 0) {
            printf("%d. %s\n", i, T[i].info);
        }
    }
}

address findRuteAwal(terminalTree T, address idx, char* awal) {
    if (idx == nil) return nil;
    if (T[idx].info && strcmp(T[idx].info, awal) == 0) return idx;

    address hasil = findRuteAwal(T, T[idx].ps_fs, awal);
    if (hasil != nil) return hasil;

    return findRuteAwal(T, T[idx].ps_nb, awal);
}

void TampilkanRuteBalik(terminalTree T, address idxTujuan, char* awal) {
    address now = idxTujuan;
    while (now != nil) {
        printf("-> %s\n", T[now].info);
        if (T[now].info && strcmp(T[now].info, awal) == 0) break;
        now = T[now].ps_pr;
    }
    if (now == nil) printf("Terminal awal tidak ditemukan dalam jalur balik.\n");
}

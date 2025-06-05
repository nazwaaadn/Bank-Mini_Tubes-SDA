#include <limits.h>
#include <malloc.h>
#include "../Header/terminal.h"
#include <string.h>

void CreateTerminal(terminalTree X)
{

    X[0].info = "Dago"; X[0].ps_fs = 1;               
    X[1].info = "Simpang Dago"; X[1].ps_fs = 6; X[1].ps_nb = 2; X[1].ps_pr = 0;  
    X[2].info = "Pasteur"; X[2].ps_fs = 8; X[2].ps_nb = 3; X[2].ps_pr = 0; 
    X[3].info = "Gasibu"; X[3].ps_fs = 9; X[3].ps_nb = 4; X[3].ps_pr = 0; 
    X[4].info = "Gedung Sate"; X[4].ps_fs = 10; X[4].ps_nb = 5; X[4].ps_pr = 0; 
    X[5].info = "Tamansari"; X[5].ps_fs = 11; X[5].ps_nb = nil; X[5].ps_pr = 0; 
    X[6].info = "Ciumbuleuit"; X[6].ps_fs = 12; X[6].ps_nb = 7; X[6].ps_pr = 1; 
    X[7].info = "Setiabudi"; X[7].ps_fs = 13; X[7].ps_nb = nil; X[7].ps_pr = 1; 
    X[8].info = "Sukajadi"; X[8].ps_fs = 14; X[8].ps_nb = nil; X[8].ps_pr = 2; 
    X[9].info = "Cibeunying Kaler"; X[9].ps_fs = 15; X[9].ps_nb = nil; X[9].ps_pr = 3; 
    X[10].info = "Cibeunying Kidul"; X[10].ps_fs = 16; X[10].ps_nb = 2; X[10].ps_pr = 4;  
    X[11].info = "Balubur"; X[11].ps_fs = 17; X[11].ps_nb = nil; X[11].ps_pr = 5; 
    X[12].info = "Punclut"; X[12].ps_fs = 18; X[12].ps_nb = nil; X[12].ps_pr = 6; 
    X[13].info = "Ledeng"; X[13].ps_fs = 19; X[13].ps_nb = nil; X[13].ps_pr = 7; 
    X[14].info = "Pajajaran"; X[14].ps_fs = 20; X[14].ps_nb = nil; X[14].ps_pr = 8; 
    X[15].info = "Antapani"; X[15].ps_fs = 21; X[15].ps_nb = nil; X[15].ps_pr = 9; 
    X[16].info = "Kiaracondong"; X[16].ps_fs = 22; X[16].ps_nb = nil; X[16].ps_pr = 10; 
    X[17].info = "Tamansari Bawah"; X[17].ps_fs = 23; X[17].ps_nb = nil; X[17].ps_pr = 11; 
    X[18].info = "Lembang"; X[18].ps_fs = nil; X[18].ps_nb = nil; X[18].ps_pr = 12; 
    X[19].info = "Parongpong"; X[19].ps_fs = nil; X[19].ps_nb = nil; X[19].ps_pr = 13;  
    X[20].info = "Cicendo"; X[20].ps_fs = 24; X[20].ps_nb = nil; X[20].ps_pr = 14; 
    X[21].info = "Ujungberung"; X[21].ps_fs = 25; X[21].ps_nb = nil; X[21].ps_pr = 15; 
    X[22].info = "Arcamanik"; X[22].ps_fs = 16; X[22].ps_nb = nil; X[22].ps_pr = 26; 
    X[23].info = "Jalan Asia Afrika"; X[23].ps_fs = 27; X[23].ps_nb = nil; X[23].ps_pr = 17; 
    X[24].info = "Cimindi"; X[24].ps_fs = 20; X[24].ps_nb = nil; X[24].ps_pr = 29; 
    X[25].info = "Cibiru"; X[25].ps_fs = 21; X[25].ps_nb = nil; X[25].ps_pr = 30; 
    X[26].info = "Cinambo"; X[26].ps_fs = nil; X[26].ps_nb = nil; X[26].ps_pr = 22; 
    X[27].info = "Sumur Bandung"; X[27].ps_fs = 31; X[27].ps_nb = 28; X[27].ps_pr = 23; 
    X[28].info = "Moh Toha"; X[28].ps_fs = 32; X[28].ps_nb = nil; X[28].ps_pr = 23; 
    X[29].info = "Cimahi Tengah"; X[29].ps_fs = 33; X[29].ps_nb = nil; X[29].ps_pr = 24;  
    X[30].info = "Cileunyi"; X[30].ps_fs = 34; X[30].ps_nb = nil; X[30].ps_pr = 25; 
    X[31].info = "Regol"; X[31].ps_fs = 35; X[31].ps_nb = nil; X[31].ps_pr = 27; 
    X[32].info = "Soreang"; X[32].ps_fs = 36; X[32].ps_nb = nil; X[32].ps_pr = 28; 
    X[33].info = "Cimahi Utara"; X[33].ps_fs = 37; X[33].ps_nb = nil; X[33].ps_pr = 29; 
    X[34].info = "Jatinangor"; X[34].ps_fs = nil; X[34].ps_nb = nil; X[34].ps_pr = 30; 
    X[35].info = "Buah Batu"; X[35].ps_fs = 38; X[35].ps_nb = nil; X[35].ps_pr = 31; 
    X[36].info = "Katapang"; X[36].ps_fs = 39; X[36].ps_nb = nil; X[36].ps_pr = 32; 
    X[37].info = "KBP"; X[37].ps_fs = nil; X[37].ps_nb = nil; X[37].ps_pr = 33; 
    X[38].info = "Bojong Soang"; X[38].ps_fs = 40; X[38].ps_nb = nil; X[38].ps_pr = 35;  
    X[39].info = "Ciparay"; X[39].ps_fs = nil; X[39].ps_nb = nil; X[39].ps_pr = 36; 
    X[40].info = "Baleendah"; X[40].ps_fs = 41; X[40].ps_nb = nil; X[40].ps_pr = 38; 
    X[41].info = "DayeuhKolot"; X[41].ps_fs = 42; X[41].ps_nb = nil; X[41].ps_pr = 40; 
    X[42].info = "Banjaran"; X[42].ps_fs = nil; X[42].ps_nb = nil; X[42].ps_pr = 41;  
}

boolean PreOrder(terminalTree P, address idx, char* tujuan, char* bufferRute) {
    if (idx == nil) return false;

    if (strlen(bufferRute) > 0) strcat(bufferRute, " -> ");
    strcat(bufferRute, P[idx].info);
    printf("-> %s\n", P[idx].info);

    if (strcmp(P[idx].info, tujuan) == 0) return true;

    if (PreOrder(P, P[idx].ps_fs, tujuan, bufferRute)) return true;
    if (PreOrder(P, P[idx].ps_nb, tujuan, bufferRute)) return true;

    return false;
}



address findRuteAwal(terminalTree T, address idx, char* awal) {
    if (idx == nil) return nil;

    if (strcmp(T[idx].info, awal) == 0) {
        return idx;
    }

    address hasil = findRuteAwal(T, T[idx].ps_fs, awal);
    if (hasil != nil) return hasil;

    return findRuteAwal(T, T[idx].ps_nb, awal);
}


void TampilkanRuteBalik(terminalTree T, address idxTujuan, char* awal) {
    address now = idxTujuan;

    while (now != nil) {
        printf("-> %s\n", T[now].info);
        if (strcmp(T[now].info, awal) == 0) {
            break; // berhenti saat sudah sampai ke awal
        }
        now = T[now].ps_pr; // naik ke parent
    }

    if (now == nil) {
        printf("Terminal awal tidak ditemukan dalam jalur balik.\n");
    }
}

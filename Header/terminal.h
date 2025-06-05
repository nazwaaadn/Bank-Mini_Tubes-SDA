#ifndef TERMINAL_H
#define TERMINAL_H
#define jml_maks 40
#define nil -1

#include <stdio.h>
#include "boolean.h"

typedef char* infotype;
typedef int address;
typedef struct { 
    infotype info;
    address ps_fs, ps_nb, ps_pr;
}nbtree;

typedef nbtree terminalTree[jml_maks+1];

void CreateTerminal(terminalTree X);
// boolean PreOrder (terminalTree P, address idx, char* tujuan);
/* Traversal PreOrder */
/* IS : P terdefinisi */
/* FS : Semua simpul P sudah ditampilkan secara PreOrder : Parent,
fs, nb */
address findRuteAwal(terminalTree T, address idx, char* awal);
boolean PreOrder(terminalTree P, address idx, char* tujuan, char* bufferRute);
void TampilkanRuteBalik(terminalTree T, address idxTujuan, char* awal);

#endif
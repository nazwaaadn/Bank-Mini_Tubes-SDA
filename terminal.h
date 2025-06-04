#ifndef TERMINAL_H
#define TERMINAL_H
#define jml_maks 40
#define nil NULL

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
boolean IsEmpty (terminalTree P);
/* Mengirimkan true jika terminalTree KOSONG */

/***** Traversal *****/
void PreOrder (terminalTree P);
/* Traversal PreOrder */
/* IS : P terdefinisi */
/* FS : Semua simpul P sudah ditampilkan secara PreOrder : Parent,
fs, nb */
void InOrder (terminalTree P);
/* Traversal InOrder */
/* IS : P terdefinisi */
/* FS : Semua simpul P sudah ditampilkan secara InOrder : fs, Parent,
Sisa anak lain */
void PrintTerminal (terminalTree P);

#endif
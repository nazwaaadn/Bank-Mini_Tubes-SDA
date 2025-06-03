#ifndef BUS_H
#define BUS_H
#define jml_maks 40
#define nil NULL

#include <stdio.h>

typedef char* infotype;
typedef int address;
typedef struct { 
    infotype info;
    address ps_fs, ps_nb, ps_pr;
}nbtree;
typedef nbtree terminalTree[jml_maks+1];

void CreateTerminal(terminalTree X);

#endif
#ifndef STACK_H
#define STACK_H
#define Info(P) (P)->info
#define Next(P) (P)->next
#define Nil NULL
#define Top(S) (S)->Top

#include "terminal.h"
#include "boolean.h"
#include <stdlib.h>

typedef struct tElmtStack *addressStack;
typedef struct tElmtStack {
    nbtree info;
    addressStack next;
} ElmtStack;

typedef struct {
    addressStack Top;
} Stack;

/* ==== FUNGSI & PROSEDUR ==== */
addressStack AlokasiStack(nbtree X);
void DealokasiStack(addressStack P);
void CreateEmptyStack(Stack *S);
boolean IsEmptyStack(Stack S);
void Push(Stack *S, nbtree X);
void Pop(Stack *S, nbtree *X);
void InsertTop(Stack *S, addressStack P);
void DelTop(Stack *S, addressStack *P);

#endif

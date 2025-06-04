#ifndef STACK_H
#define STACK_H
#include "BOOLEAN.h"
#include "tiket.h"
#include "time.h"
#define Nil NULL

typedef struct NodeStack {
    NodeTiket* tiket;           // Tiket yang disimpan di stack
    struct NodeStack* next;
} NodeStack;

typedef struct {
    NodeStack* top;
} Stack;

void SetTop (Stack *S, Stack NewTop );
void CreateEmpty (Stack *S);
boolean IsEmpty (Stack S);
void Push (Stack *S, NodeTiket* X);
void Pop (Stack *S, NodeTiket** X);

#endif


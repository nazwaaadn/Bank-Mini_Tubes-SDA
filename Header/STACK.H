// stack.h

#ifndef STACK_H
#define STACK_H

// Struktur Stack untuk menyimpan nama terminal
typedef struct Stack {
    char* terminals[100];  // Array untuk menyimpan nama terminal
    int top;  
} Stack;

// Fungsi untuk menginisialisasi stack
void initStack(Stack* stack);

// Fungsi untuk menambahkan terminal ke dalam stack
void push(Stack* stack, char* terminal);

// Fungsi untuk mengeluarkan terminal dari stack
char* pop(Stack* stack);

#endif // STACK_H
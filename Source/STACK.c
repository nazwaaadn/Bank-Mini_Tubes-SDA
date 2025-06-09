// stack.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Header/stack.h"

// Inisialisasi stack
void initStack(Stack* stack) {
    stack->top = -1;  // Stack kosong
}

// Fungsi untuk menambahkan (push) terminal ke dalam stack
void push(Stack* stack, char* terminal) {
    if (stack->top < 99) {
        stack->top++;
        stack->terminals[stack->top] = strdup(terminal);  // Salin terminal ke dalam stack
    }
}

// Fungsi untuk mengeluarkan (pop) terminal dari stack
char* pop(Stack* stack) {
    if (stack->top == -1) {
        return NULL;  // Stack kosong
    }
    char* terminal = stack->terminals[stack->top];
    stack->top--;
    return terminal;
}

// Fungsi untuk memeriksa apakah terminal mengandung kata "(pergi)"
int containsPergi(char* terminal) {
    return strstr(terminal, "(pergi)") != NULL;
}

// Fungsi untuk menghapus kata "(pergi)" dari nama terminal
void removePergi(char* terminal) {
    char* ptr = strstr(terminal, "(pergi)");
    if (ptr) {
        *ptr = '\0';  // Menghapus kata "(pergi)"
    }
}
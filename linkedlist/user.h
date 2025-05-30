#ifndef tiket_H
#define tiket_H
#include "BOOLEAN.h"
#include <stdio.h>
#include <time.h>
#define Nil NULL
#define Info(P) (P)->Info
#define Next(P) (P)->next

//user
typedef struct
{
    char nama[50]; 
	char password[20];            
    char alamat[100];          
    char noHP[15]; 
} DataUser;

typedef struct NodeUser {
    DataUser Info;               
    struct NodeTiket* transaksi;         
    struct NodeUser* next;      
} NodeUser;

extern NodeUser *HeadUser;

//admin

typedef struct
{
    char nama[50]; 
    char password[20];            
} DataAdmin;

void insertUser(DataUser userBaru);
void loginUser(char nama[], char password[]);
void loginAdmin(char nama[], char password[]);
void logoutUser();
void logoutAdmin();
void deleteTransaksi(NodeUser* user, NodeTiket* tiket);

#endif
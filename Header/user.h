#ifndef user_H
#define user_H
#include "boolean.h"
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
extern NodeUser* currentUser;



void Register();
void Login();
void insertUser(DataUser userBaru);
void loginUser(char nama[], char password[]);
void logoutUser();
// void deleteTransaksi(NodeUser* user, NodeTiket* tiket);

// tambahan
//
void menuPelanggan(NodeUser* user);  // ✅ supaya bisa dipanggil dari Login() di user.c
void pesanTiket(NodeUser* user);
void loadUsersFromFile(const char* filename);
void saveUsersToFile(const char* filename);
int isUsernameTaken(const char* nama);

#endif
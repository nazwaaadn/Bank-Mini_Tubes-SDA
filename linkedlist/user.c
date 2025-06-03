#include "user.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* USER_DB_FILENAME = "user_database.txt";

NodeUser *HeadUser = NULL;

void insertUser(DataUser userBaru) {
    NodeUser* newNode = (NodeUser*)malloc(sizeof(NodeUser));
    newNode->Info = userBaru;
    newNode->transaksi = NULL;
    newNode->next = NULL;

        if (HeadUser == NULL) {
            HeadUser = newNode;
        } else {
            NodeUser* temp = HeadUser;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }


int isUsernameTaken(const char* nama) {
    NodeUser* temp = HeadUser;
        while (temp != NULL) {
            if (strcmp(temp->Info.nama, nama) == 0) {
                return 1; // true, username sudah diambil
            }
            temp = temp->next;
        }
        return 0;
    }


void Register() {
    DataUser userBaru;

        printf("===== Registrasi Akun =====\n");
        printf("Masukkan Nama Pengguna : ");
        scanf(" %[^\n]", userBaru.nama);

        if (strlen(userBaru.nama) == 0 || isUsernameTaken(userBaru.nama)) {
            printf("Nama pengguna tidak valid atau sudah digunakan.\n");
            return;
        }

        printf("Masukkan Password       : ");
        scanf(" %[^\n]", userBaru.password);
        if (strlen(userBaru.password) == 0) {
            printf("Password tidak boleh kosong.\n");
            return;
        }

        printf("Masukkan Alamat         : ");
        scanf(" %[^\n]", userBaru.alamat);

        printf("Masukkan No. HP         : ");
        scanf(" %[^\n]", userBaru.noHP);

        insertUser(userBaru);

        FILE* file = fopen(USER_DB_FILENAME, "a");
        if (file != NULL) {
            fprintf(file, "%s;%s;%s;%s\n", userBaru.nama, userBaru.password, userBaru.alamat, userBaru.noHP);
            fclose(file);
            printf("Registrasi berhasil!\n");
        } else {
            printf("Gagal menyimpan ke file.\n");
        }
    }


void loginUser(char nama[], char password[]) {
    NodeUser* curr = HeadUser;
        while (curr != NULL) {
            if (strcmp(curr->Info.nama, nama) == 0 && strcmp(curr->Info.password, password) == 0) {
                currentUser = curr;
                printf("Login berhasil. Selamat datang, %s!\n", nama);
                return;
            }
            curr = curr->next;
        }
        printf("Login gagal. Nama atau password salah.\n");
    }


void Login() {
    char nama[50];
    char password[20];

        printf("===== Login Akun =====\n");
        printf("Nama Pengguna : ");
        scanf(" %[^\n]", nama);
        printf("Password      : ");
        scanf(" %[^\n]", password);

        loginUser(nama, password);
    }


void loadUsersFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
        if (file == NULL) {
            printf("Gagal membuka file %s\n", filename);
            return;
        }

        char buffer[256];
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            DataUser userBaru;
            if (sscanf(buffer, "%[^;];%[^;];%[^;];%[^\n]",
                    userBaru.nama,
                    userBaru.password,
                    userBaru.alamat,
                    userBaru.noHP) == 4) {
                insertUser(userBaru);
            }
        }

        fclose(file);
    }


void logoutUser() {
    currentUser = NULL;
    printf("Anda telah logout.\n");
}


    // void deleteTransaksi(NodeUser* user, NodeTiket* tiket) {
    //     // Fungsi ini bisa dikembangkan sesuai struktur NodeTiket
    //     printf("Fungsi deleteTransaksi belum diimplementasikan.\n");
    // }


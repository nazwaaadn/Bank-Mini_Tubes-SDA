#include "user.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* FileUser = "FileManajemen/userDatabase.txt";

NodeUser *HeadUser = NULL;
NodeUser* currentUser = NULL;

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
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

void Register() {
    DataUser userBaru;

    printf("\n===== REGISTRASI AKUN =====\n");

    printf("Masukkan Nama Pengguna : ");
    fgets(userBaru.nama, sizeof(userBaru.nama), stdin);
    userBaru.nama[strcspn(userBaru.nama, "\n")] = '\0';

    if (strlen(userBaru.nama) == 0 || isUsernameTaken(userBaru.nama)) {
        printf("Nama pengguna tidak valid atau sudah digunakan.\n");
        return;
    }

    printf("Masukkan Password       : ");
    fgets(userBaru.password, sizeof(userBaru.password), stdin);
    userBaru.password[strcspn(userBaru.password, "\n")] = '\0';

    if (strlen(userBaru.password) == 0) {
        printf("Password tidak boleh kosong.\n");
        return;
    }

    printf("Masukkan Alamat         : ");
    fgets(userBaru.alamat, sizeof(userBaru.alamat), stdin);
    userBaru.alamat[strcspn(userBaru.alamat, "\n")] = '\0';

    printf("Masukkan No. HP         : ");
    fgets(userBaru.noHP, sizeof(userBaru.noHP), stdin);
    userBaru.noHP[strcspn(userBaru.noHP, "\n")] = '\0';

    insertUser(userBaru);

    FILE* file = fopen(FileUser, "a");
    if (file != NULL) {
        fprintf(file, "%s;%s;%s;%s\n", userBaru.nama, userBaru.password, userBaru.alamat, userBaru.noHP);
        fclose(file);
        printf("Registrasi berhasil!\n");
    } else {
        printf("Gagal menyimpan ke file.\n");
    }
}

NodeUser* findUser(char username[], char password[]) {
    NodeUser* current = HeadUser;
    while (current != NULL) {
        if (strcmp(current->Info.nama, username) == 0 &&
            strcmp(current->Info.password, password) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void Login() {
    char nama[50];
    char password[20];

    printf("\n===== LOGIN AKUN =====\n");

    printf("Nama Pengguna : ");
    fgets(nama, sizeof(nama), stdin);
    nama[strcspn(nama, "\n")] = '\0';

    printf("Password      : ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    NodeUser* user = findUser(nama, password);
    if (user != NULL) {
        currentUser = user;
        printf("Login berhasil. Selamat datang, %s!\n", user->Info.nama);
        menuPelanggan(currentUser);
    } else {
        printf("Login gagal. Nama atau password salah.\n");
    }
}

void loadUsersFromFile(const char* FileUser) {
    FILE* file = fopen(FileUser, "r");
    if (file == NULL) {
        file = fopen(FileUser, "w");
        if (file != NULL) {
            fclose(file);
            printf("File %s dibuat karena belum ada.\n", FileUser);
        } else {
            printf("Gagal membuat file %s\n", FileUser);
        }
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

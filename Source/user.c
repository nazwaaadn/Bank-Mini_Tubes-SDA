#include "user.h"
#include "tiket.h"
#include "bus.h"
#include "desain.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* FileUser = "userDatabase.txt";


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

        FILE* file = fopen(FileUser, "a");
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
                afterLoginMenu(curr);
            }
            curr = curr->next;
        }
        printf("Login gagal. Nama atau password salah.\n");
    }

//Tambahan dari Naira buat ticket ==========================================

void afterLoginMenu(NodeUser* user) {
    int pilihan;
    char id[20];
    do {
        printf("\n+=======================================+\n");
        printf("|  === MENU PELANGGAN - %s === |\n", user->Info.nama);
        printf("+=======================================+\n");
        printf("| 1. Pesan Tiket                        |\n");
        printf("| 2. Cetak Tiket                        |\n");
        printf("| 3. Hapus Tiket                        |\n");
        printf("| 4. Batalkan Tiket                     |\n");
        printf("| 5. Tampilkan Semua Tiket              |\n");
        printf("| 0. Keluar                             |\n");
        printf("+=======================================+\n");
        printf(" Pilih menu (0-5): ");
        scanf("%d", &pilihan);
        getchar(); // newline handler

        switch (pilihan) {
            case 1:
                pesanTiket(user);  // Call ticket order function
                break;
            case 2:
                printf("Masukkan ID Tiket: ");
                scanf(" %[^\n]", id);
                cetakTiket(id);
                break;
            case 3:
                printf("Masukkan ID Tiket yang ingin dihapus: ");
                scanf(" %[^\n]", id);
                deleteTiketByID(id);
                break;
            case 4:
                printf("Masukkan ID Tiket yang ingin dibatalkan: ");
                scanf(" %[^\n]", id);
                batalkanTiket(id);
                break;
            case 5:
                printAllTiket();
                break;
            case 0:
                printf("Terima kasih telah menggunakan layanan Translibe!\n");
                break;
            default:
                printf("Pilihan tidak valid.\n");
        }
    } while (pilihan != 0);
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
//==========================================================================


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


void loadUsersFromFile(const char* FileUser) {
    FILE* file = fopen(FileUser, "r");
    if (file == NULL) {
        // Jika file tidak ada, buat file kosong agar tidak error di masa depan
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


    // void deleteTransaksi(NodeUser* user, NodeTiket* tiket) {
    //     // Fungsi ini bisa dikembangkan sesuai struktur NodeTiket
    //     printf("Fungsi deleteTransaksi belum diimplementasikan.\n");
    // }


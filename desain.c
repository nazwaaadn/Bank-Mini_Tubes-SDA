#include "library.h"
#include <stdio.h>

void SplashScreen(){
    printf("======================================================\n");
    printf("||                                                  ||\n");
    printf("||           SISTEM RUTE BUS TRANSJAKARTA           ||\n");
    printf("||                                                  ||\n");
    printf("||              ┌────────────────────┐              ||\n");
    printf("||              │   [   T-Jakarta   ]│              ||\n");
    printf("||              └────────────────────┘              ||\n");
    printf("||                                                  ||\n");
    printf("||            Pencarian & Manajemen Rute            ||\n");
    printf("||                                                  ||\n");
    printf("||             Dibuat oleh: Translibe               ||\n");
    printf("||                                                  ||\n");
    printf("||==================================================||\n");
    printf("||                  Masuk sebagai...                ||\n");
    printf("||--------------------------------------------------||\n");
    printf("||                     1. User                      ||\n");
    printf("||                     2. Admin                     ||\n");
    printf("||                     3. Keluar                    ||\n");
    printf("||==================================================||\n");
    printf("||         Masukkan Pilihan Anda [Input User]       ||\n");
    printf("======================================================\n");
}

void UserMenu()
{
    printf("===================================\n");
    printf("|        MENU UTAMA PELANGGAN     |\n");
    printf("===================================\n");
    printf("1. Pesan Tiket\n");
    printf("2. Lihat Tiket Aktif\n");
    printf("3. Riwayat Perjalanan\n");
    printf("4. Batalkan Tiket\n");
    printf("5. Keluar\n");
    printf("===================================\n");
    printf("Masukkan pilihan Anda: ");
}

void AdminMenu()
{
    
}

void ManajemenTerminal()
{
    printf("===============================================\n");
    printf("|            MANAJEMEN TERMINAL               |\n");
    printf("===============================================\n");
    printf("1. Tambah Terminal Baru\n");
    printf("2. Hapus Terminal\n");
    printf("3. Lihat Daftar Terminal\n");
    printf("4. Kembali ke Menu Utama\n");
    printf("===============================================\n");
    printf("Masukkan pilihan Anda: ");
}

void ManajemenBus()
{
    printf("===============================================\n");
    printf("|              MANAJEMEN BUS                  |\n");
    printf("===============================================\n");
    printf("1. Tambah Bus Baru\n");
    printf("2. Hapus Bus\n");
    printf("3. Edit Data Bus\n");
    printf("4. Lihat Daftar Bus\n");
    printf("5. Kembali ke Menu Utama\n");
    printf("===============================================\n");
    printf("Masukkan pilihan Anda: ");
}

void LihatRiwayat()
{
    
}

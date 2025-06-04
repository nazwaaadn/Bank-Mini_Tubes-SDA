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
    printf("5. Logout\n");
    printf("5. Keluar\n");
    printf("===================================\n");
    printf("Masukkan pilihan Anda: ");
}

void AdminMenu()
{
    printf("===================================\n");
    printf("|            MENU ADMIN           |\n");
    printf("===================================\n");
    printf("1. Lihat Bus di Setiap Terminal\n");
    printf("2. Release Bus dari Terminal\n");
    printf("3. Manajemen Bus\n");
    printf("4. Laporan Transaksi\n");
    printf("5. Laporan Perjalanan Bus\n");
    printf("6. Logout\n");
    printf("7. Keluar\n");
    printf("===================================\n");
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

void LihatBusDiTerminal() {
    printf("===============================================\n");
    printf("|         DAFTAR BUS DI SETIAP TERMINAL       |\n");
    printf("===============================================\n");
    // Placeholder, isi dengan logika di admin.c
}

void ReleaseBus() {
    printf("===============================================\n");
    printf("|           RELEASE BUS DARI TERMINAL         |\n");
    printf("===============================================\n");
    // Placeholder
}

void TambahBus() {
    printf("===============================================\n");
    printf("|               TAMBAH BUS BARU               |\n");
    printf("===============================================\n");
    // Placeholder
}

void AturRuteBus() {
    printf("===============================================\n");
    printf("|         ATUR RUTE BUS YANG KEMBALI          |\n");
    printf("===============================================\n");
    // Placeholder
}

void LihatDataTiketPelanggan() {
    printf("===============================================\n");
    printf("|         DATA TIKET PELANGGAN AKTIF          |\n");
    printf("===============================================\n");
    // Placeholder
}

void LaporanPerjalananBus() {
    printf("===============================================\n");
    printf("|         LAPORAN PERJALANAN BUS HARI INI     |\n");
    printf("===============================================\n");
    // Placeholder
}

void ManajemenAkunPelanggan() {
    printf("===============================================\n");
    printf("|          MANAJEMEN AKUN PELANGGAN           |\n");
    printf("===============================================\n");
    printf("1. Hapus Akun\n");
    printf("2. Lihat Semua Akun\n");
    printf("3. Reset Password\n");
    printf("4. Kembali ke Menu Admin\n");
    printf("===============================================\n");
    printf("Masukkan pilihan Anda: ");
}

void RiwayatTransaksiTiket() {
    printf("===============================================\n");
    printf("|          RIWAYAT TRANSAKSI TIKET            |\n");
    printf("===============================================\n");
    // Placeholder
}

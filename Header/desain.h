#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdio.h>

// ========== GLOBAL ==========
void SplashScreen(); // Tampilan awal program

// ========== MODUL USER ==========
void UserMenu();             // Menu utama pelanggan
void PesanTiket();           // Pesan tiket oleh user
void LihatTiketAktif();      // Lihat tiket yang sedang aktif
void RiwayatPerjalanan();    // Tampilkan riwayat perjalanan user
void BatalkanTiket();        // Batalkan tiket yang sudah dipesan

// ========== MODUL ADMIN ==========
void AdminMenu();                   // Menu utama admin
void LihatBusDiTerminal();         // Lihat semua bus per terminal
void ReleaseBus();                 // Melepas bus dari terminal
void TambahBus();                  // Menambahkan bus baru
void AturRuteBus();                // Mengatur rute bus yang kembali
void LihatDataTiketPelanggan();    // Melihat semua tiket pelanggan
void LaporanPerjalananBus();       // Menampilkan laporan perjalanan
void ManajemenTerminal();          // Menampilkan menu manajemen terminal
void ManajemenAkunPelanggan();     // Mengelola data user
void RiwayatTransaksiTiket();      // Menampilkan semua riwayat transaksi tiket

#endif

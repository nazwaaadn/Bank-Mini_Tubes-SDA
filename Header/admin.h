#ifndef ADMIN_H
#define ADMIN_H

#include "queue.h"

// Login admin
int LoginAdmin();

// Fungsi-fungsi aksi berdasarkan menu
void LihatBusDiSetiapTerminal(Queue* q);
void ReleaseBusDariTerminal(Queue* q);
void TambahBusBaru();
void LihatDataTiketPelanggan();
void RiwayatTransaksi();

#endif

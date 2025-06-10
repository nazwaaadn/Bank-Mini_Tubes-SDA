#ifndef ADMIN_H
#define ADMIN_H

#include "queue.h"

// Login admin
int LoginAdmin();

// Fitur menu admin:
void LihatBusDiSetiapTerminal(Queue* q);        // Lihat bus dalam antrian
void ReleaseBusDariTerminal(Queue* q);          // Release bus (ubah status jadi 1)
void RiwayatTransaksi();                        // Untuk fitur riwayat tiket (future)
void updateBusStatus(const char *idBus, int statusBaru);

#endif

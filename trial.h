#ifndef trial_H
#define trial_H

#include "../Header/bus.h"
#include "../Header/tiket.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void stringToRute(char* ruteStr, NodeRute** head);
void simulasiPerjalanan(DataBus* bus, DataTiket* tiket, int tiketCount);
time_t convertToTimeT(const char* waktuStr);
void printTime(time_t waktu);

#endif
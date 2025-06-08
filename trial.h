#ifndef trial_H
#define trial_H

#include "../Header/bus.h"
#include "../Header/tiket.h"
#include "../Header/user.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void bacaDataBus(char* filename, DataBus buses[], int* busCount);
void BacaDataTiket(char* filename, DataTiket tiket[], int* busCount);
void simulasiPerjalanan(DataBus* bus, DataTiket tiket[], int tiketCount);

#endif
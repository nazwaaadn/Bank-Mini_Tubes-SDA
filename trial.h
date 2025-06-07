#ifndef trial_H
#define trial_H

#include "../Header/bus.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void bacaDataBus(char* filename, DataBus buses[], int* busCount);
void simulasiPerjalanan(DataBus* bus);

#endif
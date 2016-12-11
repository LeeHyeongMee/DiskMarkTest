#pragma once
#include "BenchMarkData.h"

BenchMarkData* main_thr(int test, char drive, int trials, int selectedChunk);
void checkDiskFreeSpace(BenchMarkData* data);
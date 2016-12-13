#pragma once
#include "BenchMarkData.h"

BenchMarkData* main_thr(int test, char drive, int trials, int selectedChunk);
void checkDiskFreeSpace(BenchMarkData* data);
DWORD FileReadSeq(int size, char dir);
DWORD IterTrial(int size, char dir, int trial);
DWORD FileReadRand(int size, char dir, int trial);
DWORD FileWriteSeq(int size, char dir, int trial);
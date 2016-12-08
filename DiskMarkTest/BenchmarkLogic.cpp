/*
* Benchmark Test for seqRead && randRead
*/

#include "stdafx.h"
#include "BenchMarkData.h"
#include "BenchmarkLogic.h"
#include <stdio.h>
#include <windows.h>
#include <math.h>
#pragma comment(lib, "user32.lib")
#include <atlstr.h>
#include <iostream>
#include <strsafe.h>

using namespace std;

static CString testFileDir;
static CString testFilePath;

// ================================  MODULE READ  ===========================================

long long Sequential_read(BenchMarkData* data)
{
	int j;
	BOOL result;
	DWORD readPtr;
	LARGE_INTEGER StartTime, EndTime, ElapsedSeconds, Freq;
	double bufferSize = data->testSize; //data->pageSize * 1024;
	int blockNum = (int)bufferSize / data->pageSize;
	static char* bufferPtr = (char*)VirtualAlloc(NULL, bufferSize, MEM_COMMIT, PAGE_READWRITE);
	int t = 4;

	// create Test File
	static HANDLE hFile = CreateFile(testFilePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (hFile == INVALID_HANDLE_VALUE) {

		return -10000;
	}

	// kernel-mode access to performance counter
	// Start Performance Counter
	QueryPerformanceFrequency(&Freq);
	QueryPerformanceCounter(&StartTime);

	for (j = 0; j < blockNum; j++)
	{
		result = ReadFile(hFile, bufferPtr, data->pageSize, &readPtr, NULL);

		if (!result) {
			// Handle error
		}

	}

	QueryPerformanceCounter(&EndTime);

	VirtualFree(bufferPtr, bufferSize, MEM_DECOMMIT);
/*	CString str;
	str.Format(_T("starttime: %d ms, endtime: %d ms"), StartTime.QuadPart, EndTime.QuadPart); // float??À» CString??À¸?? ?Ù²??Ö±? À§?? ????. 
	AfxMessageBox(str);*/
	ElapsedSeconds.QuadPart = EndTime.QuadPart - StartTime.QuadPart;
	ElapsedSeconds.QuadPart *= 1000;
	/*str.Format(_T("%d ms"), ElapsedSeconds.QuadPart); // float??À» CString??À¸?? ?Ù²??Ö±? À§?? ????. 
	AfxMessageBox(str);
	ElapsedSeconds.QuadPart /= Freq.QuadPart;
	//	CString str;
	str.Format(_T("time spent is %d ms"), ElapsedSeconds.QuadPart); // float??À» CString??À¸?? ?Ù²??Ö±? À§?? ????. 
	AfxMessageBox(str);*/

	return ElapsedSeconds.QuadPart;
}

long long Random_read(BenchMarkData* data) {
	int j;
	BOOL setPtr_result, result;
	DWORD readPtr;
	LARGE_INTEGER StartTime, EndTime, ElapsedSeconds, Freq;
	LARGE_INTEGER randBlockPtr;
	double bufferSize = data->testSize;//data->pageSize * 1024;
	int blockNum = (int)bufferSize / data->pageSize;
	static char* bufferPtr = (char*)VirtualAlloc(NULL, bufferSize, MEM_COMMIT, PAGE_READWRITE);

	// create Test File
	static HANDLE hFile = CreateFile(testFilePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING | FILE_FLAG_RANDOM_ACCESS, NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		// handle erro
	}

	// Generate rand Num with os tick count to acquire high randomness
	srand(GetTickCount());

	QueryPerformanceFrequency(&Freq);
	QueryPerformanceCounter(&StartTime);

	for (j = 0; j < blockNum; j++) {
		randBlockPtr.QuadPart = (long long)rand() % ((int)(bufferSize / data->pageSize) + 1);
		setPtr_result = SetFilePointerEx(hFile, randBlockPtr, NULL, FILE_BEGIN);
		result = ReadFile(hFile, bufferPtr, data->pageSize, &readPtr, NULL);

		if (!setPtr_result) {
			// handle error for settig rand file ptr
		}

		if (!result) {
			// handle error for read file failure
		}
	}

	QueryPerformanceCounter(&EndTime);

	VirtualFree(bufferPtr, bufferSize, MEM_DECOMMIT);

	ElapsedSeconds.QuadPart = EndTime.QuadPart - StartTime.QuadPart;
	ElapsedSeconds.QuadPart *= 1000;
	ElapsedSeconds.QuadPart /= Freq.QuadPart;

	return ElapsedSeconds.QuadPart;
}

// ================================  MODULE WRITE  ===========================================

long long Sequential_write(BenchMarkData* data) {
	int j;
	BOOL result;
	DWORD writePtr;
	LARGE_INTEGER StartTime, EndTime, ElapsedSeconds;
	LARGE_INTEGER Freq;
	double bufferSize = data->testSize; //data->pageSize * 1024;
	int blockNum = (int)bufferSize / data->pageSize; //(int)data->trials / data->pageSize;
	static char* bufferPtr = (char*)VirtualAlloc(NULL, data->pageSize, MEM_COMMIT, PAGE_READWRITE);

	// create Test File
	static HANDLE hFile = CreateFile(testFilePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		// handle error
		return -1000012;
	}

	// kernel-mode access to performance counter
	// Start Performance Counter
	QueryPerformanceFrequency(&Freq);
	QueryPerformanceCounter(&StartTime);

	for (j = 0; j < blockNum; j++)
	{
		result = WriteFile(hFile, bufferPtr, data->pageSize, &writePtr, NULL);
		FlushFileBuffers(hFile);

		if (!result) {
			// Handle error
		}
	}

	// End Performance Counter
	QueryPerformanceCounter(&EndTime);
	VirtualFree(bufferPtr, bufferSize, MEM_DECOMMIT);

	// Format Performance Counter unit to second ( ns to s )
	ElapsedSeconds.QuadPart = EndTime.QuadPart - StartTime.QuadPart;
	ElapsedSeconds.QuadPart *= 1000;
	ElapsedSeconds.QuadPart /= Freq.QuadPart;

	return ElapsedSeconds.QuadPart;
}

long long Random_write(BenchMarkData* data) {
	int j;
	BOOL setPtr_result, result;
	DWORD writePtr;
	LARGE_INTEGER StartTime, EndTime, ElapsedSeconds;
	LARGE_INTEGER Freq, randBlockPtr;
	double bufferSize = data->pageSize * 1024;
	int blockNum = (int)bufferSize / data->pageSize;
	static char* bufferPtr = (char*)VirtualAlloc(NULL, bufferSize, MEM_COMMIT, PAGE_READWRITE);

	static HANDLE hFile = CreateFile(testFilePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING | FILE_FLAG_RANDOM_ACCESS, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		// handle error
	}

	// Generate rand Num with os tick count to acquire high randomness
	srand(GetTickCount());

	QueryPerformanceFrequency(&Freq);
	QueryPerformanceCounter(&StartTime);

	for (j = 0; j < blockNum; j++)
	{
		randBlockPtr.QuadPart = (long long)rand() % ((int)(bufferSize / data->pageSize) + 1);
		setPtr_result = SetFilePointerEx(hFile, randBlockPtr, NULL, FILE_BEGIN);

		result = WriteFile(hFile, bufferPtr, data->pageSize, &writePtr, NULL);
		FlushFileBuffers(hFile);

		if (!result) {
			// Handle error
		}
	}

	// End Performance Counter
	QueryPerformanceCounter(&EndTime);
	VirtualFree(bufferPtr, bufferSize, MEM_DECOMMIT);

	// Format Performance Counter unit to second ( ns to s )
	ElapsedSeconds.QuadPart = EndTime.QuadPart - StartTime.QuadPart;
	ElapsedSeconds.QuadPart *= 1000;
	ElapsedSeconds.QuadPart /= Freq.QuadPart;

	return ElapsedSeconds.QuadPart;
}

// ================================  CONTROLLER  ===========================================

void init_data(BenchMarkData* data) {
	SYSTEM_INFO sysinfo;

	GetSystemInfo(&sysinfo);
	data->pageSize = sysinfo.dwPageSize;
	data->testSize = 4096;
	data->bandwidth = 0.0;
}

void setTestEnv() {
	testFileDir.Format(_T("C:\\BenchMark_testDir"));
	CreateDirectory(testFileDir, NULL);
	testFilePath.Format(_T("%s\\BenchMark_testFile.tmp"), testFileDir);
	// testFilePath.Format(_T("%s\\SBenchMark%08X.tmp"), testFileDir, timeGetTime());
}

void checkDiskFreeSpace() {
	//GetDiskFreeSpaceEx()
}

BenchMarkData* callSequentialRead() {
	int b, i;
	long long sr = 0;

	BenchMarkData* data = (BenchMarkData*)VirtualAlloc(NULL, sizeof(BenchMarkData*), MEM_COMMIT, PAGE_READWRITE);
	init_data(data);

	data->trials = 5;

	// generate tests for 4K(4,096B) ... 4M(4,194,304B)
	for (b = 0; b < 6; b++) {
		// generate tests {trials} times
		for (i = 0; i < data->trials; i++) {
			sr += Sequential_read(data);
		}

		data->bandwidth += (4096 * pow(4, b)) / sr; // unit: B/ms == MB/s
		data->seqRead[b] = sr / data->trials;
		sr = 0;
	}

	// Get average bandwidth
	data->bandwidth /= 6;

	//return sr / data->trials;
	return data;
}

long long callSequentialWrite() {
	int b, i;
	long long sr = 0;

	BenchMarkData* data = (BenchMarkData*)VirtualAlloc(NULL, sizeof(BenchMarkData*), MEM_COMMIT, PAGE_READWRITE);
	init_data(data);

	data->trials = 5;

	// generate tests for 4K(4,096B) ... 4M(4,194,304B)
	for (b = 0; b < 6; b++) {
		// generate tests {trials} times
		for (i = 0; i < data->trials; i++) {
			sr += Sequential_write(data);
		}

		data->bandwidth += (4096 * pow(4, b)) / sr;
		data->seqWrite[b] = sr / data->trials;
		sr = 0;
	}

	// Get average bandwidth
	data->bandwidth /= 6;
	return sr / data->trials;
}

long long callRandomRead() {
	int b, i;
	long long sr = 0;

	BenchMarkData* data = (BenchMarkData*)VirtualAlloc(NULL, sizeof(BenchMarkData*), MEM_COMMIT, PAGE_READWRITE);
	init_data(data);

	data->trials = 5;

	// generate tests for 4K(4,096B) ... 4M(4,194,304B)
	for (b = 0; b < 6; b++) {
		// generate tests {trials} times
		for (i = 0; i < data->trials; i++) {
			sr += Random_read(data);
		}

		data->bandwidth += (4096 * pow(4, b)) / sr;
		data->seqWrite[b] = sr / data->trials;
		sr = 0;
	}

	// Get average bandwidth
	data->bandwidth /= 6;
	return sr / data->trials;
}

long long callRadomWrite() {
	int b, i;
	long long sr = 0;

	BenchMarkData* data = (BenchMarkData*)VirtualAlloc(NULL, sizeof(BenchMarkData*), MEM_COMMIT, PAGE_READWRITE);
	init_data(data);

	data->trials = 5;

	// generate tests for 4K(4,096B) ... 4M(4,194,304B)
	for (b = 0; b < 6; b++) {
		// generate tests {trials} times
		for (i = 0; i < data->trials; i++) {
			sr += Random_write(data);
		}

		data->bandwidth += (4096 * pow(4, b)) / sr;
		data->seqWrite[b] = sr / data->trials;
		sr = 0;
	}

	// Get average bandwidth
	data->bandwidth /= 6;
	return sr / data->trials;
}

BenchMarkData* main_thr(int d) {
	DWORD thread_id;
	setTestEnv();
	BenchMarkData* tmp = callSequentialRead();
	long long ans;
	
	if (d == 1) {
		tmp = callSequentialRead();
		//ans = tmp->seqRead[5];
	}
	else if (d == 2)
	{
		ans = callSequentialWrite();
	}
	else if (d == 3)
	{
		ans = callRandomRead();
	}
	else if (d == 4)
	{
		ans = callRadomWrite();
	}
	return tmp;
}

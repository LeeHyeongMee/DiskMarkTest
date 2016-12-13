/*
* Benchmark Test for seqRead && randRead
*/

#include "stdafx.h"
#pragma comment(lib, "winmm.lib")
#include <mmsystem.h>
#include "BenchMarkData.h"
#include "BenchmarkLogic.h"
#include <stdio.h>
#include <windows.h>
#include <math.h>
#pragma comment(lib, "user32.lib")
#include <atlstr.h>
#include <iostream>
#include <strsafe.h>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#define VIRTUAL_DISK_SIZE 4294967296
//#define VIRTUAL_DISK_SIZE 1024*1024
using namespace std;

CString cstr;
char diskDrive;
int trialNum;
int chunkSize;
static CString testFileDir;
static CString testFilePath;

/*BenchMarkData* seqData = NULL;
BenchMarkData* randData = NULL;*/
BenchMarkData* ansData = NULL;
static char* bufferPtr = NULL;
long long bufferSize = 0;
int blockNum = 0;
DWORD start, endTime, elapse;

void ErrorExit(LPTSTR lpszFunction) {
	// Retrieve the system error message for the last-error code
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process
	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}

void init(BenchMarkData* data) {
	//!!!!!!!!!!!!need to get chunkSize input !!!!!!!!!!!!!!!
	data->chunkSize = chunkSize;
	//!!!!!!!!!!!!need to get chunkSize input !!!!!!!!!!!!!!!

	data->drive = diskDrive;
	data->seqRead = 0;
	data->randRead = 0;
	data->bandwidth = 0.0;
	data->trials = trialNum;
	data->testSize = VIRTUAL_DISK_SIZE * trialNum;
}

void setTestDir(char drive) {
	testFileDir.Format(_T("%c:\\BenchMark_testDir"), drive);
	CreateDirectory(testFileDir, NULL);

	testFilePath.Format(_T("%s\\BenchMark_test%08X.tmp"), testFileDir, GetTickCount());
	// testFilePath.Format(_T("%s\\SBenchMark.tmp"), testFileDir);
}

void checkDiskFreeSpace(BenchMarkData* data) {
	TCHAR Root[4];
	ULARGE_INTEGER freeSpace;
	ULARGE_INTEGER diskSize;
	ULARGE_INTEGER diskFreeSpace;

	GetDiskFreeSpaceEx(Root, &freeSpace, &diskSize, &diskFreeSpace);

	if (diskFreeSpace.HighPart == 0 && data->testSize > diskFreeSpace.LowPart) {
		cstr.Format(_T("No available space for the test. %ldB needed. Aborting..."), diskFreeSpace.LowPart);
		AfxMessageBox(cstr);
	}
}


// ==============  MODULE READ ============== 

//long long Sequential_read(BenchMarkData* data) {
DWORD Sequential_read(BenchMarkData* data) {
	int j;
	BOOL result, highResolutionSupport;
	DWORD readPtr;
	//LARGE_INTEGER StartTime, EndTime, ElapsedSeconds, Freq;
	//DWORD start, end, elapse;

	cstr.Format(_T("Sequential Read start"));
	AfxMessageBox(cstr);

	bufferSize = VIRTUAL_DISK_SIZE;
	blockNum = (int)bufferSize / data->chunkSize;
	bufferPtr = (char*)VirtualAlloc(NULL, bufferSize, MEM_COMMIT, PAGE_READWRITE);
	// set test file directory
	setTestDir(data->drive);

	// create Test File in testFilePath
	static HANDLE hFile = CreateFile(testFilePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		//handle error
		ErrorExit(TEXT("Sequential Read: Create File failure"));
	}

	// Start Performance Counter
	//highResolutionSupport = QueryPerformanceFrequency(&Freq);
	//QueryPerformanceCounter(&StartTime);
	start = timeGetTime();

	for (j = 0; j < blockNum; j++)
	{
		result = ReadFile(hFile, bufferPtr, bufferSize, &readPtr, NULL);

		if (!result) {
			ErrorExit(TEXT("Sequential Read: Read File failure"));
		}
	}
	endTime = timeGetTime();

	elapse = endTime - start;

	//QueryPerformanceCounter(&EndTime);

	//ElapsedSeconds.QuadPart = EndTime.QuadPart - StartTime.QuadPart;
	//ElapsedSeconds.QuadPart /= Freq.QuadPart;
	//// convert second to millisecond
	//ElapsedSeconds.QuadPart *= 1000;

	CloseHandle(hFile);
	// delete test file created
	result = DeleteFile(testFilePath);
	if (!result) {
		// handle error
		ErrorExit(TEXT("Sequential Read: Delete File failure"));
	}

	//return ElapsedSeconds.QuadPart;
	return elapse;
}

//long long Random_read(BenchMarkData* data) {
DWORD Random_read(BenchMarkData* data) {
	int j;
	BOOL setPtr_result, result;
	DWORD readPtr;
	DWORD start, end, elapse;
	LARGE_INTEGER StartTime, EndTime, ElapsedSeconds, Freq;
	LARGE_INTEGER randBlockPtr;

	cstr.Format(_T("Random Read start"));
	AfxMessageBox(cstr);

	bufferSize = VIRTUAL_DISK_SIZE;
	blockNum = (int)bufferSize / data->chunkSize; //division by zero error occurs
	bufferPtr = (char*)VirtualAlloc(NULL, bufferSize, MEM_COMMIT, PAGE_READWRITE);
	// set test file directory
	setTestDir(data->drive);

	// create Test File
	static HANDLE hFile = CreateFile(testFilePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING | FILE_FLAG_RANDOM_ACCESS, NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		ErrorExit(TEXT("Random Read: Create File failure"));
	}

	// Generate rand Num with os tick count to acquire high randomness
	srand(GetTickCount());

	//QueryPerformanceFrequency(&Freq);
	//QueryPerformanceCounter(&StartTime);
	start = timeGetTime();

	for (j = 0; j < blockNum; j++) {
		cstr.Format(_T("iterating blockNum %d"), j);
		AfxMessageBox(cstr);

		// evaluate random pointer address
		randBlockPtr.QuadPart = (long long)(rand() % blockNum) * data->chunkSize;

		// set random pointer to handler file
		setPtr_result = SetFilePointerEx(hFile, randBlockPtr, NULL, FILE_BEGIN);

		result = ReadFile(hFile, bufferPtr, bufferSize, &readPtr, NULL);

		if (!setPtr_result) {
			ErrorExit(TEXT("Random Read: Random File pointer failure"));
		}

		if (!result) {
			ErrorExit(TEXT("Random Read: Read File failure"));
		}
	}

	//QueryPerformanceCounter(&EndTime);

	//ElapsedSeconds.QuadPart = EndTime.QuadPart - StartTime.QuadPart;
	//ElapsedSeconds.QuadPart /= Freq.QuadPart;

	// convert second to millisecond
	//ElapsedSeconds.QuadPart *= 1000; 
	end = timeGetTime();
	elapse = end - start;

	CloseHandle(hFile);
	// delete test file
	result = DeleteFile(testFilePath);
	if (!result) {
		//handle error
		ErrorExit(TEXT("Random Read: Delete File failure"));
	}
	cstr.Format(_T("Random Read end"));
	AfxMessageBox(cstr);
	//return ElapsedSeconds.QuadPart;
	return elapse;
}

// ==============  MODULE WRITE ============== 
long long Sequential_write(BenchMarkData* data) {
	int j;
	BOOL result;
	DWORD writePtr;
	LARGE_INTEGER StartTime, EndTime, ElapsedSeconds;
	LARGE_INTEGER Freq;

	bufferSize = VIRTUAL_DISK_SIZE;
	blockNum = (int)bufferSize / data->chunkSize;
	bufferPtr = (char*)VirtualAlloc(NULL, bufferSize, MEM_COMMIT, PAGE_READWRITE);

	// create Test File
	static HANDLE hFile = CreateFile(testFilePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		// handle error
		ErrorExit(TEXT("Sequential Write: Create File failure"));
	}

	// Start Performance Counter
	QueryPerformanceFrequency(&Freq);
	QueryPerformanceCounter(&StartTime);

	for (j = 0; j < blockNum; j++)
	{
		result = WriteFile(hFile, bufferPtr, bufferSize, &writePtr, NULL);
		// flush buffer to disk
		FlushFileBuffers(hFile);

		if (!result) {
			ErrorExit(TEXT("Sequential Write: Write File failure"));
		}
	}

	// End Performance Counter
	QueryPerformanceCounter(&EndTime);

	ElapsedSeconds.QuadPart = EndTime.QuadPart - StartTime.QuadPart;
	ElapsedSeconds.QuadPart /= Freq.QuadPart;

	// convert second to millisecond
	ElapsedSeconds.QuadPart *= 1000;

	CloseHandle(hFile);
	// delete test file
	result = DeleteFile(testFilePath);
	if (!result) {
		//handle error
		ErrorExit(TEXT("Random Read: Delete File failure"));
	}

	return ElapsedSeconds.QuadPart;
}

long long Random_write(BenchMarkData* data) {
	int j;
	BOOL setPtr_result, result;
	DWORD writePtr;
	LARGE_INTEGER StartTime, EndTime, ElapsedSeconds, Freq;
	LARGE_INTEGER randBlockPtr;

	bufferSize = VIRTUAL_DISK_SIZE;
	blockNum = (int)bufferSize / data->chunkSize;
	bufferPtr = (char*)VirtualAlloc(NULL, bufferSize, MEM_COMMIT, PAGE_READWRITE);

	// create Test File
	static HANDLE hFile = CreateFile(testFilePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING | FILE_FLAG_RANDOM_ACCESS, NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		// handle error
		ErrorExit(TEXT("Random Write: Create File failure"));
	}

	// Generate rand Num with os tick count to acquire high randomness
	srand(GetTickCount());

	QueryPerformanceFrequency(&Freq);
	QueryPerformanceCounter(&StartTime);

	for (j = 0; j < blockNum; j++)
	{
		// evaluate random pointer address
		randBlockPtr.QuadPart = (long long)(rand() % blockNum) * data->chunkSize;

		// set random pointer to handler file
		setPtr_result = SetFilePointerEx(hFile, randBlockPtr, NULL, FILE_BEGIN);

		result = WriteFile(hFile, bufferPtr, bufferSize, &writePtr, NULL);
		// flush buffer to disk
		FlushFileBuffers(hFile);

		if (!setPtr_result) {
			//handle error for file pointer
			ErrorExit(TEXT("Random Write: Random File pointer failure"));
		}

		if (!result) {
			// Handle error
			ErrorExit(TEXT("Random Write: Write File failure"));
		}
	}

	// End Performance Counter
	QueryPerformanceCounter(&EndTime);
	ElapsedSeconds.QuadPart = EndTime.QuadPart - StartTime.QuadPart;
	ElapsedSeconds.QuadPart /= Freq.QuadPart;

	// convert second to millisecond
	ElapsedSeconds.QuadPart *= 1000;

	CloseHandle(hFile);
	// delete test file
	result = DeleteFile(testFilePath);
	if (!result) {
		//handle error
		ErrorExit(TEXT("Random Read: Delete File failure"));
	}

	return ElapsedSeconds.QuadPart;
}


// ==============  CONTROLLER ============== 

BenchMarkData* callSequentialRead() {
	int i;
	//long long sr = 0;
	DWORD sr = 0;

	BenchMarkData* data = (BenchMarkData*)VirtualAlloc(NULL, sizeof(BenchMarkData*), MEM_COMMIT, PAGE_READWRITE);
	init(data);
	checkDiskFreeSpace(data);

	Sequential_read(data);
	sr += elapse;

	cstr.Format(_T("start is %ld, end %ld, elapse %ld"), start, endTime, elapse);
	AfxMessageBox(cstr);

	// eval average
	data->bandwidth = VIRTUAL_DISK_SIZE / sr;
	data->seqRead = sr / data->trials;

	return data;
}

BenchMarkData* callRandomRead() {
	int i;
	//long long sr = 0;
	DWORD sr = 0;

	BenchMarkData* data = (BenchMarkData*)VirtualAlloc(NULL, sizeof(BenchMarkData*), MEM_COMMIT, PAGE_READWRITE);
	init(data);
	checkDiskFreeSpace(data);

	for (i = 0; i < data->trials; i++) {
		sr += Random_read(data);
		cstr.Format(_T("sr is %ld"), sr);
		AfxMessageBox(cstr);
	}

	// eval average
	data->randRead = sr / data->trials;
	data->bandwidth = VIRTUAL_DISK_SIZE / sr;

	return data;
}

BenchMarkData* callSequentialWrite() {
	int i;
	long long sr = 0;

	BenchMarkData* data = (BenchMarkData*)VirtualAlloc(NULL, sizeof(BenchMarkData*), MEM_COMMIT, PAGE_READWRITE);
	init(data);
	checkDiskFreeSpace(data);

	for (i = 0; i < data->trials; i++) {
		sr += Sequential_write(data);
	}

	// eval average
	data->seqWrite = sr / data->trials;
	data->bandwidth = VIRTUAL_DISK_SIZE / sr;

	return data;
}

BenchMarkData* callRandomWrite() {
	int i;
	long long sr = 0;

	BenchMarkData* data = (BenchMarkData*)VirtualAlloc(NULL, sizeof(BenchMarkData*), MEM_COMMIT, PAGE_READWRITE);
	init(data);
	checkDiskFreeSpace(data);

	for (i = 0; i < data->trials; i++) {
		sr += Random_write(data);
	}

	//eval average
	data->randWrite = sr / data->trials;
	data->bandwidth = VIRTUAL_DISK_SIZE / sr;

	return data;
}


BenchMarkData* main_thr(int test, char drive, int trials, int selectedChunk) {
	diskDrive = drive;
	trialNum = trials;
	chunkSize = selectedChunk;

	// implement test
	if (test == 1) {
		ansData = callSequentialRead();
	}
	else if (test == 2) {
		ansData = callRandomRead();
	}
	else if (test == 2)
	{
		ansData = callSequentialWrite();
	}
	else if (test == 4) {
		ansData = callRandomWrite();
	}

	// 형미 언니, gui에서 seq, rand, bufferPtr 접근해서 data 처리하고 free 시켜주면 될 듯
	// VirtualFree(seq, sizeof(BenchMarkData*), MEM_DECOMMIT);
	// VirtualFree(rand, sizeof(BenchMarkData*), MEM_DECOMMIT);
	// VirtualFree(bufferPtr, bufferSize, MEM_DECOMMIT);

	//trialNum = 0;
	//diskDrive = NULL;
	//chunkSize = 0;
	return ansData;
}

DWORD FileReadSeq(int size, char dir)
{
	setTestDir(dir);
	BOOL result, highResolutionSupport;
	DWORD readPtr;
	int blockNum;
	//bufferSize = VIRTUAL_DISK_SIZE;
	if (size == 512)
	{
		bufferPtr = (char*)VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
		blockNum = pow(2, 21);
		// create Test File in testFilePath
	}
	else if (size == 1024)
	{
		bufferPtr = (char*)VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
		 blockNum = pow(2, 20);
		
	}
	else if (size == (int)pow(2, 11))
	{
		bufferPtr = (char*)VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
		blockNum = pow(2, 19);
	}
	else if (size == (int)pow(2, 12))
	{
		bufferPtr = (char*)VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
		 blockNum = pow(2, 18);
	}
	else if (size == (int)pow(2, 13))
	{
		bufferPtr = (char*)VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
		 blockNum = pow(2, 17);
	}
	else if (size == (int)pow(2, 14))
	{
		bufferPtr = (char*)VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
		 blockNum = pow(2, 16);
	}
		
	
	static HANDLE hFile = CreateFile(testFilePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	start = timeGetTime();
	CString a;
	a.Format(_T("testFilePath is %s"), testFilePath);
	a.Format(_T("%d"), start);
	AfxMessageBox(a);

	for (int j = 0; j < blockNum; j++)
	{
		result = ReadFile(hFile, bufferPtr, size, &readPtr, NULL); //512Byte

		if (result == FALSE) {
			ErrorExit(TEXT("Sequential Read: Read File failure"));
		}
	}

	endTime = timeGetTime();
	a.Format(_T("end time is %d"), endTime);
	AfxMessageBox(a);

	elapse = endTime - start;

	CloseHandle(hFile);
		// delete test file created
	result = DeleteFile(testFilePath);
	if (!result) 
	{
			// handle error
		ErrorExit(TEXT("Sequential Read: Delete File failure"));
	}
	
	VirtualFree(bufferPtr, size, MEM_DECOMMIT);
	//return ElapsedSeconds.QuadPart;
	return elapse;

}

DWORD IterTrial(int size, char dir, int trial)
{
	DWORD sum = 0;

	for (int i = 0; i < trial; i++)
	{
		sum = sum + FileReadSeq(size, dir);
		CString s;
		s.Format(_T("iteration %d th time and sum is %d"), i + 1, sum);
		AfxMessageBox(s);
	}

	return sum;
}

DWORD FileReadRand(int size, char dir, int trial)
{
	int j;
	BOOL setPtr_result, result;
	DWORD readPtr;
	DWORD start, end, elapse;
	LARGE_INTEGER StartTime, EndTime, ElapsedSeconds, Freq;
	LARGE_INTEGER randBlockPtr;

//	cstr.Format(_T("Random Read start"));
//	AfxMessageBox(cstr);
	setTestDir(dir);

	bufferSize = VIRTUAL_DISK_SIZE;
//	blockNum = (int)bufferSize / data->chunkSize; //division by zero error occurs
	bufferPtr = (char*)VirtualAlloc(NULL, bufferSize, MEM_COMMIT, PAGE_READWRITE);
	// set test file directory
//	setTestDir(data->drive);

	// create Test File
	static HANDLE hFile = CreateFile(testFilePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING | FILE_FLAG_RANDOM_ACCESS, NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		ErrorExit(TEXT("Random Read: Create File failure"));
	}

	// Generate rand Num with os tick count to acquire high randomness
	srand(GetTickCount());


	start = timeGetTime();
	int blockNum = pow(2, 21);

	for (j = 0; j < blockNum; j++) {
		//cstr.Format(_T("iterating blockNum %d"), j);
		//AfxMessageBox(cstr);

		// evaluate random pointer address
		randBlockPtr.QuadPart = (long long)(rand() % blockNum) * 512; //512Byte

		// set random pointer to handler file
		setPtr_result = SetFilePointerEx(hFile, randBlockPtr, NULL, FILE_BEGIN);

		result = ReadFile(hFile, bufferPtr, 512, &readPtr, NULL);

		if (!setPtr_result) {
			ErrorExit(TEXT("Random Read: Random File pointer failure"));
		}

		if (!result) {
			ErrorExit(TEXT("Random Read: Read File failure"));
		}
	}

	//QueryPerformanceCounter(&EndTime);

	//ElapsedSeconds.QuadPart = EndTime.QuadPart - StartTime.QuadPart;
	//ElapsedSeconds.QuadPart /= Freq.QuadPart;

	// convert second to millisecond
	//ElapsedSeconds.QuadPart *= 1000; 
	end = timeGetTime();
	elapse = end - start;

	CloseHandle(hFile);
	// delete test file
	result = DeleteFile(testFilePath);
	if (!result) {
		//handle error
		ErrorExit(TEXT("Random Read: Delete File failure"));
	}
	cstr.Format(_T("Random Read end"));
	AfxMessageBox(cstr);
	//return ElapsedSeconds.QuadPart;
	return elapse;
}

DWORD FileWriteSeq(int size, char dir, int trial)
{
	int j;
	BOOL result;
	DWORD writePtr;
	LARGE_INTEGER StartTime, EndTime, ElapsedSeconds;
	LARGE_INTEGER Freq;

	bufferSize = VIRTUAL_DISK_SIZE;
	blockNum = pow(2, 21);
	bufferPtr = (char*)VirtualAlloc(NULL, bufferSize, MEM_COMMIT, PAGE_READWRITE);
	setTestDir(dir);
	// create Test File
	static HANDLE hFile = CreateFile(testFilePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		// handle error
		ErrorExit(TEXT("Sequential Write: Create File failure"));
	}

	// Start Performance Counter
	//QueryPerformanceFrequency(&Freq);
	//QueryPerformanceCounter(&StartTime);
	start = timeGetTime();

	for (j = 0; j < blockNum; j++)
	{
		result = WriteFile(hFile, bufferPtr, 512, &writePtr, NULL);
		// flush buffer to disk
		FlushFileBuffers(hFile);

		if (!result) {
			ErrorExit(TEXT("Sequential Write: Write File failure"));
		}
	}

	// End Performance Counter
	//QueryPerformanceCounter(&EndTime);

	//ElapsedSeconds.QuadPart = EndTime.QuadPart - StartTime.QuadPart;
	//ElapsedSeconds.QuadPart /= Freq.QuadPart;

	// convert second to millisecond
	//ElapsedSeconds.QuadPart *= 1000;
	endTime = timeGetTime();

	CloseHandle(hFile);
	// delete test file
	result = DeleteFile(testFilePath);
	if (!result) {
		//handle error
		ErrorExit(TEXT("Random Read: Delete File failure"));
	}

	return (endTime - start);
}
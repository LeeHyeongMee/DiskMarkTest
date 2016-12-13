#pragma once


class BenchMarkData
{
public:
	char drive;
	int trials;
	int chunkSize;
	long long testSize;

	double bandwidth;

	long long seqRead;
	long long randRead;

	double seqWrite;
	double randWrite;
};
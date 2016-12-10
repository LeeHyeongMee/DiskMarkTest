#pragma once


class BenchMarkData 
{
public:
  char drive;
	int trials;
	int chunkSize;
	long long testSize;

	double bandwidth;

	double seqRead;
	double randRead;

	double seqWrite;
	double randWrite;
};

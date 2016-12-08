#pragma once
#pragma once


class BenchMarkData //: public CWnd
{
public:
	//BenchMarkData(CWnd* pParent = NULL);

	int testSize;
	int trials;
	double pageSize;

	double bandwidth;
	double seqRead[6];
	double randRead[6];
	double seqWrite[6];
	double randWrite[6];

	//double seqRead_4K;
	//double seqRead_16K;
	//double seqRead_64K;
	//double seqRead_256K;
	//double seqRead_1M;
	//double seqRead_4M;

	//double randRead_4K;
	//double randRead_16K;
	//double randRead_64K;
	//double randRead_256K;
	//double randRead_1M;
	//double randRead_4M;

	//double seqWrite_4K;
	//double seqWrite_16K;
	//double seqWrite_64K;
	//double seqWrite_256K;
	//double seqWrite_1M;
	//double seqWrite_4M;

	//double randWrite_4K;
	//double randWrite_16K;
	//double randWrite_64K;
	//double randWrite_256K;
	//double randWrite_1M;
	//double randWrite_4M;

	double migration_128G;
	double migration_256G;
	double migration_512G;
};

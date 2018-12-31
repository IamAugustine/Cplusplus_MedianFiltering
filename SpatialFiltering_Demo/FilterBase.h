#pragma once
#ifndef _FilterBase_H_
#define _FilterBase_H_
#endif // !_FilterBase_H_
#include "Kernel.h"
#include <deque>
using namespace std;
typedef unsigned short ushort;

class FilterBase
{
public:
	FilterBase();
	~FilterBase();
protected:
	int ImageHeight ;
	int ImageWidth ;
	FilterKernel* Kernel;
	int blockHeight;
public:
	virtual void Apply(ushort* imageIn, int height, int width, ushort* imageOut) = 0;
	virtual void FilterBlock(ushort* imgIn, ushort* imgOut) = 0;
public:
	int GetCPUCoreNumber();
	ushort** SegmentImage(ushort* imgIn, int blockNumber);
	void ReconstructImage(ushort** segments, ushort* imgOut, int blockNumber, const int blockHeight);
};


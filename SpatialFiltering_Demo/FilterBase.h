#pragma once
#ifndef _FilterBase_H_
#define _FilterBase_H_
#endif // !_FilterBase_H_
#include "Kernel.h"
#include <deque>
#include <thread>
#include <algorithm>

using namespace std;
typedef unsigned short ushort;

class Filter2D
{
public:
	Filter2D();
	~Filter2D();
protected:
	int ImageHeight ;
	int ImageWidth ;
	FilterKernel* Kernel;
	int blockHeight;
public:
	void Apply(const ushort* imageIn, int height, int width, ushort* imageOut);
	void FilterBlock(const ushort* imgIn, ushort* imgOut) ;
	void KernelMoveRight(const ushort * imgIn, int rowIndex, int clmIndexToAdd, deque<ushort>& tmp);
	deque<ushort> InitializeDeque(const ushort * imgIn, const int y);
	ushort Calculate(deque<ushort> data);
public:
	int GetCPUCoreNumber();
	ushort** SegmentImage(const ushort* imgIn, int blockNumber);
	void ReconstructImage(ushort** segments, ushort* imgOut, int blockNumber, const int blockHeight);
};


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
	
public:
	void Apply(  ushort* imageIn, int height, int width, ushort* imageOut);
	virtual void FilterBlock(  ushort* imgIn, ushort* imgOut) ;
	virtual void KernelMoveRight(  ushort * imgIn, int rowIndex, int clmIndexToAdd, deque<ushort>& tmp);
	virtual deque<ushort> InitializeDeque(  ushort * imgIn,   int y);
	virtual ushort Calculate(deque<ushort> data);
	virtual void ProcessingBlocks(ushort** blocksIn,byte blockHeight, byte threadCount, ushort** blocksOut);
	void(Filter2D::*fun)(  ushort*, ushort*) = &Filter2D::FilterBlock;
	int blockHeight;
	int bndBufferHeight;
public:
	int GetCPUCoreNumber();
	ushort** SegmentImage(  ushort* imgIn, int blockNumber);
	void ReconstructImage(ushort** segments, ushort* imgOut, int blockNumber,   int blockHeight);
};
enum Direction
{
	Vertical,
	Horizontal,
	Both
};


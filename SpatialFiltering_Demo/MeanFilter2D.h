#pragma once
#include "FilterBase.h"
class MeanFilter2D :
	public Filter2D
{
public:
	MeanFilter2D();
	MeanFilter2D(byte size);
	MeanFilter2D(byte sizeX, byte sizeY);
	~MeanFilter2D();
	//void Apply(const ushort* imageIn, int height, int width, ushort* imageOut);
public:
	void FilterBlock(const ushort* imageIn, ushort* imageOut);
	void KernelMoveRight(const ushort * imgIn, int rowIndex, int clmIndexToAdd, deque<float>& tmp);
	deque<float> InitializeDeque2(const ushort * imgIn, const int y);
	void ProcessingBlocks(ushort** blocksIn, byte blockHeight, byte threadCount, ushort** blocksOut);
	void(MeanFilter2D::*fun)(const ushort*, ushort*) = &MeanFilter2D::FilterBlock;
private:
	int blockHeight;

};


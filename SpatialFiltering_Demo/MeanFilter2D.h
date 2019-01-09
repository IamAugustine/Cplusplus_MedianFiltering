#pragma once
#include "FilterBase.h"
class MeanFilter2D :
	public Filter2D
{
public:
	//MeanFilter2D();
	MeanFilter2D(byte size) :MeanFilter2D(size, size) {};
	MeanFilter2D(byte sizeX, byte sizeY);
	~MeanFilter2D();
	//void Apply(  ushort* imageIn, int height, int width, ushort* imageOut);
public:
	void FilterBlock(  ushort* imageIn, ushort* imageOut);
	void KernelMoveRight(  ushort * imgIn, int rowIndex, int clmIndexToAdd, deque<float>& tmp);
	deque<float> InitializeDeque2(  ushort * imgIn,   int y);
	void ProcessingBlocks(ushort** blocksIn, byte blockHeight, byte threadCount, ushort** blocksOut);
	void(MeanFilter2D::*fun)(  ushort*, ushort*) = &MeanFilter2D::FilterBlock;


};


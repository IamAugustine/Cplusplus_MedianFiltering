#pragma once
#include "FilterBase.h"
class MedianFilter2D :
	public Filter2D
{
public:
	MedianFilter2D();
	MedianFilter2D(int size);
	MedianFilter2D(int sizeX, int sizeY);
	~MedianFilter2D();
public:
	void FilterBlock(const ushort* imgIn, ushort* imgOut);
	void ProcessingBlocks(ushort** blocksIn, byte blockHeight, byte threadCount, ushort** blocksOut);
	void (MedianFilter2D::*fun)(const ushort*, ushort*) = &MedianFilter2D::FilterBlock;
private:
	int blockHeight;

	
};


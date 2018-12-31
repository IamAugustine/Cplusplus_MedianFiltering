#pragma once
#include "FilterBase.h"
class MedianFilter2D :
	public FilterBase
{
public:
	MedianFilter2D();
	MedianFilter2D(int size);
	MedianFilter2D(int sizeX, int sizeY);
	~MedianFilter2D();
	void Apply(ushort* imageIn, int height, int width, ushort* imageOut);
	void FilterBlock(ushort* imgIn, ushort* imgOut);
private:
	int blockHeight;
	void KernelMoveRight(ushort * imgIn, int rowIndex, int clmIndexToAdd, deque<ushort>& tmp);
	deque<ushort> InitializeDeque(ushort * imgIn, const int y);
	
};


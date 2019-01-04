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
	void Apply(const ushort* imageIn, int height, int width, ushort* imageOut);
	void FilterBlock(const ushort* imgIn, ushort* imgOut);
private:
	int blockHeight;
	void KernelMoveRight(const ushort * imgIn, int rowIndex, int clmIndexToAdd, deque<ushort>& tmp);
	deque<ushort> InitializeDeque(const ushort * imgIn, const int y);
	
};


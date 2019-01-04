#pragma once
#include "FilterBase.h"
class MeanFilter2D :
	public FilterBase
{
public:
	MeanFilter2D();
	MeanFilter2D(byte size);
	MeanFilter2D(byte sizeX, byte sizeY);
	~MeanFilter2D();
	void Apply(const ushort* imageIn, int height, int width, ushort* imageOut);
	void FilterBlock(const ushort* imageIn, ushort* imageOut);
private:
	int blockHeight;
	void KernelMoveRight(const ushort * imgIn, int rowIndex, int clmIndexToAdd, deque<float>& tmp);
	deque<float> InitializeDeque(const ushort * imgIn, const int y);
};


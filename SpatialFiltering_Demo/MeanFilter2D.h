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
	void Apply(const ushort* imageIn, int height, int width, ushort* imageOut);
	void FilterBlock(const ushort* imageIn, ushort* imageOut);
	void KernelMoveRight(const ushort * imgIn, int rowIndex, int clmIndexToAdd, deque<float>& tmp);
	deque<float> InitializeDeque(const ushort * imgIn, const int y);
private:
	int blockHeight;

};


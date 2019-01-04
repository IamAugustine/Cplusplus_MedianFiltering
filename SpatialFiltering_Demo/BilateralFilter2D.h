#pragma once
#include "FilterBase.h"
class BilateralFilter2D :
	public FilterBase
{
public:
	BilateralFilter2D();
	~BilateralFilter2D();
protected:
	float sigma;
	float mu;
public:
	void Apply(const ushort* imageIn, int height, int width, ushort* imageOut);
	void FilterBlock(const ushort* imgIn, ushort* imgOut);
private:
	int blockHeight;
	void KernelMoveRight(ushort * imgIn, int rowIndex, int clmIndexToAdd, deque<ushort>& tmp);
	deque<ushort> InitializeDeque(ushort * imgIn, const int y);
};


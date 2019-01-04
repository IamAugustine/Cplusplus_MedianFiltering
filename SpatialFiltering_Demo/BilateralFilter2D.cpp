#include "pch.h"
#include "BilateralFilter2D.h"


BilateralFilter2D::BilateralFilter2D()
{
}


BilateralFilter2D::~BilateralFilter2D()
{
}

void BilateralFilter2D::Apply(const ushort * imageIn, int height, int width, ushort * imageOut)
{
}

void BilateralFilter2D::FilterBlock(const ushort * imgIn, ushort * imgOut)
{
}

void BilateralFilter2D::KernelMoveRight(ushort * imgIn, int rowIndex, int clmIndexToAdd, deque<ushort>& tmp)
{
}

deque<ushort> BilateralFilter2D::InitializeDeque(ushort * imgIn, const int y)
{
	return deque<ushort>();
}

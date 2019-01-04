#pragma once
#include "FilterBase.h"
class MeanFilterByTwo1DKernels :
	public FilterBase
{
public:
	MeanFilterByTwo1DKernels(byte kernelSize);
	MeanFilterByTwo1DKernels(byte sizeX, byte sizeY);
	~MeanFilterByTwo1DKernels();
	void Apply(const ushort* imageIn, int height, int width, ushort* imageOut);
	void FilterBlock(const ushort* imageIn, ushort* imageOut);
private:
	int blockHeight;
	void MeanFilter1D_H(const ushort* imageIn, ushort* imageOut);
	void MeanFilter1D_T(const ushort* imageIn, ushort* imageOut);
	ushort GetFirstPixelPerRow(const ushort * imgIn, const int y);
	ushort GetFirstPixelPerColumn(const ushort * imgIn, const int columnIndex);
	ushort ShiftPixel(ushort lastPixe, ushort pixelToAdd, ushort pixelToRemove, byte r);
};

#pragma once
#include "FilterBase.h"
class MeanFilterByTwo1DKernels :
	public Filter2D
{
public:
	MeanFilterByTwo1DKernels(byte kernelSize);
	MeanFilterByTwo1DKernels(byte sizeX, byte sizeY);
	~MeanFilterByTwo1DKernels();
public:
	//void Apply(const ushort* imageIn, int height, int width, ushort* imageOut);
	void FilterBlock(const ushort* imageIn, ushort* imageOut);
	void ProcessingBlocks(ushort** blocksIn, byte blockHeight, byte threadCount, ushort** blocksOut);
	void (MeanFilterByTwo1DKernels::*fun)(const ushort*, ushort*);
private:
	int blockHeight;
	void MeanFilter1D_H(const ushort* imageIn, ushort* imageOut);
	void MeanFilter1D_T(const ushort* imageIn, ushort* imageOut);
	ushort GetFirstPixelPerRow(const ushort * imgIn, const int y);
	ushort GetFirstPixelPerColumn(const ushort * imgIn, const int columnIndex);
	ushort ShiftPixel(ushort lastPixe, ushort pixelToAdd, ushort pixelToRemove, byte r);
};

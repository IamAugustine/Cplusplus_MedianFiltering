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
	//void Apply(  ushort* imageIn, int height, int width, ushort* imageOut);
	void FilterBlock(  ushort* imageIn, ushort* imageOut);
	void ProcessingBlocks(ushort** blocksIn, byte blockHeight, byte threadCount, ushort** blocksOut);
	void (MeanFilterByTwo1DKernels::*fun)(  ushort*, ushort*);
private:
	int blockHeight;
	void MeanFilter1D_H(  ushort* imageIn, ushort* imageOut);
	void MeanFilter1D_T(  ushort* imageIn, ushort* imageOut);
	ushort GetFirstPixelPerRow(  ushort * imgIn,   int y);
	ushort GetFirstPixelPerColumn(  ushort * imgIn,   int columnIndex);
	ushort ShiftPixel(ushort lastPixe, ushort pixelToAdd, ushort pixelToRemove, byte r);
};

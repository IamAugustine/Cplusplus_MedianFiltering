#pragma once
#include "FilterBase.h"
#include "GaussianFilter2D.h"
class BilateralFilter2D :
	public GaussianFilter2D
{
public:
	BilateralFilter2D(float sigmaS, float sigmaP);
	BilateralFilter2D(float sigmaS, byte N, float sigmaP);
	~BilateralFilter2D();
public:
	ushort Calculate(deque<ushort> data);
	void FilterBlock(  ushort* imageIn, ushort* imageOut);
	void ProcessingBlocks(ushort** blocksIn, byte blockHeight, byte threadCount, ushort** blocksOut);
	void (BilateralFilter2D::*fun)(  ushort*, ushort*) = &BilateralFilter2D::FilterBlock;
private:
	float* pixelWeight;
	void CalculatePixelGaussian();
	int blockHeight;
	float sigmaS;
	float sigmaP;
};


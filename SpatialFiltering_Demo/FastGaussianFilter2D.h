#pragma once
#include "FilterBase.h"

// Based on http://blog.ivank.net/fastest-gaussian-blur.html
// Mathsmaticl derivation http://web.csse.uwa.edu.au/research/?a=826172

class FastGaussianFilter2D :
	public Filter2D
{
public:
	FastGaussianFilter2D();
	FastGaussianFilter2D(float sigma);
	FastGaussianFilter2D(float sigma, int round);
	~FastGaussianFilter2D();
public:
	void FilterBlock(  ushort* imageIn, ushort* imageOut);
	void ProcessingBlocks(ushort** blocksIn, byte blockHeight, byte threadCount, ushort** blocksOut);
	void (FastGaussianFilter2D::*fun)(  ushort*, ushort*) = &FastGaussianFilter2D::FilterBlock;
private:
	void EstimateBoxBlurParameters(float sigma, byte n, byte* kernelSize);
	byte* kernelSizes;
	byte roundOfBoxBlur;
};


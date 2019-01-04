#pragma once
#include "FilterBase.h"

// Based on http://blog.ivank.net/fastest-gaussian-blur.html
// Mathsmaticl derivation http://web.csse.uwa.edu.au/research/?a=826172

class FastGaussianFilter2D :
	public FilterBase
{
public:
	FastGaussianFilter2D();
	FastGaussianFilter2D(float sigma);
	FastGaussianFilter2D(float sigma, int round);
	~FastGaussianFilter2D();
	void Apply(const ushort* imageIn, int height, int width, ushort* imageOut);
	void FilterBlock(const ushort* imageIn, ushort* imageOut);
private:
	void EstimateBoxBlurParameters(float sigma, byte n, byte* kernelSize);
	byte* kernelSizes;
	byte roundOfBoxBlur;
};


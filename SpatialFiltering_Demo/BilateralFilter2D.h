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
	void Apply(const ushort* imageIn, int height, int width, ushort* imageOut);
	ushort Calculate(deque<ushort> data);
	void FilterBlock(const ushort* imageIn, ushort* imageOut);
	void CalculatePixelGaussian();
private:
	float* pixelWeight;
	int blockHeight;
	float sigmaS;
	float sigmaP;
};


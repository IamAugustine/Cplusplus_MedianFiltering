#include "pch.h"
#include "FastGaussianFilter2D.h"
#include "MeanFilterByTwo1DKernels.h"
#include <algorithm>
#include <iterator>
FastGaussianFilter2D::FastGaussianFilter2D()
{
}

FastGaussianFilter2D::FastGaussianFilter2D(float sigma, int n)
{
	kernelSizes = new byte[n];
	roundOfBoxBlur = n;
	EstimateBoxBlurParameters(sigma, n, kernelSizes);
	
}


FastGaussianFilter2D::~FastGaussianFilter2D()
{
}


void FastGaussianFilter2D::FilterBlock(  ushort * imageIn, ushort * imageOut)
{
	MeanFilterByTwo1DKernels* filter;
	ushort* temp = new ushort[blockHeight*ImageWidth];
	memcpy(temp, imageIn, blockHeight*ImageWidth);
	for (size_t i = 0; i < roundOfBoxBlur; i++)
	{
		filter = new MeanFilterByTwo1DKernels(kernelSizes[i]);
		filter->Apply(temp, blockHeight, ImageWidth, imageOut);
		memcpy(imageOut, temp, blockHeight*ImageWidth);
	}
	delete temp;
}

void FastGaussianFilter2D::ProcessingBlocks(ushort ** blocksIn, byte blockHeight, byte threadCount, ushort ** blocksOut)
{
	for (size_t i = 0; i < threadCount; i++)
	{
		blocksIn[i] = new ushort[blockHeight*ImageWidth];
		thread t(fun, this, blocksIn[i], blocksOut[i]);//"this" is the key for C2839
		if (t.joinable()) t.join();
	}
}

void FastGaussianFilter2D::EstimateBoxBlurParameters(float sigma, byte n, byte* kernelSize)//n is the number of round of box blurring.  
{
	

	//Estimate the width of n averaging kernels. The width is same for each kernel
	byte idealBoxBlurWidth =  (byte) round(sqrt((12 * sigma * sigma / n) + 1));

	//Use two kernels with odd width to approximate the ideal width
	byte lowerOddW = idealBoxBlurWidth % 2 == 0 ? idealBoxBlurWidth - 1 : idealBoxBlurWidth; //Filter 1
	byte upperOddW = lowerOddW + 2; // Filter 2

	//Estimate the number of Filter1 and Filter2
	byte numberOfFilter1 = (byte)round((12 * sigma * sigma - n * lowerOddW * lowerOddW - 4 * n * lowerOddW - 3 * n) 
							/ (-4 * lowerOddW - 4));

	//Send back the kernel size of each round of averaging filter
	for (int i = 0; i < n; i++) {
		kernelSize[i] = i < numberOfFilter1 ? lowerOddW : upperOddW;
	}
	


}

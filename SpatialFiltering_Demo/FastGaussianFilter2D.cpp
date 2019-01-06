#include "pch.h"
#include "FastGaussianFilter2D.h"
#include "MeanFilterByTwo1DKernels.h"
#include <algorithm>
#include <iterator>
FastGaussianFilter2D::FastGaussianFilter2D()
{
}

FastGaussianFilter2D::FastGaussianFilter2D(float sigma)
{
	FastGaussianFilter2D(sigma, 4);
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

void FastGaussianFilter2D::Apply(const ushort * imageIn, int height, int width, ushort * imageOut)
{
	ImageHeight = height;
	ImageWidth = width;
	deque<int> dataSegment;
	size_t threadCount = GetCPUCoreNumber();
	blockHeight = ImageHeight / threadCount + Kernel->RadiusV * 2;
	ushort** fltdSegmentts = new ushort*[threadCount];
	ushort** blockSegments = SegmentImage(imageIn, threadCount);
	for (size_t i = 0; i < threadCount; i++)
	{
		fltdSegmentts[i] = new ushort[blockHeight*ImageWidth];
		thread t(&FastGaussianFilter2D::FilterBlock, this, blockSegments[i], fltdSegmentts[i]);//"this" is the key for C2839
		if (t.joinable()) t.join();
	}
	delete blockSegments;

	ReconstructImage(fltdSegmentts, imageOut, threadCount, blockHeight);

	delete fltdSegmentts;
}

void FastGaussianFilter2D::FilterBlock(const ushort * imageIn, ushort * imageOut)
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

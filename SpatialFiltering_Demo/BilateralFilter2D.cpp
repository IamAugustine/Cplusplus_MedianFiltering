#include "pch.h"
#include "BilateralFilter2D.h"
#define PI 3.14159265358979323846


BilateralFilter2D::BilateralFilter2D(float sigmaS, float sigmaP):sigmaS(sigmaS),sigmaP(sigmaP)
{
	CalculatePixelGaussian();
	Kernel = new GaussianKernel(EstimateSizebySigmaOnly(sigmaS),sigmaS);
}

BilateralFilter2D::BilateralFilter2D(float sigmaS, byte N, float sigmaP) : sigmaS(sigmaS), sigmaP(sigmaP)
{
	CalculatePixelGaussian();
	Kernel = new GaussianKernel(N,sigmaS);
}

BilateralFilter2D::~BilateralFilter2D()
{
}

void BilateralFilter2D::Apply(const ushort * imageIn, int height, int width, ushort * imageOut)
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
		thread t(&BilateralFilter2D::FilterBlock, this, blockSegments[i], fltdSegmentts[i]);//"this" is the key for C2839
		if (t.joinable()) t.join();
	}
	delete blockSegments;

	ReconstructImage(fltdSegmentts, imageOut, threadCount, blockHeight);

	delete fltdSegmentts;
}

ushort BilateralFilter2D::Calculate(deque<ushort> data)
{
	float v = 0;
	ushort p = data.at(Kernel->RadiusH);
	std::transform(data.begin(), data.end(), Kernel->Kernel.begin(), Kernel->Kernel.begin(), [&v, &p, this](ushort d, float f) {v += d * f * pixelWeight[abs(d-p)]; return f; });
	return (ushort)(v + 0.5);
}

void BilateralFilter2D::FilterBlock(const ushort * imageIn, ushort * imageOut)
{
	std::deque<ushort> kernelDeque;
	//std::deque<ushort> tempDeque;
	for (size_t rowIndex = Kernel->RadiusV; rowIndex < blockHeight - Kernel->RadiusV; rowIndex++)
	{
		kernelDeque = InitializeDeque(imageIn, rowIndex);
		//std::nth_element(tempDeque.begin(), tempDeque.begin() + tempDeque.size() / 2, tempDeque.end());
		imageOut[rowIndex * ImageWidth] = Calculate(kernelDeque);
		int newClmIndex = 1;
		while (newClmIndex <= ImageWidth)
		{
			KernelMoveRight(imageIn, rowIndex, newClmIndex + Kernel->RadiusH, kernelDeque);
			imageOut[rowIndex * ImageWidth + newClmIndex] = Calculate(kernelDeque);
			newClmIndex++;
		}
	}
}

void BilateralFilter2D::CalculatePixelGaussian()
{
	unsigned char threeSigma = round(3 * sigmaP);
	pixelWeight = new float[threeSigma];
	for (byte i = 0; i < threeSigma; i++)
	{
		pixelWeight[i] = exp(-1 * i*i / sigmaP / sigmaP) / sqrt(2 * PI) / sigmaP;
	}

}



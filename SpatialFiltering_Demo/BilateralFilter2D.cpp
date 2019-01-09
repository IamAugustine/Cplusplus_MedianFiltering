#include "pch.h"
#include "BilateralFilter2D.h"
#define PI 3.14159265358979323846


BilateralFilter2D::BilateralFilter2D(float sigmaS, float sigmaP):sigmaS(sigmaS),sigmaP(sigmaP)
{
	CalculatePixelGaussian();
	Kernel = (new GaussianKernel(EstimateSizebySigmaOnly(sigmaS),sigmaS));
}

BilateralFilter2D::BilateralFilter2D(float sigmaS, byte N, float sigmaP) : sigmaS(sigmaS), sigmaP(sigmaP)
{
	CalculatePixelGaussian();
	Kernel = new GaussianKernel(N,sigmaS);
}

BilateralFilter2D::~BilateralFilter2D()
{
}

ushort BilateralFilter2D::Calculate(deque<ushort> data)
{
	float v = 0;
	ushort p = data.at((data.size() - 1) / 2);
	std::transform(data.begin(), data.end(), Kernel->Kernel.begin(), Kernel->Kernel.begin(), [&v, &p, this](ushort d, float f) {v += d * f * pixelWeight[abs(d-p)]; return f; });
	return (ushort)(v + 0.5);
}

void BilateralFilter2D::FilterBlock(  ushort * imageIn, ushort * imageOut)
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

void BilateralFilter2D::ProcessingBlocks(ushort ** blocksIn, byte blockHeight, byte threadCount, ushort ** blocksOut)
{
	for (size_t i = 0; i < threadCount; i++)
	{
		blocksOut[i] = new ushort[blockHeight*ImageWidth];
		thread t(fun, this, blocksIn[i], blocksOut[i]);//"this" is the key for C2839
		if (t.joinable()) t.join();
	}
}


void BilateralFilter2D::CalculatePixelGaussian()
{
	unsigned char threeSigma = round(3 * sigmaP);
	pixelWeight = new float[threeSigma];
	float sum = 0;
	for (byte i = 0; i < threeSigma; i++)
	{
		pixelWeight[i] = exp(-1 * i*i / sigmaP / sigmaP) / sqrt(2 * PI) / sigmaP;
		sum += abs(pixelWeight[i]);
	}
	for (byte i = 0; i < threeSigma; i++)
	{
		pixelWeight[i] /= sum;
	}
}



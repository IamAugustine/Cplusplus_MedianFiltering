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
	std::transform(data.begin(), data.end(), Kernel->Kernel.begin(), Kernel->Kernel.begin(), [&v, &p, this](ushort d, float f) {float t;  t = abs(d - p) >= this->presetLength ? 0 : pixelWeight[abs(d - p)]; v += t; return f; });
	return (ushort)(v + 0.5);
}

void BilateralFilter2D::FilterBlock(  ushort * imageIn, ushort * imageOut)
{
	std::deque<ushort> kernelDeque;
	//std::deque<ushort> tempDeque;
	for (size_t rowIndex = bndBufferHeight; rowIndex < blockHeight - bndBufferHeight; rowIndex++)
	{
		kernelDeque = InitializeDeque(imageIn, rowIndex);
		imageOut[rowIndex * ImageWidth] = Calculate(kernelDeque);
		int newClmIndex = 1;
		while (newClmIndex <= ImageWidth)
		{
			KernelMoveRight(imageIn, rowIndex, newClmIndex, kernelDeque);
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
	presetLength = round(3 * sigmaP);
	pixelWeight = new float[presetLength];
	float sum = 0;
	for (byte i = 0; i < presetLength; i++)
	{
		pixelWeight[i] = exp(-1 * i*i / sigmaP / sigmaP) ;
		//sum += abs(pixelWeight[i]);
	}
}



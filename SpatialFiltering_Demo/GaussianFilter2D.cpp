#include "pch.h"
#include "GaussianFilter2D.h"
#include <thread>
#include <numeric>
GaussianFilter2D::GaussianFilter2D()
{
}

GaussianFilter2D::GaussianFilter2D(float sigma)
{
	GaussianKernel(EstimateSizebySigmaOnly(sigma), sigma);
}

GaussianFilter2D::GaussianFilter2D(byte size, float sigma)
{
	GenerateKernel(size, sigma);
}

GaussianFilter2D::GaussianFilter2D(byte sizeX, byte sizeY, float sigmaX, float sigmaY)
{
	GenerateKernel(sizeX, sizeY, sigmaX, sigmaY);
}


GaussianFilter2D::~GaussianFilter2D()
{
}

void GaussianFilter2D::Apply(const ushort * imageIn, int height, int width, ushort * imageOut)
{
	ImageHeight = height;
	ImageWidth = width;
	deque<int> dataSegment;
	size_t threadCount = GetCPUCoreNumber();
	blockHeight = ImageHeight / threadCount + Kernel->RadiusV * 2;
	//int** blockSegments = new int*[threadCount];
	ushort** fltdSegmentts = new ushort*[threadCount];
	ushort** blockSegments = SegmentImage(imageIn, threadCount);
	for (size_t i = 0; i < threadCount; i++)
	{
		fltdSegmentts[i] = new ushort[blockHeight*ImageWidth];
		thread t(&GaussianFilter2D::FilterBlock, this, blockSegments[i], fltdSegmentts[i]);//"this" is the key for C2839
		if (t.joinable()) t.join();
	}
	delete blockSegments;

	ReconstructImage(fltdSegmentts, imageOut, threadCount, blockHeight);

	delete fltdSegmentts;
}

void GaussianFilter2D::FilterBlock(const ushort * imageIn, ushort * imageOut)
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

void GaussianFilter2D::GenerateKernel(byte size, float sigma)
{
	this->Kernel = new GaussianKernel(size, sigma);
}

void GaussianFilter2D::GenerateKernel(byte sizeX, byte sizeY, float sigmaX, float sigmaY)
{
	this->Kernel = new GaussianKernel(sizeX, sizeY, sigmaX, sigmaY);
}

byte GaussianFilter2D::EstimateSizebySigmaOnly(float sigma)
{
	byte order = (byte)round(sigma * 5);
	return order % 2 == 0 ? order + 1 : order;
}

void GaussianFilter2D::KernelMoveRight(const ushort * imgIn, int rowIndex, int clmIndexToAdd, deque<ushort>& tmp)
{
	tmp.erase(tmp.begin(), tmp.begin() + Kernel->VerticalSize);
	int newClmIndex = clmIndexToAdd >= ImageWidth ? 2 * ImageWidth - clmIndexToAdd : clmIndexToAdd;//Mirror boundary
	for (int i = -1 * Kernel->RadiusV; i <= Kernel->RadiusV; i++)
	{
		int kernelRowIndex = (rowIndex + i)*ImageWidth + newClmIndex;
		tmp.push_back(imgIn[kernelRowIndex]);
	}
}

deque<ushort> GaussianFilter2D::InitializeDeque(const ushort * imgIn, const int y)
{
	deque<ushort> originKernel;
	for (int kx = -1 * Kernel->RadiusH; kx <= Kernel->RadiusH; kx++)
	{
		for (int ky = -1 * Kernel->RadiusV; ky <= 1 * Kernel->RadiusV; ky++)
		{
			int pixel00Index = (ky + Kernel->RadiusV + y)*ImageWidth + abs(kx);
			originKernel.push_back(imgIn[pixel00Index]);
		}
	}
	return originKernel;
}

ushort GaussianFilter2D::Calculate(deque<ushort> data)
{
	float v = 0;
	std::transform(data.begin(), data.end(), Kernel->Kernel.begin(), Kernel->Kernel.begin(), [&](ushort d, float f) {v += d * f; return f; });
	return (ushort)(v+0.5);
}

#include "pch.h"
#include "GaussianFilter2D.h"
#include <thread>

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
	blockHeight = ImageHeight / threadCount + Kernel->BoundaryV * 2;
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

#include "pch.h"
#include <algorithm>
#include "MedianFilter2D.h"
#include <thread>
using namespace std;

MedianFilter2D::MedianFilter2D()
{
}

MedianFilter2D::MedianFilter2D(int kernelSize)
{
	Kernel = new MedianKernel(kernelSize);
}

MedianFilter2D::MedianFilter2D(int sizeX, int sizeY)
{
	Kernel = new MedianKernel(sizeX, sizeY);
}


MedianFilter2D::~MedianFilter2D()
{
}

void MedianFilter2D::Apply(ushort * imageIn, int height, int width, ushort * imageOut)
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
		thread t (&MedianFilter2D::FilterBlock,this,blockSegments[i], fltdSegmentts[i] );//"this" is the key for C2839
		if (t.joinable()) t.join();
	}
	delete blockSegments;

	ReconstructImage(fltdSegmentts, imageOut, threadCount, blockHeight);

	delete fltdSegmentts;
}

void MedianFilter2D::KernelMoveRight(ushort * imgIn, int rowIndex, int clmIndexToAdd, deque<ushort>& tmp)
{
	tmp.erase(tmp.begin(), tmp.begin() + Kernel->VerticalSize);
	int newClmIndex = clmIndexToAdd >= ImageWidth ? 2 * ImageWidth - clmIndexToAdd : clmIndexToAdd;//Mirror boundary
	for (int i = -1 * Kernel->BoundaryV; i <= Kernel->BoundaryV; i++)
	{
		int kernelRowIndex = (rowIndex + i)*ImageWidth + newClmIndex;
		tmp.push_back(imgIn[kernelRowIndex]);
	}
}

deque<ushort> MedianFilter2D::InitializeDeque(ushort * imgIn, const int y)
{
	//const size_t kernelSize = boundary * 2 + 1;
	deque<ushort> originKernel;
	for (int kx = -1 * Kernel->BoundaryH; kx <= Kernel->BoundaryH; kx++)
	{
		for (int ky = -1 * Kernel->BoundaryV; ky <= 1 * Kernel->BoundaryV; ky++)
		{
			int pixel00Index = (ky + Kernel->BoundaryV + y)*ImageWidth + abs(kx);
			originKernel.push_back(imgIn[pixel00Index]);
		}
	}
	return originKernel;
}

void MedianFilter2D::FilterBlock(ushort * imgIn, ushort * imgOut)
{
	//imgOut = new int[blockHeight*width];
	std::deque<ushort> kernelDeque;
	std::deque<ushort> tempDeque;
	for (size_t rowIndex = Kernel->BoundaryV; rowIndex < blockHeight - Kernel->BoundaryV; rowIndex++)
	{
		kernelDeque = InitializeDeque(imgIn, rowIndex);
		tempDeque = kernelDeque;
		int newClmIndex = 0;
		std::nth_element(tempDeque.begin(), tempDeque.begin() + tempDeque.size() / 2, tempDeque.end());
		imgOut[rowIndex * ImageWidth + newClmIndex] = tempDeque[tempDeque.size() / 2];
		newClmIndex++;
		while (newClmIndex <= ImageWidth)
		{
			KernelMoveRight(imgIn, rowIndex, newClmIndex + Kernel->BoundaryH, kernelDeque);
			tempDeque = kernelDeque;
			std::nth_element(tempDeque.begin(), tempDeque.begin() + tempDeque.size() / 2, tempDeque.end());
			imgOut[rowIndex * ImageWidth + newClmIndex] = tempDeque[tempDeque.size() / 2];
			newClmIndex++;
		}
	}
}

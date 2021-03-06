#include "pch.h"
#include "FilterBase.h"
#include <Windows.h>
#include "IOHelper.h"

Filter2D::Filter2D()
{
	
}


Filter2D::~Filter2D()
{
}

void Filter2D::Apply(  ushort * imageIn, int height, int width, ushort * imageOut)
{
	ImageHeight = height;
	ImageWidth = width;
	deque<int> dataSegment;
	size_t threadCount = GetCPUCoreNumber();
	bndBufferHeight = Kernel->RadiusV;
	blockHeight = ImageHeight / threadCount + bndBufferHeight * 2;
	//int** blockSegments = new int*[threadCount];
	ushort** fltdSegmentts = new ushort*[threadCount];
	ushort** blockSegments = SegmentImage(imageIn, threadCount);

	ProcessingBlocks(blockSegments, blockHeight, threadCount, fltdSegmentts);

	delete blockSegments;

	ReconstructImage(fltdSegmentts, imageOut, threadCount, blockHeight);

	delete fltdSegmentts;
}

void Filter2D::FilterBlock(  ushort * imageIn, ushort * imageOut)
{
	std::deque<ushort> kernelDeque;
	//std::deque<ushort> tempDeque;
	for (size_t rowIndex = bndBufferHeight; rowIndex < blockHeight - bndBufferHeight; rowIndex++)
	{
		kernelDeque = InitializeDeque(imageIn, rowIndex);
		//std::nth_element(tempDeque.begin(), tempDeque.begin() + tempDeque.size() / 2, tempDeque.end());
		imageOut[rowIndex * ImageWidth] = Calculate(kernelDeque);
		int newClmIndex = 1;
		while (newClmIndex < ImageWidth)
		{
			KernelMoveRight(imageIn, rowIndex, newClmIndex + Kernel->RadiusH, kernelDeque);
			imageOut[rowIndex * ImageWidth + newClmIndex] = Calculate(kernelDeque);
			newClmIndex++;
		}
	}
}

void Filter2D::KernelMoveRight(  ushort * imgIn, int rowIndex, int clmIndexToAdd, deque<ushort>& tmp)
{
	tmp.erase(tmp.begin(), tmp.begin() + Kernel->VerticalSize);
	int newClmIndex = clmIndexToAdd >= ImageWidth ? 2 * ImageWidth - clmIndexToAdd-1 : clmIndexToAdd;//Mirror boundary
	for (int i = -1 * Kernel->RadiusV; i <= Kernel->RadiusV; i++)
	{
		int kernelRowIndex = (rowIndex + i)*ImageWidth + newClmIndex;
		tmp.push_back(imgIn[kernelRowIndex]);
	}
}

deque<ushort> Filter2D::InitializeDeque(  ushort * imgIn,   int y)
{
	deque<ushort> originKernel;
	for (int kx = -1 * Kernel->RadiusH; kx <= Kernel->RadiusH; kx++)
	{
		for (int ky = -1 * Kernel->RadiusV; ky <= 1 * Kernel->RadiusV; ky++)
		{
			int pixel00Index = (ky+ y)*ImageWidth + abs(kx);
			originKernel.push_back(imgIn[pixel00Index]);
		}
	}
	return originKernel;
}

ushort Filter2D::Calculate(deque<ushort> data)
{
	float v = 0;
	std::transform(data.begin(), data.end(), Kernel->Kernel.begin(), Kernel->Kernel.begin(), [&](ushort d, float f) {v += d * f; return f; });
	return (ushort)(v + 0.5);
}

void Filter2D::ProcessingBlocks(ushort ** blocksIn, byte blockHeight, byte threadCount, ushort ** blocksOut)
{
	
	for (size_t i = 0; i < threadCount; i++)
	{
		blocksOut[i] = new ushort[blockHeight*ImageWidth];
		thread t(fun, this, blocksIn[i], blocksOut[i]);//"this" is the key for C2839
		if (t.joinable()) t.join();
	}
	IOHelper::SaveToLocalFile("f1.txt", blocksOut[0], blockHeight, ImageWidth);
}

int Filter2D::GetCPUCoreNumber()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwNumberOfProcessors;
}

ushort ** Filter2D::SegmentImage(  ushort * imgIn, int blockNumber)
{
	ushort** segments = new ushort*[blockNumber];
	ushort* temp = new ushort[(ImageHeight + bndBufferHeight * 2)*ImageWidth];
	//Copy image
	std::copy(imgIn, imgIn + ImageHeight * ImageWidth, temp + bndBufferHeight * ImageWidth);
	//Copy boundary
	std::copy(imgIn, imgIn + bndBufferHeight * ImageWidth, temp);
	std::copy(imgIn + (ImageHeight - bndBufferHeight)*ImageWidth, imgIn + ImageHeight * ImageWidth, temp + (ImageHeight + bndBufferHeight) * ImageWidth);
	//IOHelper::SaveToLocalFile("block0.txt", temp, height, width);
	size_t blockHeight = ImageHeight / blockNumber + bndBufferHeight * 2;
	size_t blockSize = blockHeight * ImageWidth;
	//Segment images
	for (int i = 0; i < blockNumber; i++)
	{
		segments[i] = new ushort[blockSize];
		int segmentPoint = i * (blockHeight - 2 * bndBufferHeight)*ImageWidth;
		std::copy(temp + segmentPoint, temp + segmentPoint + blockSize, segments[i]);
	}
	delete temp;
	return segments;
}

void Filter2D::ReconstructImage(ushort ** segments, ushort * imgOut, int blockNumber,   int blockHeight)
{
	size_t blockSize = (blockHeight - bndBufferHeight * 2)*ImageWidth;
	int entryPoint = bndBufferHeight * ImageWidth;

	for (int i = 0; i < blockNumber; i++)
	{
		int imgRowIndex = i * (blockHeight - bndBufferHeight * 2);
		std::copy(segments[i] + entryPoint, segments[i] + entryPoint + blockSize, imgOut + imgRowIndex * ImageWidth);
	}
}

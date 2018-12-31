#include "pch.h"
#include "FilterBase.h"
#include <Windows.h>

FilterBase::FilterBase()
{
}


FilterBase::~FilterBase()
{
}

void FilterBase::Apply(ushort * imageIn, int height, int width, ushort * imageOut)
{
}

int FilterBase::GetCPUCoreNumber()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwNumberOfProcessors;
}

ushort ** FilterBase::SegmentImage(ushort * imgIn, int blockNumber)
{
	ushort** segments = new ushort*[blockNumber];
	ushort* temp = new ushort[(ImageHeight + Kernel->BoundaryV * 2)*ImageWidth];
	//Copy image
	std::copy(imgIn, imgIn + ImageHeight * ImageWidth, temp + Kernel->BoundaryV * ImageWidth);
	//Copy boundary
	std::copy(imgIn, imgIn + Kernel->BoundaryV * ImageWidth, temp);
	std::copy(imgIn + (ImageHeight - Kernel->BoundaryV)*ImageWidth, imgIn + ImageHeight * ImageWidth, temp + (ImageHeight + Kernel->BoundaryV) * ImageWidth);
	//IOHelper::SaveToLocalFile("block0.txt", temp, height, width);
	size_t blockHeight = ImageHeight / blockNumber + Kernel->BoundaryV * 2;
	size_t blockSize = blockHeight * ImageWidth;
	//Segment images
	for (int i = 0; i < blockNumber; i++)
	{
		segments[i] = new ushort[blockSize];
		int segmentPoint = i * (blockHeight - 2 * Kernel->BoundaryV)*ImageWidth;
		std::copy(temp + segmentPoint, temp + segmentPoint + blockSize, segments[i]);
	}
	delete temp;
	return segments;
}

void FilterBase::ReconstructImage(ushort ** segments, ushort * imgOut, int blockNumber, const int blockHeight)
{
	size_t blockSize = (blockHeight - Kernel->BoundaryV * 2)*ImageWidth;
	int entryPoint = Kernel->BoundaryV * ImageWidth;

	for (int i = 0; i < blockNumber; i++)
	{
		int imgRowIndex = i * (blockHeight - Kernel->BoundaryV * 2);
		std::copy(segments[i] + entryPoint, segments[i] + entryPoint + blockSize, imgOut + imgRowIndex * ImageWidth);
	}
}

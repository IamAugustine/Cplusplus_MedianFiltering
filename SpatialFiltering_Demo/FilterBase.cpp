#include "pch.h"
#include "FilterBase.h"
#include <Windows.h>

FilterBase::FilterBase()
{
}


FilterBase::~FilterBase()
{
}

void FilterBase::Apply(const ushort * imageIn, int height, int width, ushort * imageOut)
{
}

int FilterBase::GetCPUCoreNumber()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwNumberOfProcessors;
}

ushort ** FilterBase::SegmentImage(const ushort * imgIn, int blockNumber)
{
	ushort** segments = new ushort*[blockNumber];
	ushort* temp = new ushort[(ImageHeight + Kernel->RadiusV * 2)*ImageWidth];
	//Copy image
	std::copy(imgIn, imgIn + ImageHeight * ImageWidth, temp + Kernel->RadiusV * ImageWidth);
	//Copy boundary
	std::copy(imgIn, imgIn + Kernel->RadiusV * ImageWidth, temp);
	std::copy(imgIn + (ImageHeight - Kernel->RadiusV)*ImageWidth, imgIn + ImageHeight * ImageWidth, temp + (ImageHeight + Kernel->RadiusV) * ImageWidth);
	//IOHelper::SaveToLocalFile("block0.txt", temp, height, width);
	size_t blockHeight = ImageHeight / blockNumber + Kernel->RadiusV * 2;
	size_t blockSize = blockHeight * ImageWidth;
	//Segment images
	for (int i = 0; i < blockNumber; i++)
	{
		segments[i] = new ushort[blockSize];
		int segmentPoint = i * (blockHeight - 2 * Kernel->RadiusV)*ImageWidth;
		std::copy(temp + segmentPoint, temp + segmentPoint + blockSize, segments[i]);
	}
	delete temp;
	return segments;
}

void FilterBase::ReconstructImage(ushort ** segments, ushort * imgOut, int blockNumber, const int blockHeight)
{
	size_t blockSize = (blockHeight - Kernel->RadiusV * 2)*ImageWidth;
	int entryPoint = Kernel->RadiusV * ImageWidth;

	for (int i = 0; i < blockNumber; i++)
	{
		int imgRowIndex = i * (blockHeight - Kernel->RadiusV * 2);
		std::copy(segments[i] + entryPoint, segments[i] + entryPoint + blockSize, imgOut + imgRowIndex * ImageWidth);
	}
}

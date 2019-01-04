#include "pch.h"
#include "MeanFilterByTwo1DKernels.h"




MeanFilterByTwo1DKernels::MeanFilterByTwo1DKernels(byte kernelSize)
{
	MeanFilterByTwo1DKernels(kernelSize, kernelSize);
}

MeanFilterByTwo1DKernels::MeanFilterByTwo1DKernels(byte sizeX, byte sizeY)
{
	Kernel = new MeanKernel(sizeY, sizeX);
}

MeanFilterByTwo1DKernels::~MeanFilterByTwo1DKernels()
{
	delete Kernel;
}

void MeanFilterByTwo1DKernels::Apply(const ushort * imageIn, int height, int width, ushort * imageOut)
{
	ImageHeight = height;
	ImageWidth = width;
	deque<int> dataSegment;
	size_t threadCount = GetCPUCoreNumber();
	blockHeight = ImageHeight / threadCount + Kernel->BoundaryV * 2;
	ushort** fltdSegmentts = new ushort*[threadCount];
	ushort** blockSegments = SegmentImage(imageIn, threadCount);
	for (size_t i = 0; i < threadCount; i++)
	{
		fltdSegmentts[i] = new ushort[blockHeight*ImageWidth];
		thread t(&MeanFilterByTwo1DKernels::FilterBlock, this, blockSegments[i], fltdSegmentts[i]);//"this" is the key for C2839
		if (t.joinable()) t.join();
	}
	delete blockSegments;

	ReconstructImage(fltdSegmentts, imageOut, threadCount, blockHeight);

	delete fltdSegmentts;
}

void MeanFilterByTwo1DKernels::FilterBlock(const ushort * imageIn, ushort * imageOut)
{
	ushort* temp = new ushort[blockHeight*ImageWidth];
	MeanFilter1D_H(imageIn, temp);
	MeanFilter1D_T(temp, imageOut);
	delete temp;
}

void MeanFilterByTwo1DKernels::MeanFilter1D_H(const ushort * imageIn, ushort * imageOut)
{
	for (size_t rowIndex = Kernel->BoundaryV; rowIndex < blockHeight - Kernel->BoundaryV; rowIndex++)
	{
		int rowOffset = rowIndex * ImageWidth;
		imageOut[rowOffset] = GetFirstPixelPerRow(imageIn, rowIndex);
		int newClmIndex = 1;
		while (newClmIndex < ImageWidth)
		{
			int pixelToAdd = newClmIndex + Kernel->BoundaryH;
			if (pixelToAdd >= ImageWidth)
				pixelToAdd = 2 * ImageWidth - pixelToAdd; //mirror boundary
			int pixelToRemove = abs(newClmIndex - Kernel->BoundaryH - 1);//mirror boundary
			imageOut[rowOffset + newClmIndex] = ShiftPixel(imageOut[rowOffset + newClmIndex - 1], imageIn[rowOffset + pixelToAdd], imageIn[rowOffset + pixelToRemove], Kernel->BoundaryH);
			//imageOut[rowOffset + newClmIndex] = (ushort)((imageOut[rowOffset + newClmIndex - 1]*Kernel->HorizontalSize + (imageIn[rowOffset + pixelToAdd] - imageIn[rowOffset + pixleToRemove])*1.0f / Kernel->HorizontalSize));
			newClmIndex++;
		}
	}
}


void MeanFilterByTwo1DKernels::MeanFilter1D_T(const ushort * imageIn, ushort * imageOut)
{
	for (size_t columnIndex = 0; columnIndex < ImageWidth; columnIndex++)
	{
		int columnOffset = columnIndex + Kernel->BoundaryV*ImageWidth;
		imageOut[columnOffset] = GetFirstPixelPerColumn(imageIn, columnIndex);
		int newRowIndex = 1;
		while (newRowIndex < ImageHeight)
		{
			int pixelToAdd = newRowIndex + Kernel->BoundaryV;
			if (pixelToAdd >= ImageHeight)
				pixelToAdd = 2 * ImageHeight - pixelToAdd; //mirror boundary
			int pixelToRemove = abs(newRowIndex - Kernel->BoundaryV - 1);//mirror boundary
			imageOut[columnOffset + newRowIndex * ImageWidth] = ShiftPixel(imageOut[columnOffset + (newRowIndex - 1)*ImageWidth], imageIn[columnOffset + pixelToAdd * ImageWidth], imageIn[columnOffset + pixelToRemove * ImageWidth], Kernel->BoundaryV);
			//imageOut[rowOffset + newClmIndex] = (ushort)((imageOut[rowOffset + newClmIndex - 1]*Kernel->HorizontalSize + (imageIn[rowOffset + pixelToAdd] - imageIn[rowOffset + pixleToRemove])*1.0f / Kernel->HorizontalSize));
			newRowIndex ++;
		}
	}
}

ushort MeanFilterByTwo1DKernels::GetFirstPixelPerRow(const ushort * imgIn, const int rowIndex)
{
	float mean = 0;
		for (int kx = -1 * Kernel->BoundaryH; kx <= 1 * Kernel->BoundaryH; kx++)
		{
			int pixel00Index = (Kernel->BoundaryV + rowIndex)*ImageWidth + abs(kx);
			mean += (float)(imgIn[pixel00Index]) / Kernel->HorizontalSize;
		}
		return (ushort)(mean + 0.5);

}

ushort MeanFilterByTwo1DKernels::GetFirstPixelPerColumn(const ushort * imgIn, const int columnIndex)
{
	float mean = 0;
	for (int ky = 0; ky <= Kernel->VerticalSize; ky++)
	{
		int pixel00Index = ky * ImageWidth + columnIndex;
		mean += (float)(imgIn[pixel00Index]) / Kernel->VerticalSize;
	}
	return (ushort)(mean + 0.5);
}

ushort MeanFilterByTwo1DKernels::ShiftPixel(ushort lastPixel, ushort pixelToAdd, ushort pixelToRemove, byte w)
{
	float mean = (float)(lastPixel*w - pixelToRemove + pixelToAdd) / w;
	return (ushort)(mean + 0.5);
}

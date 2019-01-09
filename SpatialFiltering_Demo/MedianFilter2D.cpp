#include "pch.h"
#include <algorithm>
#include "MedianFilter2D.h"
#include <thread>
#include "IOHelper.h"

using namespace std;

MedianFilter2D::MedianFilter2D(int sizeX, int sizeY)
{
	Kernel = new MedianKernel(sizeX, sizeY);
}


MedianFilter2D::~MedianFilter2D()
{
}

void MedianFilter2D::FilterBlock(  ushort * imgIn, ushort * imgOut)
{
	//imgOut = new int[blockHeight*width];
	std::deque<ushort> kernelDeque;
	std::deque<ushort> tempDeque;
	for (size_t rowIndex = Kernel->RadiusV; rowIndex < blockHeight - Kernel->RadiusV; rowIndex++)
	{
		kernelDeque = InitializeDeque(imgIn, rowIndex);
		tempDeque = kernelDeque;
		int newClmIndex = 0;
		std::nth_element(tempDeque.begin(), tempDeque.begin() + tempDeque.size() / 2, tempDeque.end());
		imgOut[rowIndex * ImageWidth + newClmIndex] = tempDeque[(tempDeque.size()-1) / 2];
		newClmIndex++;
		while (newClmIndex <= ImageWidth)
		{
			KernelMoveRight(imgIn, rowIndex, newClmIndex + Kernel->RadiusH, kernelDeque);
			tempDeque = kernelDeque;
			std::nth_element(tempDeque.begin(), tempDeque.begin() + tempDeque.size() / 2, tempDeque.end());
			imgOut[rowIndex * ImageWidth + newClmIndex] = tempDeque[tempDeque.size() / 2];
			newClmIndex++;
		}
	}
}

void MedianFilter2D::ProcessingBlocks(ushort ** blocksIn, byte blockHeight, byte threadCount, ushort ** blocksOut)
{
	for (size_t i = 0; i < threadCount; i++)
	{
		blocksOut[i] = new ushort[blockHeight*ImageWidth];
		thread t(fun, this, blocksIn[i], blocksOut[i]);
		if (t.joinable()) t.join();
	}
}

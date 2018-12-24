#include "pch.h"
#include "MedianFilter.h"
#include <Windows.h>
#include <thread>;
#include <algorithm>
#include <iterator>
#include <map>;
#include "IOHelper.h"
using namespace std;
MedianFilter::MedianFilter()
{
}


MedianFilter::~MedianFilter()
{
}

void MedianFilter::Apply(int * imgIn, int * imgOut, const int height, const int width, const int kernelSize)
{
	{
		int boundary = (kernelSize - 1) / 2;
		deque<int> dataSegment;
		size_t threadCount = GetCPUCoreNumber();
		int blockHeight = height / threadCount + boundary*2;
		//int** blockSegments = new int*[threadCount];
		int** fltdSegmentts = new int*[threadCount];
		int** blockSegments = SegmentImage(imgIn, threadCount, height, width, boundary);
		int* temp = blockSegments[1];
		for (size_t i = 0; i < threadCount; i++)
		{
			fltdSegmentts[i] = new int[blockHeight*width];
			std::thread t{ FilterBlock, blockSegments[i], fltdSegmentts[i], blockHeight, width, boundary };
			if (t.joinable()) t.join();
		}


		ReconstructImage(fltdSegmentts, imgOut, threadCount, blockHeight, height, width, boundary);
		delete blockSegments;
		delete fltdSegmentts;

	}
}

void MedianFilter::FilterBlock(int * imgIn, int * imgOut, const int blockHeight, const int width, const int boundary)
{
	//imgOut = new int[blockHeight*width];
	std::deque<int> kernelDeque;
	std::deque<int> tempDeque;
	for (size_t rowIndex = boundary; rowIndex < blockHeight-boundary; rowIndex++)
	{
		kernelDeque = InitializeDeque(imgIn, rowIndex, boundary, width);
		tempDeque = kernelDeque;
		int newClmIndex = 0;
		std::nth_element(tempDeque.begin(), tempDeque.begin() + tempDeque.size() / 2, tempDeque.end());
		imgOut[rowIndex * width+newClmIndex] = tempDeque[tempDeque.size() / 2];
		newClmIndex++;
		while (newClmIndex <= width)
		{
			KernelMoveRight(imgIn, rowIndex, newClmIndex+boundary, width, boundary, kernelDeque);
			tempDeque = kernelDeque;
			std::nth_element(tempDeque.begin(), tempDeque.begin() + tempDeque.size() / 2, tempDeque.end());
			imgOut[rowIndex * width + newClmIndex] = tempDeque[tempDeque.size() / 2];
			newClmIndex++;
		}
	}

}
deque<int> MedianFilter::InitializeDeque(int* imgIn, const int y, const int boundary, const int width)
{
	const size_t kernelSize = boundary * 2 + 1;
	deque<int> originKernel;
	for (int kx = -1 * boundary; kx <= boundary; kx++)
	{
		for (int ky = -1 * boundary; ky <= 1 * boundary; ky++)
		{
			int pixel00Index = (ky + boundary+y)*width+abs(kx);
			originKernel.push_back(imgIn[pixel00Index]);
		}
	}
	return originKernel;
}
void MedianFilter::KernelMoveRight(int* imgIn, int rowIndex, int clmIndexToAdd, int width, int boundary,  deque <int>& tmp)
{
	tmp.erase(tmp.begin(), tmp.begin() + boundary * 2 + 1);
	int newClmIndex = clmIndexToAdd >= width ? 2 * width - clmIndexToAdd : clmIndexToAdd;//Mirror boundary
	for (int i = -1*boundary; i <= boundary; i++)
	{
		int kernelRowIndex = (rowIndex + i)*width + newClmIndex ;
		tmp.push_back(imgIn[kernelRowIndex]);
	}
}
int** MedianFilter::SegmentImage(int * imgIn, int blockNumber, const int height, const int width, const int boundary)
{
	int** segments = new int*[blockNumber];
	int* temp = new int[(height + boundary * 2)*width];
	//Copy image
	std::copy(imgIn, imgIn + height * width, temp+boundary*width);
	//Copy boundary
	std::copy(imgIn, imgIn + boundary * width, temp);
	std::copy(imgIn + (height - boundary)*width, imgIn + height * width, temp + (height+boundary) * width);
	//IOHelper::SaveToLocalFile("block0.txt", temp, height, width);
	size_t blockHeight = height / blockNumber + boundary * 2;
	size_t blockSize = blockHeight * width;
	//Segment images
	for (int i = 0; i < blockNumber; i++)
	{
		segments[i] = new int[blockSize];
		int segmentPoint = i * (blockHeight - 2*boundary)*width;
		std::copy(temp + segmentPoint, temp + segmentPoint + blockSize, segments[i]);
	}
	delete temp;
	return segments;
}

void MedianFilter::ReconstructImage(int ** segments, int* imgOut, int blockNumber, const int blockHeight, const const int height, const int width, const int boundary)
{
	//imgOut = new int[height*width];
	
	size_t blockSize = (blockHeight - boundary * 2)*width;
	int entryPoint = boundary * width;

	for (int i = 0; i < blockNumber; i++)
	{
		int imgRowIndex = i * (blockHeight - boundary * 2);
		int* block = segments[i];
		std::copy(segments[i] + entryPoint, segments[i] + entryPoint + blockSize, imgOut + imgRowIndex * width);
	}
}

#pragma once
using namespace std;
#include <vector>;
#include <deque>
#include "FilterBaseClass.h"

class MedianFilter:public FilterBaseClass
{
public:
	MedianFilter();
	~MedianFilter();
private:
	
	static void FilterBlock(int* imgIn, int* imgOut, const int height, const int width, const int kernelSize);
	static deque<int> InitializeDeque(int * imgIn, const int y, const int boundary, const int width);
	static void KernelMoveRight(int * imgIn, int rowIndex, int clmIndexToAdd, int width, int boundary, deque<int>& tmp);
	static int** SegmentImage(int* imgIn, int blockNumber, const const int height, const int width, const int kernelSize);
	static void ReconstructImage(int ** segments, int* imgOut, int blockNumber, const int blockHeight, const const int height, const int width, const int boundary);
public:
	static void Apply(int* imgIn, int* imgOut, const int height, const int width, const int kernelSize);
	
};


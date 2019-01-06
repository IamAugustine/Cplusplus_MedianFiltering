#pragma once
#include "FilterBase.h"
#ifndef _GaussianFilter2D_H_
#define _GaussianFilter2D_H_
#endif // !_GaussianFilter2D_H_

class GaussianFilter2D :
	public FilterBase
{
public:
	GaussianFilter2D();
	GaussianFilter2D(float sigma);
	GaussianFilter2D(byte size, float sigma);
	GaussianFilter2D(byte sizeX, byte sizeY, float sigmaX, float sigmaY);
	~GaussianFilter2D();
	void Apply(const ushort* imageIn, int height, int width, ushort* imageOut);
	void FilterBlock(const ushort* imageIn, ushort* imageOut);
	byte EstimateSizebySigmaOnly(float sigma);
	void KernelMoveRight(const ushort * imgIn, int rowIndex, int clmIndexToAdd, deque<ushort>& tmp);
	deque<ushort> InitializeDeque(const ushort * imgIn, const int y);
	virtual ushort Calculate(deque<ushort> data);
private:
	void GenerateKernel(byte size, float sigma);
	void GenerateKernel(byte sizeX, byte sizeY,  float sigmaX, float sigmaY);

	

};


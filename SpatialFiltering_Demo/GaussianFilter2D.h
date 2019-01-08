#pragma once
#include "FilterBase.h"
#ifndef _GaussianFilter2D_H_
#define _GaussianFilter2D_H_
#endif // !_GaussianFilter2D_H_

class GaussianFilter2D :
	public Filter2D
{
public:
	GaussianFilter2D();
	GaussianFilter2D(float sigma);
	GaussianFilter2D(byte size, float sigma);
	GaussianFilter2D(byte sizeX, byte sizeY, float sigmaX, float sigmaY);
	~GaussianFilter2D();
public:
	byte EstimateSizebySigmaOnly(float sigma);
private:
	void GenerateKernel(byte size, float sigma);
	void GenerateKernel(byte sizeX, byte sizeY,  float sigmaX, float sigmaY);

	

};


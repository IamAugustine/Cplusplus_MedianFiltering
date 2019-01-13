#include "pch.h"
#include "GaussianFilter2D.h"
#include <thread>
#include <numeric>
GaussianFilter2D::GaussianFilter2D()
{
}

GaussianFilter2D::GaussianFilter2D(float sigma)
{
	GaussianKernel(EstimateSizebySigmaOnly(sigma), sigma);
}


GaussianFilter2D::GaussianFilter2D(byte sizeX, byte sizeY, float sigmaX, float sigmaY)
{
	GenerateKernel(sizeX, sizeY, sigmaX, sigmaY);
}


GaussianFilter2D::~GaussianFilter2D()
{
}


void GaussianFilter2D::GenerateKernel(byte sizeX, byte sizeY, float sigmaX, float sigmaY)
{
	this->Kernel = new GaussianKernel(sizeX, sizeY, sigmaX, sigmaY);
}

byte GaussianFilter2D::EstimateSizebySigmaOnly(float sigma)
{
	byte order = (byte)round(sigma * 3);
	return order % 2 == 0 ? order + 1 : order;
}

//void GaussianFilter2D::ProcessingBlocks(ushort ** blocksIn, byte blockHeight, byte threadCount, ushort ** blocksOut)
//{
//	for (size_t i = 0; i < threadCount; i++)
//	{
//		blocksIn[i] = new ushort[blockHeight*ImageWidth];
//		thread t(fun, this, blocksIn[i], blocksOut[i]);//"this" is the key for C2839
//		if (t.joinable()) t.join();
//	}
//}

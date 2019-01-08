#pragma once
#include <array>
#include <vector>
#include <functional>
typedef unsigned char byte;
constexpr byte MAX_KERNEL_SIZE = 21;




using namespace std;
class FilterKernel
{

public:
	FilterKernel();
	FilterKernel(byte verticalSize, byte horizontalSize, float* kernel) :VerticalSize(verticalSize), HorizontalSize(horizontalSize)
	{
		kernelLength = verticalSize * horizontalSize;
		Kernel.resize(kernelLength);
		Kernel.assign(kernel[0], kernel[kernelLength]);
	}
	~FilterKernel();

public:
	byte VerticalSize;
	byte HorizontalSize;
	vector<float> Kernel{ MAX_KERNEL_SIZE * MAX_KERNEL_SIZE,1 };
	byte RadiusH;
	byte RadiusV;
private:
	int kernelLength;
};

class MeanKernel :public FilterKernel
{
public:
	MeanKernel(byte kernelSize = 3) :MeanKernel(kernelSize, kernelSize) {};
	MeanKernel(byte verticalSize, byte horizontalSize);
};
class GaussianKernel :public FilterKernel
{
public:
	GaussianKernel(byte sizeX =3, byte sizeY = 3, float sigmaX =1, float sigmaY = 1);
	GaussianKernel(byte size, float sigma) :GaussianKernel(size, size, sigma, sigma) {};
private:
	float sigmaX;
	float sigmaY;
};
class LinearMotionBlurKernel :public FilterKernel
{
public:
	LinearMotionBlurKernel(byte size, float angle);
	byte* NonZeroIndexInSparseKernel;

private:
	void CalculateKernel(byte size, float angle);
};

class SobelKernel :public FilterKernel
{

};
class MedianKernel :public FilterKernel
{
public:
	MedianKernel(byte size) :MedianKernel(size, size) {};
	MedianKernel(byte sizeX, byte sizeY);
};
class LaplacianKernel :public FilterKernel
{
public:
	LaplacianKernel(float alpha);
};
class LaplacianOfGaussianKernel :public FilterKernel
{
public:
	LaplacianOfGaussianKernel(float alpha, float sigma);
};

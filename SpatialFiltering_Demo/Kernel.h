#pragma once
#include <array>
#include <vector>
#include <functional>
typedef unsigned char byte;
constexpr byte MAX_KERNEL_SIZE = 11;




using namespace std;
class FilterKernel
{

public:
	FilterKernel();
	template<typename T> FilterKernel(T verticalSize, T horizontalSize, float* kernel) :VerticalSize((byte)verticalSize), HorizontalSize((byte)horizontalSize)
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
	MeanKernel();
	MeanKernel(byte kernelSize=3);
	MeanKernel(byte verticalSize, byte horizontalSize);
	//~MeanKernel();
};
class GaussianKernel :public FilterKernel
{
public:
	//GaussianKernel();
	GaussianKernel(byte sizeX =3, byte sizeY = 3, float sigmaX =1, float sigmaY = 1);
	GaussianKernel(byte size =3, float sigma =1);
	//~GaussianKernel();
private:
	float sigmaX;
	float sigmaY;
};
class LinearMotionBlurKernel :public FilterKernel
{
public:
	//MotionBlur();
	
	LinearMotionBlurKernel(byte size, float angle);
	//~LinearMotionBlurKernel();
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
	MedianKernel(byte size);
	MedianKernel(byte sizeX, byte sizeY);
};
class LaplacianKernel :public FilterKernel
{
public:
	LaplacianKernel(float alpha);
	//~LaplacianKernel();
};

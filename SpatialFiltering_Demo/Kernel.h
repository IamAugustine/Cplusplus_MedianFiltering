#pragma once
#include <array>
#include <vector>
#define MAX_KERNEL_SIZE 11
typedef unsigned char byte;
using namespace std;
class FilterKernel
{
public:
	FilterKernel();
	template<typename T> FilterKernel(T verticalSize, T horizontalSize, float* kernel):VerticalSize((byte)verticalSize), HorizontalSize((byte)horizontalSize)
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
	byte BoundaryH;
	byte BoundaryV;
private:
	int kernelLength;
public:
	enum KernelName
	{
		Average3x3,
		Average5x5,
		Gaussian3x3,
		Gaussian5x5,
		Gaussian7x7,
		SobelVertical,
		SobelHorizontal,
		MotionBlur,
		MotionBlur45,
		MotionBLur135,
		MotionBlur225,
		MotionBlur315,


	};
	static FilterKernel GetTypicalKernel(KernelName name)
	{
		switch (name)
		{
		default:
			break;
		}
	}
};
class MeanKernel :public FilterKernel
{
public:
	MeanKernel();
	MeanKernel(byte kernelSize=3);
	MeanKernel(byte verticalSize, byte horizontalSize);
	~MeanKernel();
};
class GaussianKernel :public FilterKernel
{
public:
	//GaussianKernel();
	GaussianKernel(byte sizeX =3, byte sizeY = 3, float sigmaX =1, float sigmaY = 1);
	GaussianKernel(byte size =3, float sigma =1);
	~GaussianKernel();
private:
	float sigmaX;
	float sigmaY;
};
class MotionBlur :public FilterKernel
{
public:
	//MotionBlur();
	MotionBlur(byte size);
	MotionBlur(byte size, float angle);
	~MotionBlur();
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
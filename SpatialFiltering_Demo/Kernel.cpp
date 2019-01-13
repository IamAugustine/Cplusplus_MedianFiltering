#include "pch.h"
#include "Kernel.h"
#include <math.h>
constexpr auto PI = 3.14159265358979323846;
#define Square(x) (x)*(x)
template<typename T> inline float DegreeToRadian(T t) {return ((int)t % 180) * PI / 180; };

FilterKernel::FilterKernel()
{
}
FilterKernel::FilterKernel(byte verticalSize, byte horizontalSize) :VerticalSize(verticalSize), HorizontalSize(horizontalSize)
{
	RadiusH = (horizontalSize - 1) / 2;
	RadiusV = (verticalSize - 1) / 2;
}
FilterKernel::~FilterKernel()
{
	std::vector<float>().swap(Kernel);
}

//MeanKernel::MeanKernel(byte kernelSize) :MeanKernel(kernelSize, kernelSize) {}

MeanKernel::MeanKernel(byte verticalSize, byte horizontalSize)
{
	VerticalSize = verticalSize;
	HorizontalSize = horizontalSize;
	Kernel.resize(verticalSize*horizontalSize);
	RadiusH = (HorizontalSize - 1) / 2;
	RadiusV = (VerticalSize - 1) / 2;
	std::fill(Kernel.begin(), Kernel.end(), 1.0f / verticalSize * horizontalSize);
}

GaussianKernel::GaussianKernel(byte sizeX, byte sizeY, float sigmaX, float sigmaY):sigmaX(sigmaX), sigmaY(sigmaY)
{
	VerticalSize = sizeY;
	HorizontalSize = sizeX;
	Kernel.clear();
	RadiusH = (HorizontalSize - 1) / 2;
	RadiusV = (VerticalSize - 1) / 2;
	float sum = 0;
	for (int i = 0; i < HorizontalSize; i++)
	{
		for (int j = 0; j < VerticalSize; j++)
		{
			float gValue =  exp(-1 * (Square(i - RadiusH) / (2 * Square(sigmaX)) + Square(j - RadiusV) / (2 * Square(sigmaY))));
			sum += gValue;
			Kernel.push_back(gValue);
		}
	}
	std::transform(Kernel.begin(), Kernel.end(), Kernel.begin(), [&](float c) {return c / sum; });
}

//GaussianKernel::GaussianKernel(byte size, float sigma) :GaussianKernel(size, size, sigma, sigma) {}



LinearMotionBlurKernel::LinearMotionBlurKernel(byte size, float angle)
{
	VerticalSize = size;
	HorizontalSize = size;
	Kernel.resize(size*size);
	RadiusH = (HorizontalSize - 1) / 2;
	RadiusV = (VerticalSize - 1) / 2;
	CalculateKernel(size, angle);
}


void LinearMotionBlurKernel::CalculateKernel(byte size, float angle)
{
	// From the implementation of fspecial in Matlab
	//https://www.mathworks.com/help/images/ref/fspecial.html

	float phi = DegreeToRadian(angle);
	float cosphi = cos(phi);
	float sinphi = sin(phi);
	int xsign = cosphi >= 0 ? 1 : -1;
	byte linewdt = 1;
	auto sx = (int)(RadiusH*cosphi + linewdt * xsign);
	auto sy = (int)(RadiusH*sinphi + linewdt);
}

//MedianKernel::MedianKernel(byte size = 3) :MedianKernel(size, size) {}

MedianKernel::MedianKernel(byte sizeX, byte sizeY)
{
	VerticalSize = sizeY;
	HorizontalSize = sizeX;
	
	RadiusH = (HorizontalSize - 1) / 2;
	RadiusV = (VerticalSize - 1) / 2;
}

LaplacianKernel::LaplacianKernel(float alpha)
{
	Kernel.resize(9);
	VerticalSize = HorizontalSize = 3;
	RadiusH = RadiusV = 1;
	Kernel[5] = -4 / (1 + alpha);
	Kernel[0] = Kernel[2] = Kernel[6] = Kernel[8] = alpha / (1 + alpha);
	Kernel[1] = Kernel[3] = Kernel[5] = Kernel[7] = (1 - alpha) / (1 + alpha);
}

LaplacianOfGaussianKernel::LaplacianOfGaussianKernel(int N, float sigma)
{
	float sum = 0;
	Kernel.resize(N*N);
	VerticalSize = HorizontalSize = N;
	for (size_t column = 0; column < N; column++)
	{
		for (size_t row = 0; row < N; row++)
		{
			float k = Square(row - RadiusH) + Square(column - RadiusV) / (2 * sigma*sigma);
			Kernel[row + column * N] = -exp(-k)*(1-k);
			sum += Kernel[row + column * N];
		}
	}
	std::transform(Kernel.begin(), Kernel.end(), Kernel.begin(), [&](float v) {return v / sum; });
}

LaplacianOfGaussianKernel::LaplacianOfGaussianKernel(float sigma)
{
}

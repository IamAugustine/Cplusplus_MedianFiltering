#include "pch.h"
#include "Kernel.h"
#include <math.h>
constexpr auto PI = 3.14159265358979323846;
#define Square(x) (x)*(x)
template<typename T> inline float DegreeToRadian(T t) {return ((int)t % 180) * PI / 180; };

FilterKernel::FilterKernel()
{
}

FilterKernel::~FilterKernel()
{
	std::vector<float>().swap(Kernel);
}

MeanKernel::MeanKernel()
{
}

MeanKernel::MeanKernel(byte kernelSize)
{
	MeanKernel(kernelSize, kernelSize);
	

}

MeanKernel::MeanKernel(byte verticalSize, byte horizontalSize)
{
	VerticalSize = verticalSize;
	HorizontalSize = horizontalSize;
	Kernel.resize(verticalSize*horizontalSize);
	RadiusH = (HorizontalSize - 1) / 2;
	RadiusV = (VerticalSize - 1) / 2;
	std::fill(Kernel.begin(), Kernel.end(), 1.0f / verticalSize * horizontalSize);
}

//MeanKernel::~MeanKernel()
//{
//	std::vector<float>().swap(Kernel);
//}

GaussianKernel::GaussianKernel(byte sizeX, byte sizeY, float sigmaX, float sigmaY):sigmaX(sigmaX), sigmaY(sigmaY)
{
	VerticalSize = sizeY;
	HorizontalSize = sizeX;
	Kernel.resize(sizeX*sizeY);
	RadiusH = (HorizontalSize - 1) / 2;
	RadiusV = (VerticalSize - 1) / 2;
	float sum = 0;
	float constItem = 1 / (2 * PI*sigmaX*sigmaY);
	for (int i = 0; i <= HorizontalSize; i++)
	{
		for (int j = 0; j <= VerticalSize; j++)
		{
			float gValue = constItem * exp(-1 * (Square(i - RadiusH) / (2 * Square(sigmaX)) + Square(j - RadiusV) / (2 * Square(sigmaY))));
			sum += gValue;
			Kernel.push_back(gValue);
		}
	}
	std::transform(Kernel.begin(), Kernel.end(), Kernel.begin(), [&](float c) {return c / sum; });
}

GaussianKernel::GaussianKernel(byte size, float sigma)
{
	GaussianKernel(size, size, sigma, sigma);
}

//GaussianKernel::~GaussianKernel()
//{
//	std::vector<float>().swap(Kernel);
//}


LinearMotionBlurKernel::LinearMotionBlurKernel(byte size, float angle)
{
	VerticalSize = size;
	HorizontalSize = size;
	Kernel.resize(size*size);
	RadiusH = (HorizontalSize - 1) / 2;
	RadiusV = (VerticalSize - 1) / 2;
	CalculateKernel(size, angle);
}

//LinearMotionBlurKernel::~LinearMotionBlurKernel()
//{
//}

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

MedianKernel::MedianKernel(byte size = 3)
{
	MedianKernel(size, size);
}

MedianKernel::MedianKernel(byte sizeX, byte sizeY)
{
	VerticalSize = sizeY;
	HorizontalSize = sizeX;
	
	RadiusH = (HorizontalSize - 1) / 2;
	RadiusV = (VerticalSize - 1) / 2;
}

LaplacianKernel::LaplacianKernel(float alpha)
{
}

//LaplacianKernel::~LaplacianKernel()
//{
//}

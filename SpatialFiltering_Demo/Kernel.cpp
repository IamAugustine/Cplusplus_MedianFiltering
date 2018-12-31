#include "pch.h"
#include "Kernel.h"
#include <math.h>
#define PI 3.14159265358979323846
#define Square(x) (x)*(x)
FilterKernel::FilterKernel()
{
}

FilterKernel::~FilterKernel()
{
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
	this->Kernel.resize(verticalSize*horizontalSize);
	std::fill(Kernel.begin(), Kernel.end(), 1.0f / verticalSize * horizontalSize);
}

MeanKernel::~MeanKernel()
{
	std::vector<float>().swap(Kernel);
}

GaussianKernel::GaussianKernel(byte sizeX, byte sizeY, float sigmaX, float sigmaY):sigmaX(sigmaX), sigmaY(sigmaY)
{
	VerticalSize = sizeY;
	HorizontalSize = sizeX;
	Kernel.resize(sizeX*sizeY);
	int radiusX = (HorizontalSize - 1) / 2;
	int radiusY = (VerticalSize - 1) / 2;
	float sum = 0;
	float constItem = 1 / (2 * PI*sigmaX*sigmaY);
	for (int i = 0; i <= HorizontalSize; i++)
	{
		for (int j = 0; j <= VerticalSize; j++)
		{
			float gValue = constItem * exp(-1 * (Square(i - radiusX) / (2 * Square(sigmaX)) + Square(j - radiusY) / (2 * Square(sigmaY))));
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

GaussianKernel::~GaussianKernel()
{
	std::vector<float>().swap(Kernel);
}

MotionBlur::MotionBlur(byte size)
{
}

MotionBlur::MotionBlur(byte size, float angle)
{
}

MotionBlur::~MotionBlur()
{
}

MedianKernel::MedianKernel(byte size = 3)
{
	MedianKernel(size, size);
}

MedianKernel::MedianKernel(byte sizeX, byte sizeY)
{
	VerticalSize = sizeY;
	HorizontalSize = sizeX;
	BoundaryH = (HorizontalSize - 1) / 2;
	BoundaryV = (VerticalSize - 1) / 2;
}

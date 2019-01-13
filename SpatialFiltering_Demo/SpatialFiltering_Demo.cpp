// SpatialFiltering_Demo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "MedianFilter2D.h"
#include "IOHelper.h"
#include "MeanFilter2D.h"
#include "GaussianFilter2D.h"
#include "FastGaussianFilter2D.h"
#include "Laplacian2D.h"
#include "Sobel.h"
#include "BilateralFilter2D.h"
#include "FilterBase.h"
#include "LinearMotionBlur2D.h"
enum FilterType
{
	Mean,
	Median,
	Gaussian, 
	Biliteral,
	Laplacian,
	FastGaussian,
	LaplacianOfGaussian,
	Sobel,
	LinearMotion,
	Custom,
};
int main()
{
    std::cout << "Hello World!\n"; 
	const char* filename = "simu_image.txt";
	const char* saveFileName = "median_filtered_image.txt";
	int height = 200;
	int width = 200;
	byte kernelSize = 5;
	static ushort* imgData = IOHelper::ReadLocalFile<ushort>(filename, height, width);
	static ushort* fltdImgData = new ushort[height *width];
	
	//auto mdfilter = new MedianFilter2D(kernelSize);
	FilterType filterType = FilterType::Laplacian;
	Filter2D* filterToApply;
	switch (filterType)
	{
	case Mean:
		filterToApply = new MeanFilter2D(kernelSize);
		break;
	case Median:
		filterToApply = new MedianFilter2D(kernelSize);
		break;
	case Gaussian:
	{
		float sigma = 3;
		filterToApply = new GaussianFilter2D(kernelSize, sigma);
	}
		break;
	case Biliteral:
	{
		float sigmaS = 3;
		float sigmaP = 5;
		filterToApply = new BilateralFilter2D(sigmaS, kernelSize,sigmaP);
	}
		break;
	case Laplacian:
	{
		float alpha = 0;
		filterToApply = new Laplacian2D(alpha);
	}
		break;
	case LaplacianOfGaussian:
	{
		filterToApply = nullptr;
	}
		break;
	case Sobel:
	{
		Direction direction = Direction::Both;
		filterToApply = new Sobel2D(direction);
	}
		break;
	case LinearMotion:
	{
		byte size = 9;
		float angle = 45;
		filterToApply = new LinearMotionBlur2D(size, angle);
	}
		break;
	case Custom:
		filterToApply = new Filter2D();
	case FastGaussian:
	{
		float sigma = 3;
		filterToApply = new FastGaussianFilter2D(sigma);
	}
		break;
	default:
	{
		float sigma = 3;
		filterToApply = new GaussianFilter2D(sigma);
	}
		break;
	}

	filterToApply->Apply(imgData,height,width, fltdImgData);
	IOHelper::SaveToLocalFile(saveFileName, fltdImgData, height, width);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

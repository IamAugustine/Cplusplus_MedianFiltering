// FastMedianFiltering.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "IOHelper.h";
#include "MedianFilter.h";

int main()
{
    //std::cout << "Hello World!\n"; 
	const char* filename = "simu_image.txt";
	const char* saveFileName = "median_filtered_image.txt";
	int height = 200;
	int width = 200;
	int kernelSize = 7;
	static int* imgData = IOHelper::ReadLocalFile(filename, height, width);
	static int* fltdImgData = new int[height *width];
	MedianFilter::Apply(imgData, fltdImgData, height, width, kernelSize);
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

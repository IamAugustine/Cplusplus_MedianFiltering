#pragma once
#include <fstream>;
#include <iostream>;
using namespace std;

class IOHelper
{
public:
	IOHelper();
	~IOHelper();
public:
	static int *ReadLocalFile(const char* filename, int height, int width)
	{
		fstream fs;
		fs.open(filename);
		if (fs.is_open())
		{
			int* data = new int[height * width];
			int i = 0;
			while (!fs.eof())
			{
				fs >> data[i];
				i++;
			}
			fs.close();
			return data;
		}
		else
		{
			return nullptr;
		}
		
	}
	static void SaveToLocalFile(const char* filename, int* data, int height, int width)
	{
		ofstream fs;
		fs.open(filename);
		if (fs.is_open())
		{
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					fs << data[i*width + j] << "\t";
				}
				fs << endl;
			}
		}
		fs.close();
	}
};


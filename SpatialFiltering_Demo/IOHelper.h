#pragma once
#include <fstream>
using namespace std;

class IOHelper
{
public:
	IOHelper();
	~IOHelper();
public:
	template<typename T>static T *ReadLocalFile(const char* filename, int height, int width)
	{
		fstream fs;
		fs.open(filename);
		if (fs.is_open())
		{
			T* data = new T[height * width];
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
	template<typename T> static void SaveToLocalFile(const char* filename, T* data, int height, int width)
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


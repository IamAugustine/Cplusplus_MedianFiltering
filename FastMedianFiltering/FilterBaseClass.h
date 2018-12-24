#pragma once
class FilterBaseClass
{
public:
	FilterBaseClass();
	~FilterBaseClass();
public: 
	int KernelSize;
	int ImgaeHeight;
	int ImageWidth;
public:
	static int GetCPUCoreNumber();
public:
	virtual void Apply(int* imageIn, int* imageOut) = 0;
};


#pragma once
#include "FilterBaseClass.h"
class BilateralFilter :
	public FilterBaseClass
{
public:
	BilateralFilter();
	~BilateralFilter();
public:
	void Apply(int* imgIn, int* imgOut);
};


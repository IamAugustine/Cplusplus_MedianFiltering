#pragma once
#include "FilterBase.h"
class BilateralFilter2D :
	public FilterBase
{
public:
	BilateralFilter2D();
	~BilateralFilter2D();
protected:
	float sigma;
	float mu;

};


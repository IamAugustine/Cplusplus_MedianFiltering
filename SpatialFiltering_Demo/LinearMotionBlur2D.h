#pragma once
#include "FilterBase.h"
class LinearMotionBlur2D :
	public FilterBase
{
public:
	LinearMotionBlur2D(unsigned char distance, float angle);
	~LinearMotionBlur2D();
};


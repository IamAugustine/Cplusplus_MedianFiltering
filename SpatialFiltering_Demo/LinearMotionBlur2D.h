#pragma once
#include "FilterBase.h"
class LinearMotionBlur2D :
	public Filter2D
{
public:
	LinearMotionBlur2D(unsigned char distance, float angle);
	~LinearMotionBlur2D();
};


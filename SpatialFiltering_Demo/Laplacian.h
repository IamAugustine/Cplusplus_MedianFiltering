#pragma once
#include "FilterBase.h"
class Laplacian2D :
	public Filter2D
{
public:
	Laplacian2D(float alpha);
	~Laplacian2D();
};


#pragma once
#include "FilterBase.h"
class Laplacian :
	public Filter2D
{
public:
	Laplacian(float alpha);
	~Laplacian();
};


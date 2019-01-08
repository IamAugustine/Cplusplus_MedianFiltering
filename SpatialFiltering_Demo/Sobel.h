#pragma once
#include "FilterBase.h"

	enum Direction
	{
		Vertical,
		Horizontal,
		Both
	};
class Sobel2D :
	public Filter2D
{

public:
	Sobel2D(Direction direction);
	~Sobel2D();

};


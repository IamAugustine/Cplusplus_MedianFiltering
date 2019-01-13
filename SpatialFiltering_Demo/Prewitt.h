#pragma once
#include "FilterBase.h"
class Prewitt :
	public Filter2D
{
public:
	Prewitt(Direction direction);
	~Prewitt();
public:
	ushort Calculate(deque<ushort> data);
	Direction DerivativeDirection;
};


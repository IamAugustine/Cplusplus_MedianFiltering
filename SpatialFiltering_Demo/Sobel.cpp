#include "pch.h"
#include "Sobel.h"




Sobel2D::Sobel2D(Direction direction):DerivativeDirection(direction)
{
	Kernel = new FilterKernel(3);
}

Sobel2D::~Sobel2D()
{
}

ushort Sobel2D::Calculate(deque<ushort> data)
{
	switch (DerivativeDirection)
	{
	case Vertical:
		return data[0] + 2 * data[3] + data[6] - data[2] - 2 * data[5] - data[8];
		break;
	case Horizontal:
		return data[0] + 2 * data[1] + data[2] - data[6] - 2 * data[7] - data[8];
		break;
	case Both:
	{
		ushort dv = data[0] + 2 * data[3] + data[6] - data[2] - 2 * data[5] - data[8];
		ushort dh = data[0] + 2 * data[1] + data[2] - data[6] - 2 * data[7] - data[8];
		return ushort(sqrt(dh*dh + dv * dv) + 0.5);
	}
		break;
	default:
	{
		ushort dv = data[0] + 2 * data[3] + data[6] - data[2] - 2 * data[5] - data[8];
		ushort dh = data[0] + 2 * data[1] + data[2] - data[6] - 2 * data[7] - data[8];
		return ushort(sqrt(dh*dh + dv * dv) + 0.5);
	}
		break;
	}
}

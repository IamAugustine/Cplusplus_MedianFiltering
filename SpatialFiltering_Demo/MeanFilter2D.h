#pragma once
#include "FilterBase.h"
class MeanFilter2D :
	public FilterBase
{
public:
	MeanFilter2D();
	MeanFilter2D(byte size);
	MeanFilter2D(byte sizeX, byte sizeY);
	~MeanFilter2D();
	void Apply(ushort* imageIn, int height, int width, ushort* imageOut);
	void FilterBlock(ushort* imageIn, ushort* imageOut);
};


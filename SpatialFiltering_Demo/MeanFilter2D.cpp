#include "pch.h"
#include "MeanFilter2D.h"


MeanFilter2D::MeanFilter2D()
{
}

MeanFilter2D::MeanFilter2D(byte size)
{
	Kernel = new MeanKernel(size);
}

MeanFilter2D::MeanFilter2D(byte sizeX, byte sizeY)
{
	Kernel = new MeanKernel(sizeY, sizeX);
}


MeanFilter2D::~MeanFilter2D()
{
	
}

void MeanFilter2D::Apply(ushort * imageIn, int height, int width, ushort * imageOut)
{

}

void MeanFilter2D::FilterBlock(ushort * imageIn, ushort * imageOut)
{
}

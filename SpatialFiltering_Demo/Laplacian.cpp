#include "pch.h"
#include "Laplacian2D.h"



Laplacian2D::Laplacian2D(float alpha)
{
	Kernel = new LaplacianKernel(alpha);
}

Laplacian2D::~Laplacian2D()
{
}

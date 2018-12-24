#include "pch.h"
#include "FilterBaseClass.h"
#include <Windows.h>

FilterBaseClass::FilterBaseClass()
{
}


FilterBaseClass::~FilterBaseClass()
{
}

int FilterBaseClass::GetCPUCoreNumber()
{
	SYSTEM_INFO si;

	GetSystemInfo(&si);

	return si.dwNumberOfProcessors;
}

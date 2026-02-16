#pragma once
#include "ExPort.h"

extern "C"
{
	DLL_EXPORTS  void Walk(unsigned int renderHostId, float d);
	DLL_EXPORTS void Strafe(unsigned int renderHostId, float d);
	DLL_EXPORTS void Pitch(unsigned int renderHostId, float d);
	DLL_EXPORTS void RotateY(unsigned int renderHostId, float d);
}

#pragma once
#include"pch.h"
#include"GraphicsException.h"
#ifndef ThrowGraphicsfFailed
#define ThrowGraphicsfFailed(x)                                              \
{                                                                     \
    HRESULT hr__ = (x);       \
if (FAILED(hr__)) { throw GraphicsException(hr__, #x, __FILE__, __LINE__); } \
}
#endif
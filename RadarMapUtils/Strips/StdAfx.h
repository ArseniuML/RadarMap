// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

#include <Stdlib.h>
#include <Stdio.h>
#include <String.h>

#include "Strips.h"

inline void ZeroMemory(void* addr, udword size)
{
	memset(addr, 0, size);
}

inline void CopyMemory(void* dest, const void* src, udword size)
{
	memcpy(dest, src, size);
}

inline void FillMemory(void* dest, udword size, ubyte val)
{
	memset(dest, val, size);
}
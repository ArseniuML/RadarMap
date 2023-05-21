#pragma once

#include "SimpleTypes.h"

namespace MathUtils
{
	void LagrangeInterpPoly
	(
		int length,
		double* pos,
		DecartVector* val, 
		double desiredPos,
		DecartVector* res
	);
};


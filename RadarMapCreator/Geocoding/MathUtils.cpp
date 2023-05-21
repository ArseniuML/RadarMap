#include "StdAfx.h"
#include "MathUtils.h"

void MathUtils::LagrangeInterpPoly
(
	int length,
	double* pos,
	DecartVector* val, 
	double desiredPos,
	DecartVector* res
)
{
	res->X = 0;
	res->Y = 0;
	res->Z = 0;

	for (int i = 0; i < length; ++i) {
		double weight = 1;
		for (int j = 0; j < length; ++j) {
			if (j != i) {
				weight *= (desiredPos - pos[j]) / (pos[i] - pos[j]);
			}
		}
		res->X += weight * val[i].X;
		res->Y += weight * val[i].Y;
		res->Z += weight * val[i].Z;
	}
}
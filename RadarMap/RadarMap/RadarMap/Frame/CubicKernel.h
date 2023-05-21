#pragma once
#include <RLib/Collection/Texture.h>
#include <RLib/Collection/Array2D.h>
#include <RLib/Shader/Program.h>

namespace RadarMap {
	namespace Frame {
	
//ядро кубической интерпол€ции
//см. "GPU Gems", глава 24

class CubicKernel : public RLib::Collection::TextureFloatRGBA
{
public:
	CubicKernel(int size, float B = 1.f/3.f, float C = 1.f/3.f)
	{
		RLib::Collection::Array2D<float, 4> arr(size, 1);
		
		for(int i = 0; i < size; i++) 
		{
			float x = i / (float) (size - 1);
			arr(i, 0)[0] = MitchellNetravali(x + 1, B, C);
			arr(i, 0)[1] = MitchellNetravali(x, B, C);
			arr(i, 0)[2] = MitchellNetravali(1 - x, B, C);
			arr(i, 0)[3] = MitchellNetravali(2 - x, B, C);
		}
		
		Filter(GL_LINEAR, GL_LINEAR);
		Wrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		Resize(size, 1);
		FromArray(&arr(0, 0)[0]);
	}

private:
	// Mitchell Netravali Reconstruction Filter
	// B = 1,   C = 0   - cubic B-spline
	// B = 1/3, C = 1/3 - recommended
	// B = 0,   C = 1/2 - Catmull-Rom spline

	float MitchellNetravali(float x, float B, float C)
	{
		float ax = fabs(x);
		if (ax < 1) 
		{
			return ((12 - 9 * B - 6 * C) * ax * ax * ax +
				(-18 + 12 * B + 6 * C) * ax * ax + (6 - 2 * B)) / 6;
		} 
		else if ((ax >= 1) && (ax < 2)) 
		{
			return ((-B - 6 * C) * ax * ax * ax +
				(6 * B + 30 * C) * ax * ax + (-12 * B - 48 * C) *
				ax + (8 * B + 24 * C)) / 6;
		};
		return 0;
	}
};

	}
}

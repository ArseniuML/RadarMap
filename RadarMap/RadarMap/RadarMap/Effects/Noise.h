#pragma once
#include <RLib/Collection/Texture.h>
#include <RLib/Collection/Array2D.h>
#include <RLib/Shader/Program.h>

namespace RadarMap {
	namespace Effects {
	
// ласс служит дл€ генерации текстуры шума с необходимыми веро€тностными характеристиками
//»митаци€ измен€ющегос€ шума создаетс€ при помощи случайных перестановок элементов шумовой текстуры

class Noise
{
public:
	Noise();

	RLib::Collection::Texture& GetNoiseTexture()	{return mNoiseTexture;}
	RLib::Collection::Texture& GetAddNoiseTexture()	{return mAddNoiseTexture;}
	RLib::Collection::Texture& GetPermTexture() {return mPermTexture;}

	void Resize(int width, int height);
	void Permute();

private:
	void rand_perm(float* p, int N);

private:

	RLib::Collection::Array2D<float, 1> mNoiseArray;
	RLib::Collection::TextureFloatR mNoiseTexture;
	RLib::Collection::TextureFloatR mAddNoiseTexture;

	RLib::Collection::TextureFloatR mPermTexture;
	RLib::Collection::Array2D<float, 1> mPermArray;
};

	}
}

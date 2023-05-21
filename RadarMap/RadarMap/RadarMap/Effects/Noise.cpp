#include "stdafx.h"
#include "Noise.h"

typedef unsigned int uint;

extern "C"	void CPU_mrg32k3a_init(uint *v1, uint *v2, int offset);
extern "C"	void CPU_mrg32k3a_gamma(int N, float alpha, float *P);
extern "C"	void CPU_mrg32k3a_normal(int N, float *P);

RadarMap::Effects::Noise::Noise()
{
	uint v1[3] = {7845, 314, 8947};
	uint v2[3] = {92487, 7645, 56348};
	CPU_mrg32k3a_init(v1, v2, 2);
}

void RadarMap::Effects::Noise::Resize( int width, int height )
{
	if (width != mNoiseTexture.Width() || height != mNoiseTexture.Height())
	{
		mNoiseTexture.Resize(width, height);
		mAddNoiseTexture.Resize(width, height);
		mNoiseArray.Resize(width, height);

		CPU_mrg32k3a_gamma(mNoiseArray.Size(), techConfig.Looks, &mNoiseArray(0)[0]);
		mNoiseTexture.FromArray(&mNoiseArray(0)[0]);

		CPU_mrg32k3a_normal(mNoiseArray.Size(), &mNoiseArray(0)[0]);
		mAddNoiseTexture.FromArray(&mNoiseArray(0)[0]);

		mPermArray.Resize(std::max(width, height), 2);
		mPermTexture.Resize(mPermArray.Width(), mPermArray.Height());

		for (int j = 0, n = 0; j < mPermArray.Height(); ++j)
		{
			for (int i = 0; i < mPermArray.Width(); ++i, ++n)
			{
				mPermArray(n)[0] = i;
			}
		}
		mPermTexture.FromArray(&mPermArray(0, 0)[0]);
	}
}

void RadarMap::Effects::Noise::Permute()
{
	rand_perm(reinterpret_cast<float*>(&mPermArray(0, 0)[0]), mNoiseTexture.Width());
	rand_perm(reinterpret_cast<float*>(&mPermArray(0, 1)[0]), mNoiseTexture.Height());
	mPermTexture.FromArray(&mPermArray(0, 0)[0]);
}

void RadarMap::Effects::Noise::rand_perm( float* p, int N )
{
	for (int i = 0; i < N; ++i) 
	{
		int j = rand() % N;
		std::swap(p[i], p[j]);
	}
}

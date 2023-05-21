#pragma once

class FwdLUT
{
	struct FwdLink
	{
		int Index;
		float Weight;
	};
	
public:
	FwdLUT()
	{
		mMapSize = 0;
		mKernelSize = 0;
	}

	void Resize(int MapSize, int KernelSize)
	{
		mMapSize = MapSize;
		mKernelSize = KernelSize;
		mLUT.resize(mMapSize * mKernelSize);
	}

	int MapSize() const 
	{
		return mMapSize;
	}

	int KernelSize() const 
	{
		return mKernelSize;
	}

	FwdLink& operator() (int ind)
	{
		return mLUT[ind];
	}

	const FwdLink& operator() (int ind) const
	{
		return mLUT[ind];
	}

private:
	std::vector<FwdLink> mLUT;
	
	int mMapSize;
	int mKernelSize;
};

class BwdLUT
{
public:
	BwdLUT() {}

	void SetImageSize(int imageSize)
	{
		mItemCount.resize(imageSize + 1, 0);
	}

	int ImageSize() const
	{
		return mItemCount.size() - 1;
	}

	void IncreaseItemSize(int nImage)
	{
		++mItemCount[nImage];
	}

	int ItemSize(int nImage) const
	{
		return mItemCount[nImage];
	}	
	
	int operator() (int nImage, int nEntry) const
	{
		return mLUT[mIndex[nImage] + nEntry];
	}	
	
	void Prepare()
	{
		mIndex.resize(mItemCount.size(), 0);
		for (int n = 1; n < mIndex.size(); ++n)
		{
			mIndex[n] += mIndex[n - 1] + mItemCount[n - 1];
		} 
		mLUT.resize(mIndex[mIndex.size() - 1]);
		mItemCount.assign(mItemCount.size(), 0);
	}

	void Fill(int nImage, int data)
	{
		mLUT[mIndex[nImage] + mItemCount[nImage]] = data;
		++mItemCount[nImage];
	}

private:
	std::vector<int> mLUT;
	std::vector<int> mIndex;
	std::vector<int> mItemCount;
};
#pragma once

namespace RLib {
	namespace Sort {

template<class T> 
struct Greater
{
	bool operator() (const T& t1, const T& t2) 
	{
		return t1 < t2;
	}
};

typedef unsigned int Index;

class MergeSort
{
public:
	MergeSort(int maxSize):
	mMaxSize(maxSize)
	{
		int halfSize = (mMaxSize / 2) + 1; 
		mLeftHalf	= (int*)malloc(halfSize * sizeof(Index));
		mRightHalf = (int*)malloc(halfSize * sizeof(Index));
	}
	
	~MergeSort()
	{
		free(mLeftHalf);
		free(mRightHalf);
	}
	
	template<class Comparator, class T>
	void Sort(const T* A, Index* ind, Index firstInd, Index lastInd, Index offset = 0, Index stride = 1)
	{
		if (!(firstInd < lastInd && lastInd - firstInd + 1 <= mMaxSize)) return;
		int halfInd = (firstInd + lastInd) / 2;

		Sort<Comparator>(A, ind, firstInd, halfInd, offset, stride);
		Sort<Comparator>(A, ind, halfInd + 1, lastInd, offset, stride);

		int n = halfInd- firstInd + 1;
		int m = lastInd- halfInd;

		for (int i = 0; i < n; ++i)
		{
			mLeftHalf[i] = ind[firstInd+i];
		}
		for (int i = 0; i < m; ++i)
		{
			mRightHalf[i] = ind[halfInd+i+1];
		}

		for (int i = 0, j = 0, k = firstInd; k <= lastInd; ++k)
		{
			if (i == n)
			{
				ind[k] = mRightHalf[j++];
			}
			else if (j == m)
			{
				ind[k] = mLeftHalf[i++];
			}
			else if (Comparator()(A[offset + stride * mLeftHalf[i]], A[offset + stride * mRightHalf[j]]))
			{
				ind[k] = mLeftHalf[i++];
			}
			else
			{
				ind[k] = mRightHalf[j++];
			}
		}
	}

protected:
	int *mLeftHalf, *mRightHalf;
	int mMaxSize;
};

template<class Sorter, class Space, class Dimension, class Action>
static void SpatialSort(Sorter& sorter, const Space* pValue,  Index* pIndices, Index firstInd, Index lastInd,  Action action, Dimension bias, bool mergeOne = false, int nDimension = 0)
{
	sorter.Sort<Greater<Dimension>>(reinterpret_cast<const Dimension*>(pValue), pIndices, firstInd, lastInd - 1, nDimension, sizeof(Space) / sizeof(Dimension));

	for (int firstIndN = firstInd, lastIndN = firstIndN + 1; lastIndN <= lastInd; ++lastIndN)
	{
		if (lastIndN < lastInd)
		{
			const Dimension* d1 = reinterpret_cast<const Dimension*>(&pValue[pIndices[firstIndN]])  + nDimension;
			const Dimension* d2 = reinterpret_cast<const Dimension*>(&pValue[pIndices[lastIndN]]) + nDimension;
			if (abs(*d1 - *d2) <= bias) continue;
		}

		if (lastIndN - firstIndN > 1)
		{
			if (nDimension < sizeof(Space) / sizeof(Dimension) - 1)
			{
				SpatialSort(sorter, pValue, pIndices, firstIndN, lastIndN, action, bias, mergeOne, nDimension + 1);
			}
			else
			{
				action(firstIndN, lastIndN);
			}
		}
		else if (mergeOne)
		{
			action(firstIndN, lastIndN);
		}

		firstIndN = lastIndN;
	}
}

	}
}
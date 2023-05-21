#pragma once

namespace RLib {
	namespace Collection {

template<class T, unsigned int N>
class ArrayItem
{
public:
	T& operator[](int index) {return mData[index];}
	T operator[](int index) const	{return mData[index];}
	
	unsigned int Size() {return N;}

private:
	T mData[N];
};

template <class T, unsigned int N>
class Array2D
{
public:
	Array2D (int width = 0, int height = 0)
	{
		Resize(width, height);
	}
	
	void Resize(int width, int height)
	{
		mWidth = width;
		mHeight = height;
		mData.resize(mWidth * mHeight);
	}

	int Width()	 {return mWidth;	}
	int Height() {return mHeight;}
	
	int Size() {return mWidth * mHeight;}
	int ByteSize() {return mWidth * mHeight * N * sizeof(T);}

	ArrayItem<T, N>& operator () (int x, int y) {return mData[y * mWidth + x];}
	const ArrayItem<T, N>& operator () (int x, int y) const {return mData[y * mWidth + x];}

	ArrayItem<T, N>& operator () (int i) {return mData[i];}
	const ArrayItem<T, N>& operator () (int i) const {return mData[i];}

protected:
	int mWidth, mHeight;
	std::vector<ArrayItem<T, N>> mData;
};

	}
}
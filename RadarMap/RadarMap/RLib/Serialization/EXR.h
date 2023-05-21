#include <RLib/Collection/Array2D.h>

namespace RLib {
	namespace Serialization {

class EXR 
{
public:
	static const char* ChannelName(unsigned int n)
	{
		switch (n)
		{
		case 0 : 
			return "R";
		case 1 : 
			return "G";
		case 2 : 
			return "B";
		case 3 : 
			return "A";

		default : 
			return 0;
		}
	}

	template<Imf::PixelType PT, class T, unsigned int N> static void LoadEXR(Collection::Array2D<T, N>& array2D, const char* fileName)
	{
		Imf::InputFile file(fileName);
		Imath::Box2i dw = file.header().dataWindow();

		int width = dw.max.x - dw.min.x + 1;
		int height = dw.max.y - dw.min.y + 1;
		int dx = dw.min.x;
		int dy = dw.min.y;

		array2D.Resize(width, height);

		Imf::FrameBuffer frameBuffer;

		for(int nc = 0; nc < N; ++nc)
		{
			frameBuffer.insert
			(
				ChannelName(nc),
				Imf::Slice
				(
					PT, 
					(char*)(&array2D(-dx, -dy)[nc]),
					N * sizeof(T),
					N * sizeof(T) * array2D.Width()
				)
			);
		}

		file.setFrameBuffer(frameBuffer);
		file.readPixels (dw.min.y, dw.max.y);
	}

	template<Imf::PixelType PT, class T, unsigned int N> static void SaveEXR(Collection::Array2D<T, N>& array2D, const char* fileName)
	{
		Imf::Header header(array2D.Width(), array2D.Height()); 
		Imf::FrameBuffer frameBuffer;

		for(int nc = 0; nc < N; ++nc)
		{
			const char* channelName = ChannelName(nc);
			header.channels().insert (channelName, Imf::Channel(PT)); 
			frameBuffer.insert
			(
				channelName,
				Imf::Slice
				(
					PT, 
					(char*)(&array2D(0, 0)[nc]),
					N * sizeof(T),
					N * sizeof(T) * array2D.Width()
				)
			);
		}

		Imf::OutputFile file(fileName, header);
		file.setFrameBuffer(frameBuffer);
		file.writePixels(array2D.Height());
	}
		
};
	
	}
}


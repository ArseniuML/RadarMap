#pragma once

namespace RadarMap
{
	//Структура определяет формат хранения параметров кадра РЛ карты
	//в communication server - е

	struct RequestData
	{
		struct
		{
			double	X, Y, Z;
			float	Roll, Pitch, Heading;
			float	Height;
		} Airplane;

		int Mode;
		
		float	DMin, DMax;
		float	AzimuthLeft, AzimuthRight;

		float  DCenter;
		float  MapWidth;
		float  MapHeight; 
	};

}
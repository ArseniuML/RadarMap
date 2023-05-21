#pragma once

namespace RadarMap
{
	//��������� ���������� ������ �������� ���������� ����� �� �����
	//� communication server - �

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
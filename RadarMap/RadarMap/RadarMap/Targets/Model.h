#pragma once

#include <RadarMap/Request.h>
#include <RadarMap/Geometry/GeometricObject.h>
#include <RadarMap/Geometry/RenderProgram.h>
#include "Point/RenderProgram.h"

namespace RadarMap {
	namespace Targets  {

//����� ������������ ���������� ������ ���� (��� ����) 
//���� � �� �� ������ (���) ���� ����� �������������� �� �� ����� 
//� ���������� ����������� � ���������� ������������ (Location)

class Model : public Geometry::GeometricObject	
{
public:
	Model(const char* fileName, float brightness);
	~Model();

	void AddLocation(double X, double Y, double Z, float Teta, float Psi, float Gamma);
	void ClearLocations();
	
	void Render(Geometry::RenderProgram& geometryRenderProgram, const Request& req);
	void RenderPoint(Point::RenderProgram& pointRenderProgram);

private:

	struct Location
	{
		double  X;
		double  Y;
		double  Z;

		float  Teta;
		float  Psi;
		float  Gamma;
	};
	std::vector<Location>	mLocations;
	
	mgcoord2d mDiameter;
	float mBrightness;
};

	}
}

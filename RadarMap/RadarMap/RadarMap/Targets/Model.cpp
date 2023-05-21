#include "stdafx.h"
#include "Model.h"

#include  "../Constants.h"

RadarMap::Targets::Model::Model(const char* fileName, float brightness):
mBrightness(brightness)
{
	GeometricObject::LoadGeometry(fileName);
	
	mgboxd bbox; mgGetBounds(mDb, &bbox);
	mDiameter.x = bbox.max.x - bbox.min.x;
	mDiameter.y = bbox.max.y - bbox.min.y;
}

RadarMap::Targets::Model::~Model()
{
}

void RadarMap::Targets::Model::AddLocation(double X, double Y, double Z, float Teta, float Psi, float Gamma)
{
	Location location;
	location.X = X;
	location.Y = Y;
	location.Z = Z;
	location.Teta = Teta;
	location.Psi = Psi;
	location.Gamma = Gamma;

	mLocations.push_back(location);
}

void RadarMap::Targets::Model::ClearLocations()
{
	mLocations.clear();
}

void RadarMap::Targets::Model::Render(Geometry::RenderProgram& geometryRenderProgram, const Request& req )
{
	BOOST_FOREACH(Location& location, mLocations )
	{
		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();

		glTranslatef(location.X,location.Y,location.Z);
		glRotatef(-location.Psi * 180.0f / PI, 0, 0, 1);
		glRotatef(location.Gamma * 180.0f / PI, 1, 0, 0);
		glRotatef(location.Teta * 180.0f / PI, 0, 1, 0);
			
		float dTgt = sqrtf((req.Airplane.X - location.X) *  (req.Airplane.X - location.X) +
						(req.Airplane.Y - location.Y) *  (req.Airplane.Y - location.Y) +
						(req.Airplane.Z - location.Z) *  (req.Airplane.Z - location.Z));
		
		//Антиалиасинг
		float pixelSizeAzimuth = 2.0f * req.FOV.AzimuthHRadius  * dTgt / (float)techConfig.Modes[req.Mode].Elements.Azimuth;
		float pixelSizeRange = (req.FOV.DistanceMax - req.FOV.DistanceMin) / (float)techConfig.Modes[req.Mode].Elements.Range;
		float pixelSize = std::max(pixelSizeAzimuth, pixelSizeRange);

		float kX = 1.15f * pixelSize / (float)mDiameter.x; kX = kX > 1.0f ? kX : 1.0f;
		float kY = 1.15f * pixelSize / (float)mDiameter.y; kY = kY > 1.0f ? kY : 1.0f;
		glScalef(kX, kY, 1);

		glUniform1fARB(geometryRenderProgram.mUniFlatBrightness, mBrightness);
		GeometricObject::Render();

		glMatrixMode( GL_MODELVIEW );
		glPopMatrix();
	}
}

void RadarMap::Targets::Model::RenderPoint( Point::RenderProgram& pointRenderProgram )
{
	//В режимах LRM и SEA1 цели рендерятся как точки (с последующим размытием)
	//Пожелание А.Андреева ("чтобы не выглядели как бомбочки")

	glPointSize(1.0f);
	glUniform1fARB(pointRenderProgram.mUniBrightness, mBrightness);
	
	glBegin(GL_POINTS);
	BOOST_FOREACH(Location& location, mLocations )
	{
		glVertex3d(location.X, location.Y, location.Z);
	}
	glEnd();
}

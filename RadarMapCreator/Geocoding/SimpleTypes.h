#pragma once

struct RadarVector
{
	double Azimuth;
	double Range;
};

struct DecartVector
{
	DecartVector() {}

	DecartVector(double X, double Y, double Z)
	{
		this->X = X;
		this->Y = Y;
		this->Z = Z;
	}

	double X;
	double Y;
	double Z;
};
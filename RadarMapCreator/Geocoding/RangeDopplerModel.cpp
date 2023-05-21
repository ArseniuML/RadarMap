#include "StdAfx.h"
#include "RangeDopplerModel.h"

#include "MathUtils.h"

const double LightSpeed = 299792458.0;
const double NonValidZeroDopplerTime = -99999.0;

RangeDopplerModel::RangeDopplerModel(const SARScapeHeader& header)
{
	mRaster.PixelsPerLine = header.RasterInfo.PixelsPerLine;
	mRaster.LinesPerImage = header.RasterInfo.LinesPerImage;

	mProcessing.SlantRangeNear = header.Processing.SlantRange[0];
	mProcessing.PixelSpacingRg = header.Processing.PixelSpacingRg;
	
	boost::posix_time::time_duration azStartDur = header.Processing.AzimuthStartTime - header.StateVectorData.TimeFirst;
	mProcessing.AzimuthStartTime = double(azStartDur.ticks()) / double(azStartDur.ticks_per_second());

	boost::posix_time::time_duration azStopDur = header.Processing.AzimuthStopTime - header.Processing.AzimuthStartTime;
	mProcessing.LineTimeInterval = (double(azStopDur.ticks()) / double(azStopDur.ticks_per_second())) / double(mRaster.LinesPerImage - 1);
	
	int size = header.StateVectorData.pos.size();
	mStateVectorData.Time.resize(size);
	for (int i = 0; i < size; ++i)
	{
		mStateVectorData.Time[i] = double(i) * header.StateVectorData.TimeDelta;
	}
	mStateVectorData.Pos = header.StateVectorData.pos;
	mStateVectorData.Vel = header.StateVectorData.vel;

	mOrbit.Pos.resize(mRaster.LinesPerImage);
	mOrbit.Vel.resize(mRaster.LinesPerImage);

	for (int i = 0; i < mRaster.LinesPerImage; ++i) {
		double time = mProcessing.AzimuthStartTime + double(i) * mProcessing.LineTimeInterval;
		MathUtils::LagrangeInterpPoly(size, &mStateVectorData.Time[0], &mStateVectorData.Pos[0], time, &mOrbit.Pos[i]);
		MathUtils::LagrangeInterpPoly(size, &mStateVectorData.Time[0], &mStateVectorData.Vel[0], time, &mOrbit.Vel[i]);
	}
}


bool RangeDopplerModel::RadarPoint(DecartVector earthPoint, DecartVector* lookVec, RadarVector* imagePoint)
{
	double zeroDopplerTime = EarthPointZeroDopplerTime(earthPoint);

	if (zeroDopplerTime == NonValidZeroDopplerTime) 
	{
		return false;
	}

	DecartVector sensorPos;
	MathUtils::LagrangeInterpPoly(mStateVectorData.Time.size(), &mStateVectorData.Time[0], &mStateVectorData.Pos[0], zeroDopplerTime, &sensorPos);
	lookVec->X = sensorPos.X - earthPoint.X;
	lookVec->Y = sensorPos.Y - earthPoint.Y;
	lookVec->Z = sensorPos.Z - earthPoint.Z;
	double slantRange = sqrt(lookVec->X * lookVec->X + lookVec->Y * lookVec->Y + lookVec->Z * lookVec->Z);
	
	zeroDopplerTime = zeroDopplerTime + slantRange / LightSpeed;
	
	MathUtils::LagrangeInterpPoly(mStateVectorData.Time.size(), &mStateVectorData.Time[0], &mStateVectorData.Pos[0], zeroDopplerTime, &sensorPos);
	lookVec->X = sensorPos.X - earthPoint.X;
	lookVec->Y = sensorPos.Y - earthPoint.Y;
	lookVec->Z = sensorPos.Z - earthPoint.Z;
	
	slantRange = sqrt(lookVec->X * lookVec->X + lookVec->Y * lookVec->Y + lookVec->Z * lookVec->Z);
	lookVec->X /= slantRange;
	lookVec->Y /= slantRange;
	lookVec->Z /= slantRange;

	imagePoint->Azimuth = (zeroDopplerTime - mProcessing.AzimuthStartTime) / mProcessing.LineTimeInterval;
	imagePoint->Range = (slantRange - mProcessing.SlantRangeNear) / mProcessing.PixelSpacingRg;
	
	return true;
}

double RangeDopplerModel::EarthPointZeroDopplerTime( DecartVector earthPoint )
{
	int lowerInd = 0;
	int upperInd = mOrbit.Pos.size() - 1;
	
	double lowerIndFreq = DopplerFrequency(lowerInd, earthPoint);
	double upperIndFreq = DopplerFrequency(upperInd, earthPoint);

	if (lowerIndFreq == 0.0) 
	{
		return mProcessing.AzimuthStartTime + lowerInd * mProcessing.LineTimeInterval;
	} 
	else if (upperIndFreq == 0.0) 
	{
		return mProcessing.AzimuthStartTime + upperInd * mProcessing.LineTimeInterval;
	} 
	else if (lowerIndFreq * upperIndFreq > 0.0) 
	{
		return NonValidZeroDopplerTime;
	}

	// start binary search
	while(upperInd - lowerInd > 1) 
	{
		int midInd = (lowerInd + upperInd) / 2;
		double midIndFreq = DopplerFrequency(midInd, earthPoint);
		
		if (midIndFreq == 0.0) 
		{
			return mProcessing.AzimuthStartTime + double(midInd) * mProcessing.LineTimeInterval;
		} 
		else if (midIndFreq * lowerIndFreq > 0.0) 
		{
			lowerInd = midInd;
			lowerIndFreq = midIndFreq;
		} 
		else if (midIndFreq * upperIndFreq > 0.0) 
		{
			upperInd = midInd;
			upperIndFreq = midIndFreq;
		}
	}

	double shift = - lowerIndFreq * double(upperInd - lowerInd) / (upperIndFreq - lowerIndFreq);
	return mProcessing.AzimuthStartTime + (double(lowerInd) + shift) * mProcessing.LineTimeInterval;
}

double RangeDopplerModel::DopplerFrequency( int azInd, DecartVector earthPoint )
{
	DecartVector diff;
	diff.X = earthPoint.X - mOrbit.Pos[azInd].X;
	diff.Y = earthPoint.Y - mOrbit.Pos[azInd].Y;
	diff.Z = earthPoint.Z - mOrbit.Pos[azInd].Z;
	double distance = sqrt(diff.X * diff.X + diff.Y * diff.Y + diff.Z * diff.Z);
	return (mOrbit.Vel[azInd].X * diff.X + mOrbit.Vel[azInd].Y * diff.Y + mOrbit.Vel[azInd].Z * diff.Z) / distance;
}

#include "StdAfx.h"
#include "TechConfig.h"
#include <RadarMap/Constants.h>

void RadarMap::Config::TechConfig::Load(const char* fileName)
{
	try
	{
		ticpp::Document doc(fileName); 
		doc.LoadFile();

		ticpp::Element* root = doc.FirstChildElement();

		root->FirstChildElement("Looks")->GetText(&Looks);

		ticpp::Element* addNoiseEl = root->FirstChildElement("AdditiveNoise");
		addNoiseEl->GetAttribute("mean", &AdditiveNoise.Mean);
		addNoiseEl->GetAttribute("stdev", &AdditiveNoise.Dev);

		Modes.clear();
		ticpp::Element* modesEl = root->FirstChildElement("Modes");
		ticpp::Iterator<ticpp::Element> modeIt("Mode");
		for (modeIt = modeIt.begin(modesEl); modeIt != modeIt.end(); ++modeIt) 
		{
			Mode mode;

			ticpp::Element* numElementsEl = modeIt->FirstChildElement("Elements");
			numElementsEl->FirstChildElement("Azimuth")->GetText(&mode.Elements.Azimuth);
			numElementsEl->FirstChildElement("Range")->GetText(&mode.Elements.Range);
			
			ticpp::Element* resolutionEl = modeIt->FirstChildElement("Resolution");
			resolutionEl->FirstChildElement("Azimuth")->GetText(&mode.Resolution.Azimuth);
			resolutionEl->FirstChildElement("Range")->GetText(&mode.Resolution.Range);
			mode.Resolution.Azimuth *= PI / 180.0f;

			modeIt->FirstChildElement("PartialFrames")->GetText(&mode.PartialFrames);

			std::string resultProjection = modeIt->FirstChildElement("Projection")->GetText();
			if (resultProjection == "Decart")
				mode.ResultProjection = Projection::Decart;
			else if (resultProjection == "Polar")
				mode.ResultProjection = Projection::Polar;

			std::string targetRender = modeIt->FirstChildElement("TargetRender")->GetText();
			if (targetRender == "Geometry")
				mode.TargetRender = Render::Geometry;
			else if (targetRender == "Point")
				mode.TargetRender = Render::Point;

			Modes.push_back(mode);
		}
	}
	catch(ticpp::Exception& ex)
	{
		Modes.clear();
		cout << "RadarMap: exception while loading file " << fileName << endl << flush;
		throw ex;
	}
}
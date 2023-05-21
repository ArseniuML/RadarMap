#include "stdafx.h"
#include "Targets.h"

RadarMap::Targets::Targets::Targets()
{
}

RadarMap::Targets::Targets::~Targets()
{
	Clear();
}

void RadarMap::Targets::Targets::LoadModels( const std::string& fileName )
{
	Clear();

	try
	{
		cout << "RadarMap: loading file " << fileName << endl << flush;
		
		std::string root = fileName.substr(0, fileName.find_last_of("\\") + 1);

		ticpp::Document doc(fileName);
		doc.LoadFile();

		long type = 0;
		float brightness = 0;
		std::string targetFileName;

		ticpp::Element* targetsEl = doc.FirstChildElement()->FirstChildElement("Targets");
		ticpp::Iterator< ticpp::Element> targetIt("Target");
		for (targetIt = targetIt.begin(targetsEl); targetIt != targetIt.end(); ++targetIt)
		{
			targetIt->GetAttribute("type", &type);
			targetIt->GetAttribute("brightness", &brightness);
			targetIt->GetAttribute("file", &targetFileName);
			mTargetMap.insert(TargetPair(type, new Model((root + targetFileName).c_str(), brightness)));
		}
	}
	catch (ticpp::Exception& ex)
	{
		Clear();
		cout << "RadarMap: exception while loading file " << fileName << endl << flush;
		cout << "RadarMap: " << ex.what() << endl << flush;
		throw ex;
	}
}

void RadarMap::Targets::Targets::ActivateTargets( const graph_29k::TActiveObjects2& activeObjectParams )
{
	BOOST_FOREACH(Model* pTarget, mActiveTargets)
		pTarget->ClearLocations();
	mActiveTargets.clear();

	for ( int i = 0;  i < activeObjectParams.Count; ++i )
	{
		//Игнорируем модель "_Self"
		char selfName[32] = "_Self";
		if (_mbscmp((unsigned char*)selfName, activeObjectParams.Items[i].Name) == 0) continue;
		
		long type = activeObjectParams.Items[i].Type;
		//Обнуляем старший байт
		*(reinterpret_cast<unsigned char*>(&type) + sizeof(type) - 1) = 0;

		TargetMap::iterator toi = mTargetMap.find(type);
		if (toi != mTargetMap.end())
		{
			Model* pTarget = toi->second;
			pTarget->AddLocation
			(
				activeObjectParams.Items[i].Z,
				activeObjectParams.Items[i].X,
				activeObjectParams.Items[i].Y,
				activeObjectParams.Items[i].Teta,
				activeObjectParams.Items[i].Psi,
				activeObjectParams.Items[i].Gamma
			);
			mActiveTargets.insert(pTarget);

#ifdef _DEBUG
			cout << "RadarMap: add target name " << (const char*)activeObjectParams.Items[i].Name <<
				" , type " << activeObjectParams.Items[i].Type << endl << flush;
			cout << "RadarMap: target X = " << activeObjectParams.Items[i].X <<
				" , Y = " << activeObjectParams.Items[i].Y << 
				" , Z = " << activeObjectParams.Items[i].Z << endl << flush;
#endif
		}
	}
}

void RadarMap::Targets::Targets::RenderTargets(Geometry::RenderProgram& geometryRenderProgram, const Request& req )
{
	BOOST_FOREACH(Model* pTarget, mActiveTargets)
		pTarget->Render(geometryRenderProgram, req);

#ifdef _DEBUG
	cout << "RadarMap: render targets, mActiveTargets.size = " << mActiveTargets.size() << endl << flush;
#endif
}

void RadarMap::Targets::Targets::RenderTargetsAsPoints( Point::RenderProgram& pointRenderProgram )
{
	BOOST_FOREACH(Model* pTarget, mActiveTargets)
		pTarget->RenderPoint(pointRenderProgram);

#ifdef _DEBUG
	cout << "RadarMap: render targets as points, mActiveTargets.size = " << mActiveTargets.size() << endl << flush;
#endif
}

void RadarMap::Targets::Targets::Clear()
{
	BOOST_FOREACH(TargetPair& top, mTargetMap)
		delete top.second;

	mTargetMap.clear();
	mActiveTargets.clear();
}

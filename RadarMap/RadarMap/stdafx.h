// stdafx.h: включаемый файл дл€ стандартных системных включаемых файлов
// или включаемых файлов дл€ конкретного проекта, которые часто используютс€,
// но не часто измен€ютс€

#pragma once

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED

#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// некоторые конструкторы CString будут €вными


#define ATL_NO_ASSERT_ON_DESTROY_NONEXISTENT_WINDOW

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>

#include <direct.h>

#include <vector>
#include <string>
#include <exception>

#include <boost/format.hpp>
#include <boost/foreach.hpp>

#include <GL/glee.h>
#include <GL/GLU.h>

#define TIXML_USE_TICPP
#include <ticpp.h>

#define API_LEV2
#include <mgapiall.h>

#include <half.h>
#include <ImfInputFile.h>
#include <ImfOutputFile.h>
#include <ImfChannelList.h>
#include <IexErrnoExc.h>

#include <IlmThreadMutex.h>

using std::cout;
using std::endl;
using std::flush;

#include <RadarMap/Config/AppConfig.h>
#include <RadarMap/Config/TechConfig.h>

extern RadarMap::Config::AppConfig appConfig;
extern RadarMap::Config::TechConfig techConfig;

// stdafx.h: включаемый файл дл€ стандартных системных включаемых файлов
// или включаемых файлов дл€ конкретного проекта, которые часто используютс€, но
// не часто измен€ютс€
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

// TODO: ”становите здесь ссылки на дополнительные заголовки, требующиес€ дл€ программы

#define NOMINMAX
#include <windows.h>

#include <string>
#include <map>

#include <boost/foreach.hpp>
#include <boost/format.hpp>

#define TIXML_USE_TICPP
#include <ticpp.h>

#define API_LEV2
#include <mgapiall.h>

#include <half.h>
#include <ImfOutputFile.h>
#include <ImfChannelList.h>
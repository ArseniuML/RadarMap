#include <fstream>
#include <string>
#include <vector>
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

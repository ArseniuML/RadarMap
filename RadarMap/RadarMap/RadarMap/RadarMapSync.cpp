#include "stdafx.h"
#include "RadarMapSync.h"

RadarMap::RadarMapSync::RadarMapSync(RLib::Window::Window* pWndShare):
Window(_T("RadarMap"), pWndShare)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//FBO mFB остается активным в течение жизни объекта класса RadarMapSync,
	//и по умолчанию используется в данном потоке
	glGenFramebuffers(1, &mFB);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFB);
	
	//Загрузка заголовка LADBM и 3D-моделей объектов
	mWorld.LoadDescription(appConfig.LoadOnStartup.Terrain);
	mWorld.LoadShapes(appConfig.LoadOnStartup.Targets);

	const GLubyte* ext = glGetString(GL_EXTENSIONS);
	std::string ext_str = std::string((const char*)ext);
	
	cout << "RadarMap: extensions: " << ext_str <<endl << flush;
}

RadarMap::RadarMapSync::~RadarMapSync()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &mFB);
}

void RadarMap::RadarMapSync::SetRequest( const Request& req )
{
	mRequest = req;
	Config::TechConfig::Mode& mode = techConfig.Modes[mRequest.Mode];
	mProcessor.Resize(mode.Elements.Azimuth, mode.Elements.Range);
}

void RadarMap::RadarMapSync::Build()
{
	mWorld.Render(mProcessor.ActiveTexture(), mRequest);
	mProcessor.Speckle(mRequest);
	mWorld.RenderPoints(mProcessor.ActiveTexture(), mRequest);
	mProcessor.Blur(mRequest);
	mProcessor.AdditiveNoise(mRequest);
	glFlush();

	cout << "RadarMap: build complete" << endl << flush;
}
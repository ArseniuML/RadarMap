#pragma once

#include "RequestData.h"

#include <RLib/Window/Window.h>
#include "World.h"
#include "Effects/Processor.h"
#include "Frame/Frame.h"


namespace RadarMap {

//Класс реализует процесс построения кадра РЛ карты: 
//чтение параметров кадра и списка активных целей
//загрузка оптимального LODа
//растеризация в координатах азимут - дальность
//моделирование искажающих эффектов
//Так как объект класса создается в отдельном потоке, то для использования OpenGL он должен иметь свой контекст рендеринга, то есть наследовать от класса окна (RLib::Window::Window). В процессе работы это окно свернуто.
//Для обеспечивания возможности копирования текстуры с построенным кадром между контекстами рендеринга классов RadarMapSync и RadarMapAsync эти контексты делаются видимыми друг другу с помощью ф-ции wglShareLists в конструкторе базового класса окна.


class RadarMapSync : private RLib::Window::Window
{
public:
	RadarMapSync(RLib::Window::Window* pWndShare);
	~RadarMapSync();

	void SetRequest( const Request& req);
	
	void LoadOptimalLevel(bool& stop)
	{
		mWorld.LoadOptimalLevel(mRequest, stop);
	}

	void ActivateTargets(const graph_29k::TActiveObjects2& activeObjectParams)
	{
		mWorld.ActivateTargets(activeObjectParams);
	}
	
	void Build();
	
	RLib::Collection::Texture& GetResult() 
	{
		return mProcessor.ActiveTexture();
	}

private:
	World mWorld;
	Request mRequest;

	GLuint mFB;
	Effects::Processor mProcessor;
};

}
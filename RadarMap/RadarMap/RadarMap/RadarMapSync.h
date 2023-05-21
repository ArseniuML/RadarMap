#pragma once

#include "RequestData.h"

#include <RLib/Window/Window.h>
#include "World.h"
#include "Effects/Processor.h"
#include "Frame/Frame.h"


namespace RadarMap {

//����� ��������� ������� ���������� ����� �� �����: 
//������ ���������� ����� � ������ �������� �����
//�������� ������������ LOD�
//������������ � ����������� ������ - ���������
//������������� ���������� ��������
//��� ��� ������ ������ ��������� � ��������� ������, �� ��� ������������� OpenGL �� ������ ����� ���� �������� ����������, �� ���� ����������� �� ������ ���� (RLib::Window::Window). � �������� ������ ��� ���� ��������.
//��� ������������� ����������� ����������� �������� � ����������� ������ ����� ����������� ���������� ������� RadarMapSync � RadarMapAsync ��� ��������� �������� �������� ���� ����� � ������� �-��� wglShareLists � ������������ �������� ������ ����.


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
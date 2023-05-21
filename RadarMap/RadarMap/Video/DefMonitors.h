#pragma once

#include <list>

//connect up STL
using namespace std;

// ��������� ��� ����������� ���������� ��������� � �� ����������
struct Monitor
{
	INT						nmb;				// monitor's number("1" - primary monitor)
	SIZE						size;				// monitor size
	POINT					offset_SC;	// offset system coordinates monitor relatively base SC
};

//	pointer to single monitor into system
typedef	list<Monitor>::iterator	itMonitor;
typedef	list<Monitor> ListMonitors;

class CDefMonitors
{
public:
	CDefMonitors(void);
	~CDefMonitors(void);

public:
	ListMonitors *list_monitors;

	//��������� ���������� �������� �� ������( "1"-������ ����� �������� )
	int GetMonitorParams(	__in	int nmb_monitor,				//����� �������� ��� ��������� ����������
												__out int &width_monitor,			//������ ��������
												__out int &height_monitor,		//������ ��������
												__out int &offset_X_monitor,	//�������� ����� ��� X ������������ ������ ������� ��
												__out int &offset_Y_monitor		//�������� ����� ��� Y ������������ ������ ������� ��
											);
};

#pragma once

#include <list>

//connect up STL
using namespace std;

// структуры дл€ определени€ количества мониторов и их параметром
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

	//получение параметров монитора по номеру( "1"-первый номер монитора )
	int GetMonitorParams(	__in	int nmb_monitor,				//номер монитора дл€ получени€ параметров
												__out int &width_monitor,			//ширина монитора
												__out int &height_monitor,		//высота монитора
												__out int &offset_X_monitor,	//смещение вдоль оси X относительно начала отсчЄта — 
												__out int &offset_Y_monitor		//смещение вдоль оси Y относительно начала отсчЄта — 
											);
};

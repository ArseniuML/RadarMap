#include "StdAfx.h"
#include "defmonitors.h"

ListMonitors List_Monitors;

BOOL CALLBACK MonitorSystemEnumProc(	HMONITOR hMonitor,HDC hdcMonitor, LPRECT lprcMonitor,	LPARAM dwData)
{
	BOOL err;
	Monitor loc_monitor;
	MONITORINFOEXA 	monitor_info_ex;		//structure contains information about a display monitor.

	monitor_info_ex.cbSize = sizeof(MONITORINFOEXA);
	err = GetMonitorInfoA(hMonitor, &monitor_info_ex);

	//определяем смещение монитора относительно базовой СК
	loc_monitor.offset_SC.x = lprcMonitor->left;
	loc_monitor.offset_SC.y = lprcMonitor->top;

	//определяем размеры монитора(!!! не рабочей области !!!)
	loc_monitor.size.cx = lprcMonitor->right - lprcMonitor->left;
	loc_monitor.size.cy = lprcMonitor->bottom - lprcMonitor->top;

	//получаем номер окна из строки(\\.\Display2)
	if ( strlen(monitor_info_ex.szDevice) ){
		loc_monitor.nmb = atoi( monitor_info_ex.szDevice + 11 );
	}else{
		loc_monitor.nmb = 0;
	}

	//проверка
	printf("Number monitor: %d\t", loc_monitor.nmb);
	printf("width_ils = %d;\t height_ils = %d;\n", loc_monitor.size.cx, loc_monitor.size.cy );
	//проверка

	List_Monitors.push_back(loc_monitor);

	return TRUE;
}

CDefMonitors::CDefMonitors(void)
{
	list_monitors = &List_Monitors;
	list_monitors->clear();

	//проверка
	printf("\\/\n");
	printf("!!!Monitor informations!!!\n");
	//проверка

	EnumDisplayMonitors(NULL, NULL, MonitorSystemEnumProc, NULL);

	//проверка
	printf("/\\\n");
	//проверка
}

CDefMonitors::~CDefMonitors(void)
{
	list_monitors->clear();
}

//получение параметров монитора по номеру( "1"-первый номер монитора )
int CDefMonitors::GetMonitorParams(	__in	int nmb_monitor,				//номер монитора для получения параметров
																		__out int &width_monitor,			//ширина монитора
																		__out int &height_monitor,		//высота монитора
																		__out int &offset_X_monitor,	//смещение вдоль оси X относительно начала отсчёта СК
																		__out int &offset_Y_monitor		//смещение вдоль оси Y относительно начала отсчёта СК
																	)
{
	itMonitor it_monitor_loc = list_monitors->begin();
	itMonitor it_monitor_end = list_monitors->end();
	itMonitor it_monitor_first = list_monitors->end();

	for (; it_monitor_loc != it_monitor_end; ++it_monitor_loc){
		if (it_monitor_loc->nmb == nmb_monitor){	//определили требуемый монитор

			width_monitor = it_monitor_loc->size.cx;
			height_monitor = it_monitor_loc->size.cy;

			offset_X_monitor = it_monitor_loc->offset_SC.x;
			offset_Y_monitor = it_monitor_loc->offset_SC.y;

			return 0;
		}
	}

	return -1;
}
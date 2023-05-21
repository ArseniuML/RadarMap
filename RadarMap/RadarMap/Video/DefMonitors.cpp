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

	//���������� �������� �������� ������������ ������� ��
	loc_monitor.offset_SC.x = lprcMonitor->left;
	loc_monitor.offset_SC.y = lprcMonitor->top;

	//���������� ������� ��������(!!! �� ������� ������� !!!)
	loc_monitor.size.cx = lprcMonitor->right - lprcMonitor->left;
	loc_monitor.size.cy = lprcMonitor->bottom - lprcMonitor->top;

	//�������� ����� ���� �� ������(\\.\Display2)
	if ( strlen(monitor_info_ex.szDevice) ){
		loc_monitor.nmb = atoi( monitor_info_ex.szDevice + 11 );
	}else{
		loc_monitor.nmb = 0;
	}

	//��������
	printf("Number monitor: %d\t", loc_monitor.nmb);
	printf("width_ils = %d;\t height_ils = %d;\n", loc_monitor.size.cx, loc_monitor.size.cy );
	//��������

	List_Monitors.push_back(loc_monitor);

	return TRUE;
}

CDefMonitors::CDefMonitors(void)
{
	list_monitors = &List_Monitors;
	list_monitors->clear();

	//��������
	printf("\\/\n");
	printf("!!!Monitor informations!!!\n");
	//��������

	EnumDisplayMonitors(NULL, NULL, MonitorSystemEnumProc, NULL);

	//��������
	printf("/\\\n");
	//��������
}

CDefMonitors::~CDefMonitors(void)
{
	list_monitors->clear();
}

//��������� ���������� �������� �� ������( "1"-������ ����� �������� )
int CDefMonitors::GetMonitorParams(	__in	int nmb_monitor,				//����� �������� ��� ��������� ����������
																		__out int &width_monitor,			//������ ��������
																		__out int &height_monitor,		//������ ��������
																		__out int &offset_X_monitor,	//�������� ����� ��� X ������������ ������ ������� ��
																		__out int &offset_Y_monitor		//�������� ����� ��� Y ������������ ������ ������� ��
																	)
{
	itMonitor it_monitor_loc = list_monitors->begin();
	itMonitor it_monitor_end = list_monitors->end();
	itMonitor it_monitor_first = list_monitors->end();

	for (; it_monitor_loc != it_monitor_end; ++it_monitor_loc){
		if (it_monitor_loc->nmb == nmb_monitor){	//���������� ��������� �������

			width_monitor = it_monitor_loc->size.cx;
			height_monitor = it_monitor_loc->size.cy;

			offset_X_monitor = it_monitor_loc->offset_SC.x;
			offset_Y_monitor = it_monitor_loc->offset_SC.y;

			return 0;
		}
	}

	return -1;
}
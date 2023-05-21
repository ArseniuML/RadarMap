// RadarMapDlgClientDlg.h : header file
//

#pragma once
#include "afxwin.h"

#import "..\..\common\dll\CommonTypes.dll"  no_namespace, named_guids exclude("IRT")  exclude("ISTC")
#import <RadarMap.dll>	raw_interfaces_only, raw_native_types, no_namespace, named_guids

#include <radarmap_29k.h>
#include <graph_29k.h>
#include <virtualdevices_29k.h>

class CRadarMapDlgClientDlg : public CDialog
{
// Construction
public:
	CRadarMapDlgClientDlg(CWnd* pParent = NULL);	// standard constructor
	~CRadarMapDlgClientDlg();	// standard constructor

	enum { IDD = IDD_RADARMAPDLGCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON	m_hIcon;
	UINT	m_nTimer;

	CSliderCtrl mSliderHeading;
	CSliderCtrl mSliderBrightness;
	CSliderCtrl mSliderDCenter;

	CComboBox mComboMode;

	IRadarMap*	mpIRadarMap;
	IRT*				mpIRT_RadarMap;
	ISTC*			mpISTC_RadarMap;
	
	ICS*				mpICS;
	IRT*				mpIRT_CS;			
	
	unsigned long mhRequired;
	unsigned long mhRadarMapOut;
	unsigned long mhMode;
	unsigned long mhActiveObjects;

	radarmap_29k::TRadarMapOut mRadarParams;
	int mMode;

	graph_29k::TActiveObjects2 mActiveObjects;
	
	float mDegreesHeading;
	std::vector<float> mAzimuthalRadius;

	float mFrameCenterZ;
	float mFrameCenterX;
	
	int mTimeToUpdate;
	bool	mRequest;

	std::streambuf* mpBufBack;
	std::ofstream mMessagesLog;

protected:
	void UpdateDMinMax();
	
	void UpdateSliderHeading();
	void UpdateSliderDCenter();
	void UpdateSliderBrightness();
	void UpdateAzimuthRadius();

	void LoadTechParams(const char* fileName);
	void LoadScenario(const char* fileName);

protected:

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg HCURSOR OnQueryDragIcon();
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonRequestMap();
	afx_msg void OnBnClickedButtonNavigate();
	afx_msg void OnEnKillfocusHeading();
	afx_msg void OnEnKillFocusDCenter();
	afx_msg void OnEnKillFocusHeight();
	afx_msg void OnEnUpdateDCenter();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCbnSelChangeComboMode();
};
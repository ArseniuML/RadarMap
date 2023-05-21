// RadarMapDlgClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RadarMapDlgClient.h"
#include "RadarMapDlgClientDlg.h"

#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRadarMapDlgClientDlg dialog

CRadarMapDlgClientDlg::CRadarMapDlgClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRadarMapDlgClientDlg::IDD, pParent),
	
	mpIRadarMap(NULL),
	mpIRT_RadarMap(NULL),
	mpISTC_RadarMap(NULL),

	mpICS(NULL),
	mpIRT_CS(NULL),

	mRequest(false)
{
	//{{AFX_DATA_INIT(CRadarMapDlgClientDlg)
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	memset(&mRadarParams, 0, sizeof(mRadarParams));
	mMode = 1;
	mFrameCenterZ = 0;
	mFrameCenterX = 0;
	memset(&mActiveObjects, 0, sizeof(mActiveObjects));
}

CRadarMapDlgClientDlg::~CRadarMapDlgClientDlg()
{	  

	if (mpIRT_RadarMap) mpIRT_RadarMap->Terminate();
	if (mpIRT_CS) mpIRT_CS->Terminate();

	if (mpIRT_CS) mpIRT_CS->Release();
	if (mpICS) mpICS->Release();
	
	if (mpIRT_RadarMap)		mpIRT_RadarMap->Release();
	if (mpISTC_RadarMap)		mpISTC_RadarMap->Release();
	if (mpIRadarMap)	mpIRadarMap->Release();

	::CoUninitialize();
	std::cout.rdbuf(mpBufBack);
}

void CRadarMapDlgClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_SLIDER_HEADING, mSliderHeading);
	DDX_Control(pDX, IDC_SLIDER_BRIGHTNESS, mSliderBrightness);
	DDX_Control(pDX, IDC_SLIDER_D, mSliderDCenter);
	DDX_Control(pDX, IDC_COMBO_MODE, mComboMode);

	DDX_Text(pDX, IDC_EDIT_Z,		mRadarParams.Airplane.Z);
	DDX_Text(pDX, IDC_EDIT_X,		mRadarParams.Airplane.X);
	DDX_Text(pDX, IDC_EDIT_Y,		mRadarParams.Airplane.Y);

	DDX_Text(pDX, IDC_EDIT_FRAMECENTER_Z, mFrameCenterZ);
	DDX_Text(pDX, IDC_EDIT_FRAMECENTER_X, mFrameCenterX);
}

BEGIN_MESSAGE_MAP(CRadarMapDlgClientDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	
	ON_WM_TIMER()
	
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	
	ON_BN_CLICKED(IDC_BUTTON_REQUESTMAP, &CRadarMapDlgClientDlg::OnBnClickedButtonRequestMap)
	ON_BN_CLICKED(IDC_BUTTON_NAVIGATE, &CRadarMapDlgClientDlg::OnBnClickedButtonNavigate)
	
	ON_EN_KILLFOCUS(IDC_EDIT_HEADING, &CRadarMapDlgClientDlg::OnEnKillfocusHeading)
	ON_EN_KILLFOCUS(IDC_EDIT_DCENTER, &CRadarMapDlgClientDlg::OnEnKillFocusDCenter)
	ON_EN_KILLFOCUS(IDC_EDIT_HEIGHT, &CRadarMapDlgClientDlg::OnEnKillFocusHeight)

	ON_EN_UPDATE(IDC_EDIT_DCENTER, &CRadarMapDlgClientDlg::OnEnUpdateDCenter)
	ON_CBN_SELCHANGE(IDC_COMBO_MODE, &CRadarMapDlgClientDlg::OnCbnSelChangeComboMode)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRadarMapDlgClientDlg message handlers

BOOL CRadarMapDlgClientDlg::OnInitDialog()
{
//_ASSERTE( _CrtCheckMemory( ) );
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	mMessagesLog.open("messages.log");
	mpBufBack = std::cout.rdbuf();
	std::cout.rdbuf(mMessagesLog.rdbuf());

	mSliderHeading.SetRange(0, 360);
	mSliderHeading.SetTicFreq(45);
	mSliderHeading.SetPageSize(45);

	mSliderBrightness.SetRange(0, 100);
	mSliderBrightness.SetTicFreq(10);
	mSliderBrightness.SetPageSize(10);

	mSliderDCenter.SetRange(0, 200);
	mSliderDCenter.SetTicFreq(10);
	mSliderDCenter.SetPageSize(10);

	m_nTimer = SetTimer(1, 20, 0);

	::CoInitialize(NULL);
	GUID CLSID_CS, IID_ICS_CS, IID_IRT_CS;
	UuidFromStringA((unsigned char*)"98ACE806-D444-43D3-9AAE-ABC7F4991360",&CLSID_CS);
	UuidFromStringA((unsigned char*)"BCEE0A86-28C3-4A3F-8531-B48029F68563",&IID_IRT_CS);
	UuidFromStringA((unsigned char*)"9478A4D4-E9CB-40EE-A821-F6B5ECFC0E26",&IID_ICS_CS);	
	
	
	HRESULT  hr_CS = ::CoCreateInstance 
	( 
		CLSID_CS,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_ICS_CS,
		(void**) &mpICS
	);
	mpICS->QueryInterface ( IID_IRT_CS,  (void**) &mpIRT_CS );

	HRESULT  hr_RadarMap = ::CoCreateInstance 
	( 
		CLSID_RadarMapCom,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IRadarMap,
		(void**) &mpIRadarMap
	);
	mpIRadarMap->QueryInterface ( IID_IRT,  (void**) &mpIRT_RadarMap );
	mpIRadarMap->QueryInterface ( IID_ISTC,  (void**) &mpISTC_RadarMap );

	mpIRT_CS->Initialize(0);
	mpISTC_RadarMap->STConnect(mpICS, 10);
	mpIRT_RadarMap->Initialize(0);

	mpICS->Open(&mhRequired, "VideoChannels.Generator.Required.6", sizeof(int));
	mpICS->Open(&mhRadarMapOut, RADAR_BUFFER_NAME, 96);
	mpICS->Open(&mhMode, "Dispatcher.Targeting.Radar.Map.Mode", sizeof(mMode));
	mpICS->Open(&mhActiveObjects, "SCENARIO_OBJECTS_DATA", sizeof(mActiveObjects));

	int req = virtualdevices_29k::VirtualDeviceType::RLMap;
	mpICS->Write(mhRequired, (unsigned char*)(&req), 0, sizeof(int));

	LoadTechParams("RadarMap.config");
	LoadScenario("ScenarioIndia.xml");
	//LoadScenario("ScenarioSyria.xml");

	mTimeToUpdate = 7000;
	SetDlgItemInt(IDC_EDIT_WAIT, mTimeToUpdate);

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CRadarMapDlgClientDlg::OnDestroy() 
{
	KillTimer(m_nTimer); 
	CDialog::OnDestroy();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRadarMapDlgClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRadarMapDlgClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CRadarMapDlgClientDlg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == m_nTimer)
	{
		static int time = 0;
		time += 20;
		
		mpIRT_CS->Run(time);
		mpIRT_RadarMap->Run(time);
		
		if (mRequest)
		{

			if (mRadarParams.TimeToUpdate > 0) 
			{
				mRadarParams.TimeToUpdate -= 20;
				if (mRadarParams.TimeToUpdate < 0) mRadarParams.TimeToUpdate = 0;
				mpICS->Write
				(
					mhRadarMapOut, 
					(unsigned char*)(&mRadarParams.TimeToUpdate), 
					(int)((unsigned char*)&mRadarParams.TimeToUpdate) - (int)((unsigned char*)&mRadarParams), 
					sizeof(mRadarParams.TimeToUpdate)
				);			
			}
			else
			{
				mRequest = false;
				mRadarParams.TimeToUpdate = mTimeToUpdate;
			}
			
			SetDlgItemInt(IDC_EDIT_WAIT, mRadarParams.TimeToUpdate);
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CRadarMapDlgClientDlg::OnBnClickedButtonRequestMap() 
{
	UpdateData(TRUE);
	mRadarParams.Airplane.Heading = PI * mDegreesHeading / 180.0;
	++(mRadarParams.RequestMap);
	
	mTimeToUpdate = GetDlgItemInt(IDC_EDIT_WAIT);
	mRadarParams.TimeToUpdate = mTimeToUpdate;
	mRequest = true;
	
	mpICS->Write(mhRadarMapOut, (unsigned char*)(&mRadarParams), 0, sizeof(mRadarParams));
	mpICS->Write(mhMode, (unsigned char*)(&mMode), 0, sizeof(mMode));
	mpICS->Write(mhActiveObjects, (unsigned char*)(&mActiveObjects), 0, sizeof(mActiveObjects));
}


void CRadarMapDlgClientDlg::OnBnClickedButtonNavigate()
{
	UpdateData(TRUE);

	float deltaZ = mFrameCenterZ - mRadarParams.Airplane.Z;
	float deltaX = mFrameCenterX - mRadarParams.Airplane.X;

	float sq = deltaZ* deltaZ + deltaX * deltaX;
	if (sq == 0) return;

	float heading = acosf(deltaX /sqrtf(sq));
	if (deltaZ > 0) heading = - heading;

	mRadarParams.Airplane.Heading = heading;
	mDegreesHeading = 180.f * heading / PI;

	mRadarParams.DCenter = sqrt(deltaZ * deltaZ + deltaX * deltaX + 
		mRadarParams.Airplane.Height * mRadarParams.Airplane.Height);

	CString text;
	text.Format(_T("%.2f"), mDegreesHeading);
	SetDlgItemText(IDC_EDIT_HEADING, text);
	UpdateSliderHeading();

	text.Format(_T("%.f"), mRadarParams.DCenter);
	SetDlgItemText(IDC_EDIT_DCENTER, text);
	UpdateSliderDCenter();
}

void CRadarMapDlgClientDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CString text;
	if  (pScrollBar == (CScrollBar*)GetDlgItem(IDC_SLIDER_HEADING))
	{
		int curpos = ((CSliderCtrl*)pScrollBar)->GetPos();
		mDegreesHeading = -float(curpos) + 180.0;
		
		text.Format(_T("%.2f"), mDegreesHeading);
		SetDlgItemText(IDC_EDIT_HEADING, text);
	}
	else if (pScrollBar == (CScrollBar*)GetDlgItem(IDC_SLIDER_BRIGHTNESS))
	{
		mRadarParams.Brightness = ((CSliderCtrl*)pScrollBar)->GetPos();
		SetDlgItemInt(IDC_EDIT_BRIGHTNESS,  mRadarParams.Brightness);
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CRadarMapDlgClientDlg::OnEnKillFocusHeight()
{
	CString text; GetDlgItemText(IDC_EDIT_HEIGHT, text);
	mRadarParams.Airplane.Height = _tstof(text);
	UpdateDMinMax();
}

void CRadarMapDlgClientDlg::OnEnUpdateDCenter()
{
	CString text; ((CWnd*)GetDlgItem(IDC_EDIT_DCENTER))->GetWindowText(text);
	mRadarParams.DCenter = _tstof(text);

	UpdateDMinMax();
}

void CRadarMapDlgClientDlg::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	CString text;
	if  (pScrollBar == (CScrollBar*)GetDlgItem(IDC_SLIDER_D))
	{
		CSliderCtrl* pSliderCtrl = (CSliderCtrl*)pScrollBar;
		int curpos = pSliderCtrl->GetRangeMax() - pSliderCtrl->GetPos();

		mRadarParams.DCenter = 1000 * curpos;
		text.Format(_T("%.f"), mRadarParams.DCenter);
		SetDlgItemText(IDC_EDIT_DCENTER, text);
	}
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CRadarMapDlgClientDlg::OnEnKillfocusHeading()
{
	CString text; GetDlgItemText(IDC_EDIT_HEADING, text);
	mDegreesHeading = _tstof(text);
	UpdateSliderHeading();
}

void CRadarMapDlgClientDlg::OnEnKillFocusDCenter()
{
	UpdateSliderDCenter();
}

void CRadarMapDlgClientDlg::OnCbnSelChangeComboMode()
{
	mMode = mComboMode.GetCurSel() + 1;
	UpdateAzimuthRadius();
	UpdateDMinMax();
}

void CRadarMapDlgClientDlg::LoadTechParams( const char* fileName )
{
	ticpp::Document doc(fileName);doc.LoadFile();
	ticpp::Element* modesEl = doc.FirstChildElement()->FirstChildElement("Modes");

	mAzimuthalRadius.clear();
	mComboMode.Clear();
	
	ticpp::Iterator<ticpp::Element> modeIt("Mode");
	for (modeIt = modeIt.begin(modesEl); modeIt != modeIt.end(); ++modeIt) 
	{
		std::string name = modeIt->GetAttribute("name");
		mComboMode.AddString(CString(name.c_str()));
		float azimuthalRadius; 
		modeIt->FirstChildElement("AzimuthalRadius")->GetText(&azimuthalRadius);
		mAzimuthalRadius.push_back(azimuthalRadius);
	}
}

void CRadarMapDlgClientDlg::LoadScenario( const char* fileName )
{
	ticpp::Document doc(fileName); doc.LoadFile();
	ticpp::Element* rootEl = doc.FirstChildElement();

	ticpp::Element* airplaneEl = rootEl->FirstChildElement("Airplane");
	airplaneEl->FirstChildElement("X")->GetText(&mRadarParams.Airplane.X);
	airplaneEl->FirstChildElement("Y")->GetText(&mRadarParams.Airplane.Y);
	airplaneEl->FirstChildElement("Z")->GetText(&mRadarParams.Airplane.Z);
	airplaneEl->FirstChildElement("Height")->GetText(&mRadarParams.Airplane.Height);
	
	CString text; text.Format(_T("%.f"), mRadarParams.Airplane.Height);
	SetDlgItemText(IDC_EDIT_HEIGHT, text);

	airplaneEl->FirstChildElement("Heading")->GetText(&mDegreesHeading);
	rootEl->FirstChildElement("DCenter")->GetText(&mRadarParams.DCenter);
	
	mMode = mComboMode.FindStringExact(0, 
		CString(rootEl->FirstChildElement("Mode")->GetText().c_str())) + 1;
	mComboMode.SetCurSel(mMode - 1);
	UpdateAzimuthRadius();	

	text.Format(_T("%.2f"), mDegreesHeading);
	SetDlgItemText(IDC_EDIT_HEADING, text);
	UpdateSliderHeading();
	
	text.Format(_T("%.f"), mRadarParams.DCenter);
	SetDlgItemText(IDC_EDIT_DCENTER, text);
	UpdateSliderDCenter();

	mRadarParams.Brightness = 50;
	UpdateSliderBrightness();

	mActiveObjects.Count = 1;
	mActiveObjects.Items[0].Type = 5;
	mActiveObjects.Items[0].Z = -210000;
	mActiveObjects.Items[0].X = -240000;
	mActiveObjects.Items[0].Y = 10;
	mActiveObjects.Items[0].Teta = 0;
	mActiveObjects.Items[0].Psi = 0;
	mActiveObjects.Items[0].Gamma = 0;

	UpdateData(FALSE);
}

void CRadarMapDlgClientDlg::UpdateDMinMax()
{
	float DCenterHor2 = mRadarParams.DCenter * mRadarParams.DCenter - mRadarParams.Airplane.Height * mRadarParams.Airplane.Height;
	if (DCenterHor2 < 0) return;

	float DCenterHor = sqrtf(DCenterHor2);
	float halfAzRadius = 0.5f * abs(mRadarParams.AzimuthLeft - mRadarParams.AzimuthRight);

	if (mMode == 6)
	{
		float nm = 1852; //миля

		if (mRadarParams.DCenter >= 145000)
		{
			mRadarParams.MapHeight = 17 * nm;
		}
		else
		{
			mRadarParams.MapHeight = 10 * nm;
		}

		mRadarParams.DMin = mRadarParams.DCenter - 0.5 * mRadarParams.MapHeight;
		mRadarParams.DMax = mRadarParams.DCenter + 0.5 * mRadarParams.MapHeight;
	}
	else
	{
		CString rad; GetDlgItemText(IDC_EDIT_DRADIUS, rad);
		if (rad.GetLength() == 0)
		{
			mRadarParams.MapWidth = 2.f * DCenterHor * sinf(halfAzRadius);
			mRadarParams.MapHeight = mRadarParams.MapWidth;
		}
		else
		{
			mRadarParams.MapWidth = 2 * halfAzRadius;
			mRadarParams.MapHeight = _tstof(rad);
		}

		float DMaxHor = DCenterHor + 0.5f * mRadarParams.MapHeight;
		float DMinHor = DCenterHor - 0.5f * mRadarParams.MapHeight;

		mRadarParams.DMax = sqrtf(DMaxHor * DMaxHor + mRadarParams.Airplane.Height * mRadarParams.Airplane.Height);
		mRadarParams.DMin = sqrtf(DMinHor * DMinHor + mRadarParams.Airplane.Height * mRadarParams.Airplane.Height);
	}

	CString text;
	text.Format(_T("%.f"), mRadarParams.DMax);	SetDlgItemText(IDC_EDIT_DMAX, text);
	text.Format(_T("%.f"), mRadarParams.DMin); SetDlgItemText(IDC_EDIT_DMIN, text);
}


void CRadarMapDlgClientDlg::UpdateSliderHeading()
{
	mSliderHeading.SetPos(-mDegreesHeading + 180.0);
}

void CRadarMapDlgClientDlg::UpdateSliderDCenter()
{
	mSliderDCenter.SetPos(mSliderDCenter.GetRangeMax() - 0.001 * mRadarParams.DCenter);
}

void CRadarMapDlgClientDlg::UpdateSliderBrightness()
{
	SetDlgItemInt(IDC_EDIT_BRIGHTNESS, mRadarParams.Brightness);
	mSliderBrightness.SetPos(mRadarParams.Brightness);
}

void CRadarMapDlgClientDlg::UpdateAzimuthRadius()
{
	CString text;
	text.Format(_T("%.2f"), mAzimuthalRadius[mMode - 1]);
	SetDlgItemText(IDC_EDIT_AZ_RADIUS, text);

	mRadarParams.AzimuthLeft = 0.5f * mAzimuthalRadius[mMode - 1] * PI / 180.f;
	mRadarParams.AzimuthRight = -mRadarParams.AzimuthLeft;
}
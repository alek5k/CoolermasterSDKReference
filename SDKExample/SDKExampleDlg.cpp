
#include "stdafx.h"
#include "SDKExample.h"
#include "SDKExampleDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define TIMER_GET_SYS_INFO    0x10001
#define MAX_COLOR_VALUE		  255
#define EFFECT_NUM			  20
#define DEVICE_NUM			  21

CString g_DeviceName[DEVICE_NUM] = {_T("MasterKeys Pro L"), _T("MasterKeys Pro S"), _T("MasterKeys Pro L White"),
									_T("MasterKeys Pro M White"), _T("MasterMouse Pro L"),

								    _T("MasterMouse Pro S"), _T("MasterKeys Pro M"), _T("MasterKeys Pro S White"),
									_T("MM520"), _T("MM530"),									
									_T("MK750"), _T("CK372"), _T("CK550/552"), _T("CK551"), _T("MM830"),
									_T("CK530"), _T("MK850"), 
									_T("SK630"), _T("SK650"), _T("SK621"), _T("MK730"),
};


CString g_EffectName[EFFECT_NUM] = {_T("Full ON"), _T("Breathing"), _T("Color Cycle"), _T("Single Key"),
									_T("Wave Color"), _T("Ripple"), _T("Cross Mode"), _T("Rain Drop"), 
									_T("Star Effect"), _T("Game Snake"), _T("Customization"), _T("Indicator"),

									_T("Fireball"), _T("Water Ripple"), _T("Reactive Punch"), _T("Snowing"),
									_T("Heartbeat"), _T("Circle Spectrum"), _T("Reactive Tornado"), 
									_T("MultiLayer")};

EFF_INDEX  g_EffectMap[EFFECT_NUM] = {EFF_FULL_ON, EFF_BREATH, EFF_BREATH_CYCLE, EFF_SINGLE, EFF_WAVE,
									 EFF_RIPPLE, EFF_CROSS, EFF_RAIN, EFF_STAR, EFF_SNAKE, EFF_REC, EFF_INDICATOR,

									 EFF_FIRE_BALL, EFF_WATER_RIPPLE, EFF_REACTIVE_PUNCH, EFF_SNOWING, 
									 EFF_HEART_BEAT, EFF_SPECTRUM, EFF_REACTIVE_TORNADO,

									 EFF_MULTI_1};

CSDKExampleDlg * g_expDlg = NULL;


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()





CSDKExampleDlg::CSDKExampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSDKExampleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bChange = false;
	m_bLedControl = false;
	g_expDlg = this;
	m_dwCPUError = ERROR_SUCCESS;
}

void CSDKExampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ROW, m_cbRow);
	DDX_Control(pDX, IDC_COMBO_COLUMN, m_cbColumn);
	DDX_Control(pDX, IDC_EDIT_R, m_edR);
	DDX_Control(pDX, IDC_EDIT_G, m_edG);
	DDX_Control(pDX, IDC_EDIT_B, m_edB);
	DDX_Control(pDX, IDC_COMBO_EFFECT, m_cbEffectList);
	DDX_Control(pDX, IDC_EDIT_R2, m_edR2);
	DDX_Control(pDX, IDC_EDIT_G2, m_edG2);
	DDX_Control(pDX, IDC_EDIT_B2, m_edB2);
	DDX_Control(pDX, IDC_COMBO_DEVICE, m_cbDevList);
	DDX_Control(pDX, IDC_BUTTON_DEVICE_SET, m_DevSet);
}

BEGIN_MESSAGE_MAP(CSDKExampleDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_ROW, &CSDKExampleDlg::OnCbnSelchangeKey)
	ON_CBN_SELCHANGE(IDC_COMBO_COLUMN, &CSDKExampleDlg::OnCbnSelchangeKey)

	ON_EN_CHANGE(IDC_EDIT_R, &CSDKExampleDlg::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_G, &CSDKExampleDlg::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_B, &CSDKExampleDlg::OnEnChangeEdit)
	ON_BN_CLICKED(IDC_BUTTON_SINGLE_KEY, &CSDKExampleDlg::OnBnClickedButtonSingleKey)
	ON_BN_CLICKED(IDC_BUTTON_ALL_KEY, &CSDKExampleDlg::OnBnClickedButtonAllKey)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_LED_CONTROL, &CSDKExampleDlg::OnBnClickedButtonLedControl)
	ON_BN_CLICKED(IDC_BUTTON_EFFECT_SET, &CSDKExampleDlg::OnBnClickedButtonEffectSet)
	ON_BN_CLICKED(IDC_BUTTON_SET_ONE_COLOR, &CSDKExampleDlg::OnBnClickedButtonSetOneColor)
	ON_BN_CLICKED(IDC_BUTTON_DEVICE_SET, &CSDKExampleDlg::OnBnClickedButtonDeviceSet)
	ON_BN_CLICKED(IDC_BUTTON_GET_LAYOUT, &CSDKExampleDlg::OnBnClickedButtonGetLayout)
	ON_BN_CLICKED(IDC_CHECK_KEY_EFF, &CSDKExampleDlg::OnBnClickedCheckKeyEff)
END_MESSAGE_MAP()


#include <PDHMsg.h>

void PDHMsgError(DWORD dwErrorCode)
{
    HANDLE hPdhLibrary = NULL;
    LPWSTR pMessage = NULL;
    DWORD_PTR pArgs[] = { (DWORD_PTR)L"<collectionname>" };
    //dwErrorCode = PDH_PLA_ERROR_ALREADY_EXISTS;

	CString csErrorCode;
	csErrorCode.Format(_T("Error Code: 0x%x, "), dwErrorCode);

    hPdhLibrary = LoadLibrary(L"pdh.dll");
    if (NULL == hPdhLibrary)
    {
        wprintf(L"LoadLibrary failed with %lu\n", GetLastError());
        return;
    }


    // Use the arguments array if the message contains insertion points, or you
    // can use FORMAT_MESSAGE_IGNORE_INSERTS to ignore the insertion points.

    if (!FormatMessage(FORMAT_MESSAGE_FROM_HMODULE |
                       FORMAT_MESSAGE_ALLOCATE_BUFFER |
                       /*FORMAT_MESSAGE_IGNORE_INSERTS |*/
                       FORMAT_MESSAGE_ARGUMENT_ARRAY,
                       hPdhLibrary, 
                       dwErrorCode,
                       0,  
                       (LPWSTR)&pMessage, 
                       0, 
                       //NULL))
                       (va_list*)pArgs))
    {
    //    wprintf(L"Format message failed with 0x%x\n", GetLastError());
		MessageBox(NULL, csErrorCode, _T("Error"), MB_OK|MB_ICONSTOP);  
        return;
    }

	csErrorCode.Append(pMessage);
	MessageBox(NULL, csErrorCode, _T("Error"), MB_OK|MB_ICONSTOP);  
    LocalFree(pMessage);
}


void ShowErrMsg(DWORD dwError)  
{  

	if (dwError == ERROR_SUCCESS)
		return ;

	if (dwError < 0x80000000L)
	{
		LPVOID lpMsgBuf;  

		FormatMessage(  
			FORMAT_MESSAGE_ALLOCATE_BUFFER |  
			FORMAT_MESSAGE_FROM_SYSTEM,  
			NULL,  
			dwError,  
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  
			(LPTSTR) &lpMsgBuf,  
			0, NULL );  
	  
		 CString csErrorCode;
		 csErrorCode.Format(_T("Error Code: 0x%x, %s"), dwError, lpMsgBuf);
		  MessageBox(NULL, csErrorCode, _T("System Error"), MB_OK|MB_ICONSTOP);  
	  
		LocalFree(lpMsgBuf);  
	}
	else
	{
		PDHMsgError(dwError);
	}
  }  


BOOL CSDKExampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//  [...] 

	// IDM_ABOUTBOX 
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		bool bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	CString  csName = _T("SDKExample Ver:");
	CString csVer;
	csVer.Format(_T("%d"), GetCM_SDK_DllVer());
	csName.Append(csVer);
	SetWindowText(csName);

	memset(&m_colorMatrix, 0x00, sizeof(m_colorMatrix));

	CString csTemp;

	for (int iIndex = 0; iIndex < MAX_LED_ROW; iIndex++)
	{
		csTemp.Format(_T("%d"), iIndex );
		m_cbRow.AddString(csTemp);
	}
	m_cbRow.SetCurSel(0);

	for (int iIndex = 0; iIndex < MAX_LED_COLUMN; iIndex++)
	{
		csTemp.Format(_T("%d"), iIndex );
		m_cbColumn.AddString(csTemp);
	}
	m_cbColumn.SetCurSel(0);

	m_edR.SetLimitText(3);
	m_edG.SetLimitText(3);
	m_edB.SetLimitText(3);

	m_edR2.SetLimitText(3);
	m_edG2.SetLimitText(3);
	m_edB2.SetLimitText(3);

	m_edR2.SetWindowText(_T("0"));
	m_edG2.SetWindowText(_T("0"));
	m_edB2.SetWindowText(_T("0"));

	for (int iDevice = 0; iDevice < DEVICE_NUM; iDevice++)
	{
		m_cbDevList.AddString(g_DeviceName[iDevice]);
	}
	m_cbDevList.SetCurSel(0);

#ifdef _DEBUG
	m_cbDevList.SetCurSel(DEV_MK850);
#endif


	for (int iEffect = 0; iEffect < EFFECT_NUM; iEffect++)
	{
		m_cbEffectList.AddString(g_EffectName[iEffect]);
	}
	m_cbEffectList.SetCurSel(0);

	OnCbnSelchangeKey();
	OnBnClickedButtonLedControl();

	SetTimer(TIMER_GET_SYS_INFO, 500, NULL);

	//ShowErrMsg(0x800007d5);
	return TRUE;
}


void CSDKExampleDlg::OnDestroy()
{
	CDialog::OnDestroy();
	EnableLedControl(false);
}


void CSDKExampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CSDKExampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CSDKExampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSDKExampleDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case TIMER_GET_SYS_INFO:
		GetSysInfo();
		break;
	}
	CDialog::OnTimer(nIDEvent);
}


void CSDKExampleDlg::GetSysInfo()
{
	CString csInfo = GetNowTime();
	GetDlgItem(IDC_EDIT_SYS_TIME)->SetWindowText(csInfo);

	DWORD dwError = ERROR_SUCCESS;
	LONG lCPUUsage =  GetNowCPUUsage(&dwError);
	csInfo.Format(_T("%d"), lCPUUsage);
	GetDlgItem(IDC_EDIT_CPU_USAGE)->SetWindowText(csInfo);
	if (dwError != ERROR_SUCCESS)
	{
		m_dwCPUError = dwError;

	}


	DWORD dwRamUsage =  GetRamUsage();
	csInfo.Format(_T("%d"), dwRamUsage);
	GetDlgItem(IDC_EDIT_RAM_USGAE)->SetWindowText(csInfo);

	float fPeekValue = GetNowVolumePeekValue();
	csInfo.Format(_T("%d"), int (fPeekValue * 100));
	GetDlgItem(IDC_EDIT_VOLUME_PEEK)->SetWindowText(csInfo);
}


void CSDKExampleDlg::OnBnClickedButtonDeviceSet()
{
	CButton * pCheck = (CButton*) GetDlgItem(IDC_CHECK_KEY_EFF);
	pCheck->SetCheck(FALSE); 
	OnBnClickedCheckKeyEff();


	DEVICE_INDEX devIndex = (DEVICE_INDEX)m_cbDevList.GetCurSel();
	SetControlDevice(devIndex);
	EnableLedControl(m_bLedControl);
	if (m_bLedControl)
		RefreshLed(true);
}


void CSDKExampleDlg::OnBnClickedButtonLedControl()
{
	m_bLedControl = !m_bLedControl;
	EnableLedControl(m_bLedControl);
	if (m_bLedControl)
		RefreshLed(true);

	if (m_bLedControl == true)
		GetDlgItem(IDC_BUTTON_LED_CONTROL)->SetWindowText(_T("Disable"));
	else
		GetDlgItem(IDC_BUTTON_LED_CONTROL)->SetWindowText(_T("Enable"));

	GetDlgItem(IDC_BUTTON_EFFECT_SET)->EnableWindow(!m_bLedControl);

	GetDlgItem(IDC_BUTTON_SINGLE_KEY)->EnableWindow(m_bLedControl);
	GetDlgItem(IDC_BUTTON_ALL_KEY)->EnableWindow(m_bLedControl);
	GetDlgItem(IDC_BUTTON_SET_ONE_COLOR)->EnableWindow(m_bLedControl);


}

void CSDKExampleDlg::OnBnClickedButtonEffectSet()
{
	int iCurSel = m_cbEffectList.GetCurSel();

	EFF_INDEX effIndex = g_EffectMap[iCurSel];
	bool bSuccess = SwitchLedEffect(effIndex);
	if (bSuccess == FALSE)
	{
		AfxMessageBox(_T("No Effect or Fail"));
	}

}


void CSDKExampleDlg::OnCbnSelchangeKey()
{
	m_bChange = true;
	int iRow = m_cbRow.GetCurSel();
	int iColumn = m_cbColumn.GetCurSel();
	const KEY_COLOR & keyColor = m_colorMatrix.KeyColor[iRow][iColumn];

	CString csColor;
	csColor.Format(_T("%d"), keyColor.r);
	m_edR.SetWindowText(csColor);

	csColor.Format(_T("%d"), keyColor.g);
	m_edG.SetWindowText(csColor);

	csColor.Format(_T("%d"), keyColor.b);
	m_edB.SetWindowText(csColor);

	m_bChange = false;

}

void CSDKExampleDlg::OnEnChangeEdit()
{
	if (m_bChange == true)
		return ;

	int iRow = m_cbRow.GetCurSel();
	int iColumn = m_cbColumn.GetCurSel();
	KEY_COLOR & keyColor = m_colorMatrix.KeyColor[iRow][iColumn];


	keyColor.r = GetColorValue(&m_edR);
	keyColor.g = GetColorValue(&m_edG);
	keyColor.b = GetColorValue(&m_edB);

}

BYTE CSDKExampleDlg::GetColorValue(CEdit * pEdit)
{
	BYTE byColor = 0x00;

	CString csColor;
	pEdit->GetWindowText(csColor);

	if (_ttoi(csColor) > MAX_COLOR_VALUE)
		byColor = MAX_COLOR_VALUE;
	else
		byColor = _ttoi(csColor);
	return byColor;
}

void CSDKExampleDlg::OnBnClickedButtonSingleKey()
{

	int iRow = m_cbRow.GetCurSel() ;
	int iColumn = m_cbColumn.GetCurSel() ;
	KEY_COLOR & keyColor = m_colorMatrix.KeyColor[iRow][iColumn];

	bool bSuccess = SetLedColor(iRow , iColumn , keyColor.r, keyColor.g, keyColor.b);
	if (bSuccess == false)
		MessageBox(_T("Set Single Key Fail!!"), _T("Warn"), MB_OK);

}

void CSDKExampleDlg::OnBnClickedButtonAllKey()
{
	bool bSuccess = SetAllLedColor(m_colorMatrix);
	if (bSuccess == false)
		MessageBox(_T("Set All Key Fail!!"), _T("Warn"), MB_OK);
}




void CSDKExampleDlg::OnBnClickedButtonSetOneColor()
{
	CString csValue;
	BYTE r = GetColorValue(&m_edR2);
	BYTE g = GetColorValue(&m_edG2);
	BYTE b = GetColorValue(&m_edB2);

	bool bSuccess = SetFullLedColor( r, g, b);
	if (bSuccess == false)
		MessageBox(_T("SetFullLedColor Fail!!"), _T("Warn"), MB_OK);
}

void CSDKExampleDlg::OnBnClickedButtonGetLayout()
{
	CEdit * pEditLayout = (CEdit *) GetDlgItem(IDC_EDIT_LAYOUT);

	LAYOUT_KEYBOARD layout = GetDeviceLayout();
	if (layout == LAYOUT_UNINIT)
		pEditLayout->SetWindowText(_T("Unplug"));
	else if (layout == LAYOUT_US)
		pEditLayout->SetWindowText(_T("US"));
	else if (layout == LAYOUT_EU)
		pEditLayout->SetWindowText(_T("EU"));
	else if (layout == LAYOUT_JP)
		pEditLayout->SetWindowText(_T("JP"));

}


void CALLBACK KeyCallBack(int iRow, int iColumn, bool bPressed)
{
	if (g_expDlg->m_bLedControl == true)
	{

		DEVICE_INDEX devIndex = (DEVICE_INDEX)g_expDlg->m_cbDevList.GetCurSel();
		if (devIndex == DEV_MMouse_L)
		{
			if (iColumn  > 3)
				iColumn = 3;
		}
		else if (devIndex == DEV_MM520 || devIndex == DEV_MM530)
		{
			if (iColumn  > 2)
				iColumn = 2;
		}
		if (bPressed)
			SetLedColor(iRow, iColumn, 255, 0 ,0);
		else
			SetLedColor(iRow, iColumn, 0, 0 ,0);
		//Sleep(1);
	}
}

void CSDKExampleDlg::OnBnClickedCheckKeyEff()
{
	CButton * pCheck = (CButton*) GetDlgItem(IDC_CHECK_KEY_EFF);
	if (pCheck->GetCheck())
	{
		SetKeyCallBack(KeyCallBack);
		EnableKeyInterrupt(true);
	}
	else
	{
		SetKeyCallBack(NULL);
		EnableKeyInterrupt(false);
	}
}

BOOL CSDKExampleDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg-> message == WM_KEYDOWN)
    {
		CButton * pCheck = (CButton*) GetDlgItem(IDC_CHECK_KEY_EFF);
		if (pCheck->GetCheck())
		{
			return TRUE;
		}
    }

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CSDKExampleDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	int iID = wParam;
	if (iID == IDC_BUTTON_CEHCK_DEV)
	{
		bool bPlug = IsDevicePlug();
		if (bPlug == TRUE)
			AfxMessageBox(_T("Device is plugged!!"));
		else
			AfxMessageBox(_T("Device is removed!!"));
	}
	if (iID == IDC_BUTTON_CPU_STATUS)
	{
		if (m_dwCPUError != ERROR_SUCCESS)
		{
			ShowErrMsg(m_dwCPUError);
			m_dwCPUError = ERROR_SUCCESS;
		}
		else
		{
			MessageBox(_T("Work Correctly!"));
		}

	}
	return CDialog::OnCommand(wParam, lParam);
}

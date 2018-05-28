
// SDKExampleDlg.h
//

#pragma once
#include "SDKDLL.h"
#include "afxwin.h"


// CSDKExampleDlg
class CSDKExampleDlg : public CDialog
{
public:
	CSDKExampleDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_SDKEXAMPLE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);


protected:
	HICON m_hIcon;

	COLOR_MATRIX	m_colorMatrix;

	void GetSysInfo();

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cbDevList;
	CButton m_DevSet;

	CComboBox m_cbEffectList;
	CComboBox m_cbRow;
	CComboBox m_cbColumn;

	CEdit m_edR;
	CEdit m_edG;
	CEdit m_edB;

	CEdit m_edR2;
	CEdit m_edG2;
	CEdit m_edB2;

	DWORD m_dwCPUError;


	bool	m_bChange;
	bool	m_bLedControl;

	BYTE GetColorValue(CEdit * pEdit);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCbnSelchangeKey();

	afx_msg void OnEnChangeEdit();
	afx_msg void OnBnClickedButtonSingleKey();
	afx_msg void OnBnClickedButtonAllKey();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonLedControl();
	afx_msg void OnBnClickedButtonEffectSet();
	afx_msg void OnBnClickedButtonSetOneColor();

	afx_msg void OnBnClickedButtonDeviceSet();
	afx_msg void OnBnClickedButtonGetLayout();
	afx_msg void OnBnClickedCheckKeyEff();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};


// EcAppDlg.h : 標頭檔
//

#pragma once
#include "afxwin.h"


// CEcAppDlg 對話方塊
class CEcAppDlg : public CDialogEx
{
// 建構
public:
	CEcAppDlg(CWnd* pParent = NULL);	// 標準建構函式

// 對話方塊資料
	enum { IDD = IDD_ECAPP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援


// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int nCtrlCode;
	int nRadioIndex;
	bool bInit;

	afx_msg void OnBnClickedBtnServoOn();
	afx_msg void OnBnClickedBtnServoOff();
	afx_msg void OnBnClickedBtnValue();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnGo();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedRadioCmd();
	afx_msg void OnBnClickedBtnHome();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();

	void Motion_CSP();
	void Motion_CSV();
	void Motion_PP();
	void Motion_PV();

	CEdit mDisCtrl;
	CEdit mAccCtrl;
	CEdit mDecCtrl;
	CEdit mSpeedCtrl;
	CEdit m_CtrlPosX;
	CEdit m_CtrlEncX;
	CEdit mCtrlControlDec;
	CEdit mCtrlControlHex;
	CEdit mCtrlStatusDec;
	CEdit mCtrlStatusHex;
	CEdit m_TargetPosCtrl;

	void GetPos();
	void GetEnc();
	void GetCiAStatus();
	void GetCiAControl();
	void GetTargetPos();

	CButton mBtnInit;
	CButton mBtnClose;
	CButton mGetvalueBtn;
	CButton mBtnServoOn;
	CButton mBtnServoOff;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnCoeUp();
	afx_msg void OnBnClickedBtnCoeDown();
	CEdit mCoeUpIndex;
	CEdit mCoeDownIndex;
	CEdit mUploadSub;
	CEdit mDownloadSub;
	CEdit mCoeUploadValue;
	CEdit mCoeDownValue;
	afx_msg void OnBnClickedBtnSetOut();
	CEdit nOutPutEdit;
	void GetRIO();
	void GetLIO();
	CEdit mInput;
	CEdit cHomeCtrlX;
	CEdit cOTPCtrlX;
	CEdit cOTNCtrlX;
	int nOTP;
	int nOTN;
};

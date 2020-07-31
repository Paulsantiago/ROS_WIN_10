
// EcAppDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "EcApp.h"
#include "EcAppDlg.h"
#include "afxdialogex.h"

#include "MCCL.h"
#include "MCCL_RTX.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CARD_INDEX 0
#define MAX_NUM_OF_DRIVERS 2
#define MAX_NUM_OF_GROUP 1

/* DS402 modes of operation 0x6060 */
#define DRV_MODE_OP_PROF_POS             1          /* profile position mode */
#define DRV_MODE_OP_VELOCITY             2          /* velocity mode (frequency converter) */
#define DRV_MODE_OP_PROF_VEL             3          /* profile velocity mode */
#define DRV_MODE_OP_PROF_TOR             4          /* profile torque mode */

#define DRV_MODE_OP_HOMING               6          /* homing mode */
#define DRV_MODE_OP_INTER_POS            7          /* interpolated position mode */
#define DRV_MODE_OP_CSP                  8          /* cyclic synchronous position mode */
#define DRV_MODE_OP_CSV                  9          /* cyclic synchronous velocity mode */
#define DRV_MODE_OP_CST                 10          /* cyclic synchronous torque   mode */


#define MODE_CSP 0
#define MODE_CSV 1
#define MODE_PP 2
#define MODE_PV 3

#define GET_DATA_TIMER		1
#define DEMO_TIMER		2
#define HOME_TIMER		3

#define DRV_OBJ_TARGET_POSITION             0x607A
#define DRV_OBJ_TARGET_VELOCITY             0x60FF
#define DRV_OBJ_PROFILE_ACC		            0x6083
#define DRV_OBJ_PROFILE_DEC					0x60FF
#define DRV_OBJ_PROFILE_VELOCITY            0x6081



int g_nGroupIndex0 = -1;
// 對 App About 使用 CAboutDlg 對話方塊

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 對話方塊資料
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

// 程式碼實作
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CEcAppDlg 對話方塊




CEcAppDlg::CEcAppDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEcAppDlg::IDD, pParent)
	, nRadioIndex(0)
	, nCtrlCode(0)
	, nOTP(0)
	, nOTN(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEcAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_CMD, nRadioIndex);
	DDX_Control(pDX, IDC_EDIT_DIS, mDisCtrl);
	DDX_Control(pDX, IDC_EDIT3, m_CtrlPosX);
	DDX_Control(pDX, IDC_EDIT2, m_CtrlEncX);
	DDX_Control(pDX, IDC_EDIT1, mCtrlControlDec);
	DDX_Control(pDX, IDC_EDIT4, mCtrlControlHex);
	DDX_Control(pDX, IDC_EDIT5, mCtrlStatusDec);
	DDX_Control(pDX, IDC_EDIT6, mCtrlStatusHex);
	DDX_Control(pDX, IDC_EDIT8, m_TargetPosCtrl);
	DDX_Control(pDX, IDC_EDIT_ACC, mAccCtrl);
	DDX_Control(pDX, IDC_EDIT_DEC, mDecCtrl);
	DDX_Control(pDX, IDC_EDIT_SPD, mSpeedCtrl);
	DDX_Control(pDX, IDC_BTN_START, mBtnInit);
	DDX_Control(pDX, IDC_BTN_CLOSE, mBtnClose);
	DDX_Control(pDX, IDC_BTN_VALUE, mGetvalueBtn);
	DDX_Control(pDX, IDC_BTN_SERVO_ON, mBtnServoOn);
	DDX_Control(pDX, IDC_BTN_SERVO_OFF, mBtnServoOff);
	DDX_Control(pDX, IDC_EDIT_INDEX_UP, mCoeUpIndex);
	DDX_Control(pDX, IDC_EDIT_INDEX_DOWN, mCoeDownIndex);
	DDX_Control(pDX, IDC_EDIT_SUB_UP, mUploadSub);
	DDX_Control(pDX, IDC_EDIT_SUB_DOWN, mDownloadSub);
	DDX_Control(pDX, IDC_EDIT_UP_VALUE, mCoeUploadValue);
	DDX_Control(pDX, IDC_EDIT_DOWN_VALUE, mCoeDownValue);
	DDX_Control(pDX, IDC_EDIT14, nOutPutEdit);
	DDX_Control(pDX, IDC_EDIT15, mInput);
	DDX_Control(pDX, IDC_EDIT18, cHomeCtrlX);
	DDX_Control(pDX, IDC_EDIT16, cOTPCtrlX);
	DDX_Control(pDX, IDC_EDIT17, cOTNCtrlX);
	DDX_Text(pDX, IDC_EDIT16, nOTP);
	DDX_Text(pDX, IDC_EDIT17, nOTN);
}

BEGIN_MESSAGE_MAP(CEcAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, &CEcAppDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_GO, &CEcAppDlg::OnBnClickedBtnGo)
	ON_BN_CLICKED(IDC_BTN_SERVO_ON, &CEcAppDlg::OnBnClickedBtnServoOn)
	ON_BN_CLICKED(IDC_BTN_SERVO_OFF, &CEcAppDlg::OnBnClickedBtnServoOff)
	ON_BN_CLICKED(IDC_BTN_VALUE, &CEcAppDlg::OnBnClickedBtnValue)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CEcAppDlg::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_STOP, &CEcAppDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_RADIO_CMD, &CEcAppDlg::OnBnClickedRadioCmd)
	ON_BN_CLICKED(IDC_BTN_HOME, &CEcAppDlg::OnBnClickedBtnHome)
	ON_BN_CLICKED(IDC_BUTTON2, &CEcAppDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDOK, &CEcAppDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_COE_UP, &CEcAppDlg::OnBnClickedBtnCoeUp)
	ON_BN_CLICKED(IDC_BTN_COE_DOWN, &CEcAppDlg::OnBnClickedBtnCoeDown)
	ON_BN_CLICKED(IDC_BTN_SET_OUT, &CEcAppDlg::OnBnClickedBtnSetOut)
END_MESSAGE_MAP()


// CEcAppDlg 訊息處理常式

BOOL CEcAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 將 [關於...] 功能表加入系統功能表。

	// IDM_ABOUTBOX 必須在系統命令範圍之中。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO: 在此加入額外的初始設定

	mDisCtrl.SetWindowTextA("10");
	mAccCtrl.SetWindowTextA("300");
	mDecCtrl.SetWindowTextA("300");
	mSpeedCtrl.SetWindowTextA("10");
	bInit = false;

	MCC_StartEcServer();
	Sleep(2000);
	int nRet = MCC_RtxInit(MAX_NUM_OF_DRIVERS);


	if (nRet != NO_ERROR)
	{
		if(nRet == ERROR_NO_DOG)
		{
			AfxMessageBox("Please Insert USB Key!",MB_OK);
		}
		else if(nRet == ERROR_RTX_INIT_ERROR)
		{

			AfxMessageBox("Not Found EcServer!",MB_OK);
			OnOK();
		}
	}
	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CEcAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CEcAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CEcAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CEcAppDlg::OnBnClickedBtnStart()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	int i = 0;
	int	 nRet;
	SYS_CARD_CONFIG    stCardConfig;
	SYS_MAC_PARAM      stMacParam;

	//nRet = MCC_RtxInit(MAX_NUM_OF_DRIVERS);
	MCC_SetSysMaxSpeed(100);

	stMacParam.wPosToEncoderDir          = 0;
	stMacParam.dwPPR			         = 10000;
	stMacParam.wRPM                      = 3000; 
	stMacParam.dfPitch                   = 1;
	stMacParam.dfGearRatio               = 1;
	stMacParam.dfHighLimit               = 50000.0;
	stMacParam.dfLowLimit                = -50000.0; 
	stMacParam.dfHighLimitOffset         = 0;
	stMacParam.dfLowLimitOffset          = 0;
	stMacParam.wPulseMode                = 0; 
	stMacParam.wPulseWidth               = 0; 
	stMacParam.wCommandMode              = 0;
	stMacParam.wOverTravelUpSensorMode   = SL_NORMAL_CLOSE;
	stMacParam.wOverTravelDownSensorMode = SL_NORMAL_CLOSE;


	for (WORD wChannel = 0;wChannel < MAX_NUM_OF_DRIVERS;wChannel++)
	{
		MCC_SetMacParam(&stMacParam, wChannel, CARD_INDEX);      //  mechanism parameters are the same for all axes
	}

	g_nGroupIndex0 = MCC_CreateGroup(0, 1, 2, -1, -1, -1, -1, -1, CARD_INDEX);

	//  stCardConfig is used to set card's base address and card style, and set one card's attributes now
	stCardConfig.wCardType    = EMP_MULTI_AXES; //7: EMP-S

	nRet = MCC_InitSystem(1, //  interpolation time interval
						&stCardConfig,
						0);
	

	if(nRet < 0)
	{
		AfxMessageBox("MCCL Init Error!");
		return ;
	}
	else
	{
		mBtnServoOn.EnableWindow(true);
		mGetvalueBtn.EnableWindow(true);
		mBtnClose.EnableWindow(true);
		mBtnInit.EnableWindow(false);
		GetDlgItem(IDC_RADIO_CMD)->EnableWindow(FALSE);

		bInit = true;

		Sleep(1000);

		if(nRet == 0)
		{
			//  set channel 0 servv on 
			UpdateData();
			switch(nRadioIndex)
			{
			case MODE_CSP:					
				break;
			}
		}
	}



}

void CEcAppDlg::Motion_CSP()
{
	CString nDis,nAcc,nDec,nSpd;
	
	mDisCtrl.GetWindowTextA(nDis);
	mAccCtrl.GetWindowTextA(nAcc);
	mDecCtrl.GetWindowTextA(nDec);
	mSpeedCtrl.GetWindowTextA(nSpd);

	for(int i = 0 ; i<MAX_NUM_OF_GROUP ; i++)
	{
		MCC_SetIncrease(i);

		MCC_SetAccTime(atoi(nAcc), i);//  set accleration time to be 50 ms
		 
		MCC_SetDecTime(atoi(nDec), i);//  set decleration time to be 50 ms

		MCC_SetFeedSpeed(atoi(nSpd), i);//  set line, arc and circle motion's feed rate (unit : mm/sec)
	}	

	MCC_SetAccType('T',g_nGroupIndex0); //set T curve
	MCC_SetDecType('T',g_nGroupIndex0); //set T curve
	
	MCC_LineX(atof(nDis), g_nGroupIndex0);

}

void CEcAppDlg::Motion_CSV()
{
	//double speed = 5000.0;
	CString nSpd;
	mSpeedCtrl.GetWindowTextA(nSpd);
	int nTargetVel = atoi(nSpd) *10000;

	MCC_EcatCoeSdoDownload(0, DRV_OBJ_TARGET_VELOCITY, 0, (BYTE*)&nTargetVel, sizeof(int));
	MCC_EcatCoeSdoDownload(1, DRV_OBJ_TARGET_VELOCITY, 0, (BYTE*)&nTargetVel, sizeof(int));
	MCC_EcatCoeSdoDownload(2, DRV_OBJ_TARGET_VELOCITY, 0, (BYTE*)&nTargetVel, sizeof(int));

}

void CEcAppDlg::Motion_PP()
{
	CString nOriginal;

	int nTargetPos = 0;

	CString nDis,nAcc,nDec,nSpd;
	
	mDisCtrl.GetWindowTextA(nDis);
	mAccCtrl.GetWindowTextA(nAcc);
	mDecCtrl.GetWindowTextA(nDec);
	mSpeedCtrl.GetWindowTextA(nSpd);

	m_CtrlEncX.GetWindowTextA(nOriginal);
	nTargetPos = (atoi(nDis)*10000 + atoi(nOriginal));

	int nSpeedValue = atoi(nSpd)*1000;
	int nAccValue = atoi(nAcc)*1000;
	int nDecValue = atoi(nDec)*1000;

	MCC_EcatCoeSdoDownload(0, DRV_OBJ_PROFILE_ACC, 0, (BYTE*)&nAccValue, sizeof(int));
	MCC_EcatCoeSdoDownload(0, DRV_OBJ_PROFILE_DEC, 0, (BYTE*)&nDecValue, sizeof(int));
	MCC_EcatCoeSdoDownload(0, DRV_OBJ_PROFILE_VELOCITY, 0, (BYTE*)&nSpeedValue, sizeof(int));
	
	MCC_EcatSetTargetPos(nTargetPos, 0);

	MCC_EcatStartAction(0);
}

void CEcAppDlg::Motion_PV()
{
	CString nAcc,nDec,nSpd;

	
	mAccCtrl.GetWindowTextA(nAcc);
	mDecCtrl.GetWindowTextA(nDec);
	mSpeedCtrl.GetWindowTextA(nSpd);
	
	int nSpeedValue = atoi(nSpd) * 1000;
	int nAccValue = atoi(nAcc) * 1000;
	int nDecValue = atoi(nDec) * 1000;

	MCC_EcatCoeSdoDownload(0, DRV_OBJ_PROFILE_ACC, 0, (BYTE*)&nAccValue, sizeof(int));
	MCC_EcatCoeSdoDownload(0, DRV_OBJ_PROFILE_DEC, 0, (BYTE*)&nDecValue, sizeof(int));	
	MCC_EcatCoeSdoDownload(0, DRV_OBJ_TARGET_VELOCITY, 0, (BYTE*)&nSpeedValue, sizeof(int));

}

void CEcAppDlg::OnBnClickedBtnGo()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	UpdateData(TRUE);
	switch(nRadioIndex)
	{
	case MODE_CSP:
		Motion_CSP();
		break;
	}


}


void CEcAppDlg::OnBnClickedBtnServoOn()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	for(int i = 0 ; i<MAX_NUM_OF_DRIVERS ; i++)
	{
		MCC_SetServoOn(i,0);
	}
	mBtnServoOn.EnableWindow(false);
	mBtnServoOff.EnableWindow(true);
}


void CEcAppDlg::OnBnClickedBtnServoOff()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	for(int i = 0 ; i<MAX_NUM_OF_DRIVERS ; i++)
	{
		MCC_SetServoOff(i,0);
	}
	mBtnServoOn.EnableWindow(true);
	mBtnServoOff.EnableWindow(false);
}


void CEcAppDlg::OnBnClickedBtnValue()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	SetTimer(GET_DATA_TIMER, 500, 0);
	mGetvalueBtn.EnableWindow(false);
}


void CEcAppDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	if(nIDEvent == GET_DATA_TIMER)
	{
		
		GetPos();
		GetEnc();
		//GetHome();
		GetCiAStatus();
		GetCiAControl();
		GetTargetPos();
		GetLIO();
		GetRIO();
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CEcAppDlg::GetPos()
{
	double dfCurPosX1 = 0, dfCurPosY1 = 0, dfCurPosZ1 = 0, dfCurPosU1 = 0, dfCurPosV1 = 0, dfCurPosW1 = 0, dfCurPosA1 = 0, dfCurPosB1 = 0;

	CString strX1;
	MCC_GetCurPos(&dfCurPosX1, &dfCurPosY1, &dfCurPosZ1, &dfCurPosU1, &dfCurPosV1, &dfCurPosW1, &dfCurPosA1, &dfCurPosB1, g_nGroupIndex0);
	strX1.Format( "%6.2f ",dfCurPosX1);
	m_CtrlPosX.SetWindowText(strX1);
}

void CEcAppDlg::GetEnc()
{

	long dfCurEncX = 0;

	CString strX;

	MCC_GetENCValue(&dfCurEncX, 0, CARD_INDEX);
	strX.Format("%8ld ",dfCurEncX);
	m_CtrlEncX.SetWindowText(strX);
}

void CEcAppDlg::OnBnClickedRadioCmd()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	mDisCtrl.EnableWindow(TRUE);
	mAccCtrl.EnableWindow(TRUE);
	mDecCtrl.EnableWindow(TRUE);
}


void CEcAppDlg::OnBnClickedRadio2()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	//if(bInit)
	//	MCC_EcatSetModeOfOp(DRV_MODE_OP_CSV, 0);

	mDisCtrl.EnableWindow(FALSE);
	mAccCtrl.EnableWindow(FALSE);
	mDecCtrl.EnableWindow(FALSE);
}


void CEcAppDlg::OnBnClickedRadio3()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	//if(bInit)
	//	MCC_EcatSetModeOfOp(DRV_MODE_OP_PROF_POS, 0);

	mDisCtrl.EnableWindow(TRUE);
	mAccCtrl.EnableWindow(TRUE);
	mDecCtrl.EnableWindow(TRUE);
	mSpeedCtrl.EnableWindow(TRUE);
}


void CEcAppDlg::OnBnClickedRadio4()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	//if(bInit)
	//	MCC_EcatSetModeOfOp(DRV_MODE_OP_PROF_VEL, 0);
	
	mSpeedCtrl.EnableWindow(TRUE);
	mAccCtrl.EnableWindow(TRUE);
	mDecCtrl.EnableWindow(TRUE);
	mDisCtrl.EnableWindow(FALSE);
}


void CEcAppDlg::OnClose()
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	if(bInit)
	{
		MCC_RtxClose();
		Sleep(1000);
	}
	CDialogEx::OnClose();
}


void CEcAppDlg::OnBnClickedBtnClose()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	KillTimer(GET_DATA_TIMER);

	MCC_CloseSystem();
	mBtnServoOff.EnableWindow(false);
	mBtnServoOn.EnableWindow(false);
	mGetvalueBtn.EnableWindow(false);
	mBtnClose.EnableWindow(false);
	mBtnInit.EnableWindow(true);

	GetDlgItem(IDC_RADIO_CMD)->EnableWindow(true);
}


void CEcAppDlg::OnBnClickedBtnStop()
{
	// TODO: 在此加入控制項告知處理常式程式碼

	if( (nRadioIndex == MODE_CSP) )
	{
		MCC_AbortMotionEx(5,0);
	}
	else if( (nRadioIndex == MODE_CSV) ||  (nRadioIndex == MODE_PV)  )
	{
		mSpeedCtrl.SetWindowTextA("0");
		OnBnClickedBtnGo();
	}
	else //PP
	{
		
	}

}




void CEcAppDlg::OnBnClickedBtnHome()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	for(int i = 0 ; i<MAX_NUM_OF_DRIVERS ; i++ )
	{
		MCC_EcatSetHomeMode(35,i);
		MCC_EcatSetHomeSwitchSpeed(5000,i);
		MCC_EcatSetHomeZeroSpeed(500,i);
	}

	MCC_EcatSetHomeAxis(1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
	MCC_EcatHome();
}

void CEcAppDlg::GetCiAStatus()
{

	WORD wCurStatusX = 0, wCurStatusY = 0 , wCurStatusZ = 0, wCurStatusU = 0, wCurStatusV = 0, wCurStatusW = 0 ,wCurStatusA = 0, wCurStatusB = 0 ;

	CString strX,strY,strZ,strU,strV,strW,strA,strB; 
	
	MCC_EcatGetStatusCode(&wCurStatusX, 0);

	strX.Format("%d",wCurStatusX);
	mCtrlStatusDec.SetWindowText(strX);

	strX.Format("0x%x",wCurStatusX);
	mCtrlStatusHex.SetWindowText(strX);
}

void CEcAppDlg::GetCiAControl()
{
	WORD wCurStatusX = 0, wCurStatusY = 0 , wCurStatusZ = 0, wCurStatusU = 0, wCurStatusV = 0, wCurStatusW = 0 ,wCurStatusA = 0, wCurStatusB = 0 ;

	CString strX,strY,strZ,strU,strV,strW,strA,strB; 
	
	MCC_EcatGetControlCode(&wCurStatusX, 0);

	strX.Format("%d",wCurStatusX);
	mCtrlControlDec.SetWindowText(strX);

	strX.Format("0x%x",wCurStatusX);
	mCtrlControlHex.SetWindowText(strX);
}

void CEcAppDlg::GetTargetPos()
{
	int nTarget = 0;
	CString strX;
	MCC_EcatGetTargetPos(&nTarget,0);

	strX.Format("%d",nTarget);
	m_TargetPosCtrl.SetWindowText(strX);

}

void CEcAppDlg::OnBnClickedButton1()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	this->UpdateData(TRUE);
	MCC_EcatSetControlCode(nCtrlCode, 0);
}


void CEcAppDlg::OnBnClickedButton2()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	CString nOriginal;
	CString nAdd;
	mDisCtrl.GetWindowTextA(nAdd);
	m_CtrlEncX.GetWindowTextA(nOriginal);
	//int nAdd = atoi(tmp);
	int nTargetPos = atoi(nAdd) + atoi(nOriginal);

	MCC_EcatSetTargetPos(nTargetPos, 0);
}


void CEcAppDlg::OnBnClickedOk()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	CDialogEx::OnOK();
}

unsigned long convertToDecimal(char hex[])
{
    char *hexString;
    int length = 0;
    const int base = 16; // Base of Hexadecimal Number
    unsigned long decimalNumber = 0;
    int i;
    // Find length of Hexadecimal Number
    for (hexString = hex; *hexString != '\0'; hexString++)
    {
        length++;
    }
    // Find Hexadecimal Number
    hexString = hex;
    for (i = 0; *hexString != '\0' && i < length; i++, hexString++)
    {
        // Compare *hexString with ASCII values
        if (*hexString >= 48 && *hexString <= 57)   // is *hexString Between 0-9
        {
            decimalNumber += (((int)(*hexString)) - 48) * pow((double)base,  length - i - 1 );
        }
        else if ((*hexString >= 65 && *hexString <= 70))   // is *hexString Between A-F
        {
            decimalNumber += (((int)(*hexString)) - 55) * pow((double)base, length - i - 1);
        }
        else if (*hexString >= 97 && *hexString <= 102)   // is *hexString Between a-f
        {
            decimalNumber += (((int)(*hexString)) - 87) * pow((double)base, length - i - 1);
        }
        else
        {
            printf(" Invalid Hexadecimal Number \n");
 
            printf(" Press enter to continue... \n");
            fflush(stdin);
            getchar();
            return 0;
            exit(0);
        }
    }
    return decimalNumber;
}

void CEcAppDlg::OnBnClickedBtnCoeUp()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	CString tmp;
	DWORD nData = 0;
	DWORD len = 0;
	CString nIndexTemp;
	CString nSubIndexTemp;
	char cHex[20];
	
	mCoeUpIndex.GetWindowTextA(nIndexTemp);

	LPTSTR lpsz = new TCHAR[nIndexTemp.GetLength()+1];
	_tcscpy(lpsz, nIndexTemp);

	WORD nIndex = convertToDecimal( lpsz);

	mUploadSub.GetWindowTextA(nSubIndexTemp);
	WORD nSubIndex = _ttol(nSubIndexTemp);

	MCC_EcatCoeSdoUpload(0, nIndex, nSubIndex, (BYTE*)&nData, sizeof(int), &len);
	tmp.Format("0x%X", nData);
	mCoeUploadValue.SetWindowTextA(tmp);

	delete[] lpsz;
}


void CEcAppDlg::OnBnClickedBtnCoeDown()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	CString nIndexTemp,nValueTemp,nSubIndexTemp;

	mCoeDownIndex.GetWindowTextA(nIndexTemp);
	LPTSTR lpsz = new TCHAR[nIndexTemp.GetLength()+1];
	_tcscpy(lpsz, nIndexTemp);
	WORD nIndex = convertToDecimal( lpsz);

	mCoeDownValue.GetWindowTextA(nValueTemp);
	WORD nValue = _ttol(nValueTemp);

	mDownloadSub.GetWindowTextA(nSubIndexTemp);
	WORD nSubIndex = _ttol(nSubIndexTemp);

	MCC_EcatCoeSdoDownload(0, nIndex, nSubIndex, (BYTE*)&nValue, sizeof(int));
}


void CEcAppDlg::OnBnClickedBtnSetOut()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	CString sOutput;
	nOutPutEdit.GetWindowTextA(sOutput);

	LPTSTR lpsz = new TCHAR[sOutput.GetLength()+1];
	_tcscpy(lpsz, sOutput);
	WORD nValue = convertToDecimal( lpsz);

	MCC_EcatSetOutput(3,nValue);
}

void CEcAppDlg::GetRIO()
{
	DWORD nData;
	MCC_EcatGetInput(3,&nData);
	CString tmp;
	tmp.Format("0x%X", nData);
	mInput.SetWindowTextA(tmp);
}

void CEcAppDlg::GetLIO()
{
	WORD status, statusOTP, statusOTN ;
	CString tmp;
	//HOME
	MCC_GetHomeSensorStatus(&status,0,0);
	tmp.Format("%d" , status);
	cHomeCtrlX.SetWindowText(tmp);

	//OTP
	MCC_GetLimitSwitchStatus(&status,1,0);
	tmp.Format("%d" , status);
	cOTPCtrlX.SetWindowText(tmp);
	
	//OTN
	MCC_GetLimitSwitchStatus(&status,0,0);
	tmp.Format("%d" , status);
	cOTNCtrlX.SetWindowText(tmp);
}
#ifndef _MCCL_RTX_H_
#define _MCCL_RTX_H_

#include "MCCL.h"

#ifdef __cplusplus
	extern "C" {
#endif


//////////////////////////////////////////////////////////////////////////////
// Definitions of Calling Convention & Storage-Class Attributes
#ifdef STATIC_LIB
	#define MCC_CALL
	#define MCC_API
#else
	#define MCC_CALL		__stdcall

	#ifdef MCCL_EXPORTS
		#define MCC_API		__declspec(dllexport)
	#else
		#define MCC_API		__declspec(dllimport)
	#endif // MCCL_EXPORTS
#endif // STATIC_LIB


//////////////////////////////////////////////////////////////////////////////
// System Management
		
// RTX functions
MCC_API BOOL MCC_CALL MCC_StartEcServer(int nSleepSec = 0);
MCC_API BOOL MCC_CALL MCC_StartEcServerEx(LPCTSTR lpEcServerPath, int nSleepSec = 0);
MCC_API int MCC_CALL MCC_RtxInit(int nAxis);
MCC_API int MCC_CALL MCC_RtxClose();

// Get Library Version
MCC_API void   MCC_CALL MCC_GetVersion(TCHAR *strVersion);

// Create/Close Motion Groups
MCC_API int    MCC_CALL MCC_CreateGroup(int xMapToCh, int yMapToCh, int zMapToCh, int uMapToCh, int vMapToCh, int wMapToCh, int aMapToCh, int bMapToCh, int nCardIndex);
MCC_API int    MCC_CALL MCC_CreateGroupEx(int xMapToCh, int yMapToCh, int zMapToCh, int uMapToCh, int vMapToCh, int wMapToCh, int aMapToCh, int bMapToCh, int nCardIndex, int nMotionQueueSize = MOTION_QUEUE_SIZE);
MCC_API int    MCC_CALL MCC_CloseGroup(int nGroupIndex);
MCC_API int    MCC_CALL MCC_CloseAllGroups();

// Set/Get Mechanism Parameters
MCC_API int    MCC_CALL MCC_SetMacParam(SYS_MAC_PARAM *pstMacParam, WORD wChannel, WORD wCardIndex = 0);
MCC_API int    MCC_CALL MCC_GetMacParam(SYS_MAC_PARAM *pstMacParam, WORD wChannel, WORD wCardIndex = 0);
MCC_API int    MCC_CALL MCC_UpdateParam();

// Get Size of Motion Command Queue
MCC_API int    MCC_CALL MCC_GetCmdQueueSize(WORD wGroupIndex = 0);

// Initialize/Close System
MCC_API int    MCC_CALL MCC_InitSystem(int nInterpolateTime, SYS_CARD_CONFIG *pstCardConfig, WORD wCardNo);
MCC_API int    MCC_CALL MCC_InitSystemEx(double dInterpolateTime, SYS_CARD_CONFIG *pstCardConfig, WORD wCardNo);
MCC_API int    MCC_CALL MCC_InitSimulation(int nInterpolateTime, SYS_CARD_CONFIG *pstCardConfig, WORD wCardNo);
MCC_API int    MCC_CALL MCC_CloseSystem();

// Reset MCCL
MCC_API int    MCC_CALL MCC_ResetMotion();

// Set/Get Max. Feed Speed
MCC_API int    MCC_CALL MCC_SetSysMaxSpeed(double dfMaxSpeed);
MCC_API double MCC_CALL MCC_GetSysMaxSpeed();


//////////////////////////////////////////////////////////////////////////////
// Coordinate Management

// Set/Get Coordinate Type
MCC_API int    MCC_CALL MCC_SetAbsolute(WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_SetIncrease(WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_GetCoordType(WORD wGroupIndex = 0);

// Get Current Position & Pulse Position
MCC_API int    MCC_CALL MCC_GetCurRefPos(double *pdfX, double *pdfY, double *pdfZ, double *pdfU, double *pdfV, double *pdfW, double *pdfA, double *pdfB, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_GetCurPos(double *pdfX, double *pdfY, double *pdfZ, double *pdfU, double *pdfV, double *pdfW, double *pdfA, double *pdfB, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_GetPulsePos(long *plX, long *plY, long *plZ, long *plU, long *plV, long *plW, long *plA, long *plB, WORD wGroupIndex = 0);

//Get Encoder value
MCC_API int    MCC_CALL MCC_GetENCValue(long *plValue, WORD wChannel, WORD wCardIndex = 0);

// Regard here as origin
MCC_API int    MCC_CALL MCC_DefineOrigin(WORD wAxis, WORD wGroupIndex = 0);

// Align command position with actual position
MCC_API int    MCC_CALL MCC_DefinePosHere(WORD wGroupIndex = 0, DWORD dwAxisMask = EMP_AXIS_ALL);

// Change command and actual positions according to specified value
MCC_API int    MCC_CALL MCC_DefinePos(WORD wAxis, double dfPos, WORD wGroupIndex = 0);


//////////////////////////////////////////////////////////////////////////////
// Software Over Travel Check & Hardware Limit Switch Check

// Enable/Disable Hardware Limit Switch Check
MCC_API int    MCC_CALL MCC_EnableLimitSwitchCheck(int nMode = 0);
MCC_API int    MCC_CALL MCC_DisableLimitSwitchCheck();

// Enable/Disable Software Over Travel Check
MCC_API int    MCC_CALL MCC_SetOverTravelCheck(int nOTCheck0, int nOTCheck1, int nOTCheck2, int nOTCheck3, int nOTCheck4, int nOTCheck5, int nOTCheck6, int nOTCheck7, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_GetOverTravelCheck(int *pnOTCheck0, int *pnOTCheck1, int *pnOTCheck2, int *pnOTCheck3, int *pnOTCheck4, int *pnOTCheck5, int *pnOTCheck6, int *pnOTCheck7, WORD wGroupIndex = 0);

// Get Limit Switch Sensor Signal
MCC_API int    MCC_CALL MCC_GetLimitSwitchStatus(WORD *pwStatus, WORD wUpDown, WORD wChannel, WORD wCardIndex = 0);


//////////////////////////////////////////////////////////////////////////////
// General Motions(Line, Arc, Circle, and Helical Motions)

// Set/Get Accleration & Deceleration Type
MCC_API int    MCC_CALL MCC_SetAccType(char cAccType, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_GetAccType(WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_SetDecType(char cDecType, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_GetDecType(WORD wGroupIndex = 0);

// Set/Get Accleration & Deceleration Time
MCC_API int    MCC_CALL MCC_SetAccTime(double dfAccTime, WORD wGroupIndex = 0);
MCC_API double MCC_CALL MCC_GetAccTime(WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_SetDecTime(double dfDecTime, WORD wGroupIndex = 0);
MCC_API double MCC_CALL MCC_GetDecTime(WORD wGroupIndex = 0);

// Set/Get Feed Speed
MCC_API double MCC_CALL MCC_SetFeedSpeed(double dfFeedSpeed, WORD wGroupIndex = 0);
MCC_API double MCC_CALL MCC_GetFeedSpeed(WORD wGroupIndex = 0);
MCC_API double MCC_CALL MCC_GetCurFeedSpeed(WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_GetSpeed(double *pdfVel0, double *pdfVel1, double *pdfVel2, double *pdfVel3, double *pdfVel4, double *pdfVel5, double *pdfVel6, double *pdfVel7, WORD wGroupIndex = 0);

// Linear Motion
MCC_API int    MCC_CALL MCC_Line(double dfX0, double dfX1, double dfX2, double dfX3, double dfX4, double dfX5, double dfX6, double dfX7, WORD wGroupIndex = 0, DWORD dwAxisMask = EMP_AXIS_ALL);

// Arc Motion
MCC_API int    MCC_CALL MCC_ArcXYZ(double dfRX0, double dfRX1, double dfRX2, double dfX0, double dfX1, double dfX2, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_ArcXY(double dfRX0, double dfRX1, double dfX0, double dfX1, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_ArcYZ(double dfRX1, double dfRX2, double dfX1, double dfX2, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_ArcZX(double dfRX2, double dfRX0, double dfX2, double dfX0, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_ArcXYZ_Aux(double dfRX0, double dfRX1, double dfRX2, double dfX0, double dfX1, double dfX2, double dfX3, double dfX4, double dfX5, double dfX6, double dfX7, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_ArcXY_Aux(double dfRX0, double dfRX1, double dfX0, double dfX1, double dfX3, double dfX4, double dfX5, double dfX6, double dfX7, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_ArcYZ_Aux(double dfRX1, double dfRX2, double dfX1, double dfX2, double dfX3, double dfX4, double dfX5, double dfX6, double dfX7, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_ArcZX_Aux(double dfRX2, double dfRX0, double dfX2, double dfX0, double dfX3, double dfX4, double dfX5, double dfX6, double dfX7, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_ArcThetaXY(double dfCX, double dfCY, double dfTheta, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_ArcThetaYZ(double dfCY, double dfCZ, double dfTheta, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_ArcThetaZX(double dfCZ, double dfCX, double dfTheta, WORD wGroupIndex = 0);

// Circular Motion
MCC_API int    MCC_CALL MCC_CircleXY(double dfCX, double dfCY, BYTE byCirDir, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_CircleYZ(double dfCY, double dfCZ, BYTE byCirDir, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_CircleZX(double dfCZ, double dfCX, BYTE byCirDir, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_CircleXY_Aux(double dfCX, double dfCY, double dfU, double dfV, double dfW, double dfA, double dfB, BYTE byCirDir, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_CircleYZ_Aux(double dfCY, double dfCZ, double dfU, double dfV, double dfW, double dfA, double dfB, BYTE byCirDir, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_CircleZX_Aux(double dfCZ, double dfCX, double dfU, double dfV, double dfW, double dfA, double dfB, BYTE byCirDir, WORD wGroupIndex = 0);

// Helical Motion
MCC_API int    MCC_CALL MCC_HelicalXY_Z( double dfCX, double dfCY, double dfPitch, double dfTheta, WORD   wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_HelicalYZ_X( double dfCY, double dfCZ, double dfPitch, double dfTheta, WORD   wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_HelicalZX_Y( double dfCZ, double dfCX, double dfPitch, double dfTheta, WORD   wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_HelicalXY_Z_Aux( double dfCX, double dfCY, double dfPitch, double dfU, double dfV, double dfW, double dfA, double dfB, double dfTheta, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_HelicalYZ_X_Aux( double dfCY, double dfCZ, double dfPitch, double dfU, double dfV, double dfW, double dfA, double dfB, double dfTheta, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_HelicalZX_Y_Aux( double dfCZ, double dfCX, double dfPitch, double dfU, double dfV, double dfW, double dfA, double dfB, double dfTheta, WORD wGroupIndex = 0);


//////////////////////////////////////////////////////////////////////////////
// Jog Motion

MCC_API int    MCC_CALL MCC_JogPulse(int nPulse, char cAxis, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_JogSpace(double dfOffset, double dfRatio, char cAxis, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_JogConti(int nDir, double dfRatio, char cAxis, WORD wGroupIndex = 0);


//////////////////////////////////////////////////////////////////////////////
// Point-to-Point Motion

// Set/Get Point-to-Point Motion  Accleration & Deceleration Type
MCC_API int    MCC_CALL MCC_SetPtPAccType(char cAccType0, char cAccType1, char cAccType2, char cAccType3, char cAccType4, char cAccType5, char cAccType6, char cAccType7,  WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_GetPtPAccType(char *pcAccType0, char *pcAccType1, char *pcAccType2, char *pcAccType3, char *pcAccType4, char *pcAccType5, char *pcAccType6, char *pcAccType7, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_SetPtPDecType(char cDecType0, char cDecType1, char cDecType2, char cDecType3, char cDecType4, char cDecType5, char cDecType6, char cDecType7, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_GetPtPDecType(char *pcDecType0, char *pcDecType1, char *pcDecType2, char *pcDecType3, char *pcDecType4, char *pcDecType5, char *pcDecType6, char *pcDecType7, WORD wGroupIndex = 0);

// Set/Get Point-to-Point Motion  Accleration & Deceleration Time
MCC_API int    MCC_CALL MCC_SetPtPAccTime(double dfAccTime0, double dfAccTime1, double dfAccTime2, double dfAccTime3, double dfAccTime4, double dfAccTime5, double dfAccTime6, double dfAccTime7, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_GetPtPAccTime(double *pdfAccTime0, double *pdfAccTime1, double *pdfAccTime2, double *pdfAccTime3, double *pdfAccTime4, double *pdfAccTime5, double *pdfAccTime6, double *pdfAccTime7, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_SetPtPDecTime(double dfDecTime0, double dfDecTime1, double dfDecTime2, double dfDecTime3, double dfDecTime4, double dfDecTime5, double dfDecTime6, double dfDecTime7, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_GetPtPDecTime(double *pdfDecTime0, double *pdfDecTime1, double *pdfDecTime2, double *pdfDecTime3, double *pdfDecTime4, double *pdfDecTime5, double *pdfDecTime6, double *pdfDecTime7, WORD wGroupIndex = 0);

// Set/Get Point-to-Point Motion Speed Ratio
MCC_API double MCC_CALL MCC_SetPtPSpeed(double dfRatio, WORD wGroupIndex = 0);
MCC_API double MCC_CALL MCC_GetPtPSpeed(WORD wGroupIndex = 0);

// Point-to-Point Motion
MCC_API int    MCC_CALL MCC_PtP(double dfX0, double dfX1, double dfX2, double dfX3, double dfX4, double dfX5, double dfX6, double dfX7, WORD wGroupIndex = 0, DWORD dwAxisMask = EMP_AXIS_ALL);

MCC_API int    MCC_CALL MCC_PtPX(double dfX, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_PtPY(double dfY, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_PtPZ(double dfZ, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_PtPU(double dfU, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_PtPV(double dfV, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_PtPW(double dfW, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_PtPA(double dfA, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_PtPB(double dfB, WORD wGroupIndex = 0);


//////////////////////////////////////////////////////////////////////////////
// Motion Status

// Get Current Motion Status
MCC_API int    MCC_CALL MCC_GetMotionStatus(WORD wGroupIndex = 0);

// Get Current Executing Motion Command
MCC_API int    MCC_CALL MCC_GetCurCommand(COMMAND_INFO *pstCurCommand, WORD wGroupIndex = 0);

// Get/Reset Motion Command Stock Count
MCC_API int    MCC_CALL MCC_GetCommandCount(int *pnCmdCount, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_ResetCommandIndex(WORD wGroupIndex = 0);

// Set/Get Hardware Pulse Stock Count
MCC_API int    MCC_CALL MCC_SetMaxPulseStockNum(int nMaxStockNum);
MCC_API int    MCC_CALL MCC_GetMaxPulseStockNum();
MCC_API int    MCC_CALL MCC_GetCurPulseStockCount(WORD *pwStockCount, WORD wChannel, WORD wCardIndex = 0);

// Get/Clear Error Code
MCC_API int    MCC_CALL MCC_GetErrorCode(WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_ClearError(WORD wGroupIndex = 0);


//////////////////////////////////////////////////////////////////////////////
// Position Control

// Set Compensation Table
MCC_API int    MCC_CALL MCC_SetCompParam(SYS_COMP_PARAM *pstCompParam, WORD wChannel, WORD wCardIndex = 0);
MCC_API int    MCC_CALL MCC_UpdateCompParam();

// Set/Get Maximum Pulse Speed & Accleration
MCC_API int    MCC_CALL MCC_SetMaxPulseSpeed(int nPulse0, int nPulse1, int nPulse2, int nPulse3, int nPulse4, int nPulse5, int nPulse6, int nPulse7, WORD wCardIndex = 0);
MCC_API int    MCC_CALL MCC_GetMaxPulseSpeed(int *pnSpeed0, int *pnSpeed1, int *pnSpeed2, int *pnSpeed3, int *pnSpeed4, int *pnSpeed5, int *pnSpeed6, int *pnSpeed7, WORD wCardIndex = 0);
MCC_API int    MCC_CALL MCC_SetMaxPulseAcc(int nPulseAcc0, int nPulseAcc1, int nPulseAcc2, int nPulseAcc3, int nPulseAcc4, int nPulseAcc5, int nPulseAcc6, int nPulseAcc7, WORD wCardIndex = 0);
MCC_API int    MCC_CALL MCC_GetMaxPulseAcc(int *pnPulseAcc0, int *pnPulseAcc1, int *pnPulseAcc2, int *pnPulseAcc3, int *pnPulseAcc4, int *pnPulseAcc5, int *pnPulseAcc6, int *pnPulseAcc7, WORD wCardIndex = 0);


//////////////////////////////////////////////////////////////////////////////
// Trajectory Planning

// Hold/Continue/Abort Motion
MCC_API int    MCC_CALL MCC_HoldMotion(WORD wGroupIndex = 0, BOOL bAfterCurCmd = 0);
MCC_API int    MCC_CALL MCC_ContiMotion(WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_AbortMotionEx(double dfDecTime, WORD wGroupIndex = 0);

// Enable/Disable Motion Blending
MCC_API int    MCC_CALL MCC_EnableBlend(WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_DisableBlend(WORD wGroupIndex = 0); 
MCC_API int    MCC_CALL MCC_CheckBlend(WORD wGroupIndex = 0);

// Set Delay Time
MCC_API int    MCC_CALL MCC_DelayMotion(DWORD dwTime, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_CheckDelay(WORD wGroupIndex = 0);
MCC_API void   MCC_CALL MCC_TimeDelay(DWORD dwTime);

// Set/Get Over-Speed Ratio for General Motions
MCC_API double MCC_CALL MCC_OverrideSpeed(double dfRate, WORD wGroupIndex = 0);
MCC_API double MCC_CALL MCC_OverrideSpeedEx(double dfRate, BOOL bInstant = 1, WORD wGroupIndex = 0);
MCC_API double MCC_CALL MCC_GetOverrideRate(WORD wGroupIndex = 0);


//////////////////////////////////////////////////////////////////////////////
// EtherCAT System function

// Servo On/Off
MCC_API int    MCC_CALL MCC_SetServoOn(WORD wChannel, WORD wCardIndex = 0);
MCC_API int    MCC_CALL MCC_SetServoOff(WORD wChannel, WORD wCardIndex = 0);

//Watch Dog
MCC_API BOOL MCC_CALL MCC_EcatEnableWatchDog(int nExpireTime);
MCC_API BOOL MCC_CALL MCC_EcatDisableWatchDog();

//EtherCAT I/O
MCC_API int  MCC_CALL MCC_EcatGetInput(DWORD dwSlaveId, DWORD *pdwInData);
MCC_API int  MCC_CALL MCC_EcatGetOutput(DWORD dwSlaveId, DWORD *pdwOutData);
MCC_API int  MCC_CALL MCC_EcatSetOutput(DWORD dwSlaveId, DWORD dwOutData);
MCC_API int  MCC_CALL MCC_EcatSetOutputEnqueue(DWORD dwSlaveId, DWORD dwOutData, WORD wGroupIndex = 0);

//EtherCAT SDO
MCC_API int  MCC_CALL MCC_EcatCoeSdoDownload(DWORD dwSlaveId, WORD wObIndex, BYTE byObSubIndex , BYTE* pbyData, DWORD dwDataLen);
MCC_API int  MCC_CALL MCC_EcatCoeSdoUpload(DWORD dwSlaveId, WORD wObIndex, BYTE byObSubIndex, BYTE* pbyData, DWORD dwDataLen, DWORD* pdwOutDataLen);

//EtherCAT Error Code
MCC_API WORD MCC_CALL MCC_EcatGetErrorCode(int *pnType, WORD *pwCode,  WORD wChannel);

//EtherCAT Reset Fault
MCC_API WORD MCC_CALL MCC_EcatFaultReset(DWORD dwSlaveId);


//////////////////////////////////////////////////////////////////////////////
// EtherCAT Home function

MCC_API int  MCC_CALL MCC_EcatSetHomeAxis(BYTE byAxisX, BYTE byAxisY, BYTE byAxisZ, BYTE byAxisU, BYTE byAxisV, BYTE byAxisW, BYTE byAxisA, BYTE byAxisB,
																							  BYTE byAxisX1, BYTE byAxisY1, BYTE byAxisZ1, BYTE byAxisU1, BYTE byAxisV1, BYTE byAxisW1, BYTE byAxisA1, BYTE byAxisB1);
MCC_API int  MCC_CALL MCC_EcatSetHomeMode(int nMode, int nChannel);
MCC_API int  MCC_CALL MCC_EcatSetHomeZeroSpeed(int nZeroSpeed, int nChannel);
MCC_API int  MCC_CALL MCC_EcatSetHomeSwitchSpeed(int nSwitchSpeed, int nChannel);
MCC_API int  MCC_CALL MCC_EcatHome();
MCC_API BOOL MCC_CALL MCC_EcatGetGoHomeStatus();
MCC_API BOOL MCC_CALL MCC_EcatAbortHome();

// Get Home Sensor Signal
MCC_API int    MCC_CALL MCC_GetHomeSensorStatus(WORD *pwStatus, WORD wChannel, WORD wCardIndex = 0);


//////////////////////////////////////////////////////////////////////////////
// Obsolete functions in earlier version of MCCL (existed only for
// compatibility and should not be used anymore)

// Set/Get configuration of the homing process
MCC_API int    MCC_CALL MCC_SetHomeConfig(SYS_HOME_CONFIG *pstHomeConfig, WORD wChannel, WORD wCardIndex = 0);
MCC_API int    MCC_CALL MCC_GetHomeConfig(SYS_HOME_CONFIG *pstHomeConfig, WORD wChannel, WORD wCardIndex = 0);
MCC_API int    MCC_CALL MCC_SetEncoderConfig(SYS_ENCODER_CONFIG *pstEncoderConfig, WORD wChannel, WORD wCardIndex = 0);
MCC_API int    MCC_CALL MCC_GetEncoderConfig(SYS_ENCODER_CONFIG *pstEncoderConfig, WORD wChannel, WORD wCardIndex = 0);

MCC_API int    MCC_CALL MCC_LineX(double dfX, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_LineY(double dfY, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_LineZ(double dfZ, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_LineU(double dfU, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_LineV(double dfV, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_LineW(double dfW, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_LineA(double dfA, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_LineB(double dfB, WORD wGroupIndex = 0);

MCC_API int    MCC_CALL MCC_AbortMotion(WORD wGroupIndex = 0, BOOL bAfterCurCmd = 0);

// Helica motion
MCC_API int    MCC_CALL MCC_HelicaXY_Z(double dfCX, double dfCY, double dfPos, double dfPitch, BYTE byCirDir, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_HelicaYZ_X(double dfCY, double dfCZ, double dfPos, double dfPitch, BYTE byCirDir, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_HelicaZX_Y(double dfCZ, double dfCX, double dfPos, double dfPitch, BYTE byCirDir, WORD wGroupIndex = 0);

MCC_API int    MCC_CALL MCC_ConeXY_Z(double dfCX, double dfCY, double dfPos, double dfPitch, double CenterRate, BYTE byCirDir, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_ConeYZ_X(double dfCY, double dfCZ, double dfPos, double dfPitch, double CenterRate, BYTE byCirDir, WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_ConeZX_Y(double dfCZ, double dfCX, double dfPos, double dfPitch, double CenterRate, BYTE byCirDir, WORD wGroupIndex = 0);

MCC_API double MCC_CALL MCC_ChangeFeedSpeed(double dfSpeed, WORD wGroupIndex = 0);
MCC_API double MCC_CALL MCC_SetOverSpeed(double dfRate, WORD wGroupIndex = 0);
MCC_API double MCC_CALL MCC_GetOverSpeed(WORD wGroupIndex = 0);
MCC_API int    MCC_CALL MCC_FixSpeed(BOOL bFix, WORD wGroupIndex = 0);


MCC_API int    MCC_CALL MCC_EnableAccDecAfterIPO(WORD wGroupIndex);
MCC_API int    MCC_CALL MCC_DisableAccDecAfterIPO(WORD wGroupIndex);

MCC_API int    MCC_CALL MCC_EnableMovingAverage(WORD wGroupIndex);
MCC_API int    MCC_CALL MCC_DisableMovingAverage(WORD wGroupIndex);


MCC_API int  MCC_CALL MCC_EcatSetControlCode(WORD wControlCode, int nChannel);
MCC_API int  MCC_CALL MCC_EcatGetStatusCode(WORD *pwStatusCode, int nChannel);
MCC_API int  MCC_CALL MCC_EcatGetControlCode(WORD *pwControlCode, int nChannel);
MCC_API int  MCC_CALL MCC_EcatSetTargetPos(int nTargetPos, int nChannel);
MCC_API int  MCC_CALL MCC_EcatStartAction(DWORD dwSlaveId);
MCC_API int  MCC_CALL MCC_EcatGetTargetPos(int *pnTargetPos, DWORD dwSlaveId);
MCC_API BOOL MCC_CALL MCC_EcatSetInverterPowerOn(BOOL bEnable);//for HS


//////////////////////////////////////////////////////////////////////////////
// Robot Func.
// Customize kinematics transformation rules

// Point-to-Point Motion
MCC_API int    MCC_CALL MCC_PtP_V6(
							double j0,
							double j1,
							double j2,
							double j3,
							double j4,
							double j5,
							double j6,
							double j7,
							//void* pvBuffer,
							//BOOL bOverrideMotion = FALSE,
							WORD wGroupIndex = 0,
							DWORD dwAxisMask = EMP_AXIS_ALL
							);

// Linear Motion
MCC_API int    MCC_CALL MCC_Line_V6(
							double x,
							double y,
							double z,
							double rx,
							double ry,
							double rz,
							BOOL bOverrideMotion = 0,
							WORD wGroupIndex = 0,
							DWORD dwAxisMask = EMP_AXIS_ALL
							);

// Circular Motion
MCC_API int    MCC_CALL MCC_Arc_V6(
							double x0, // ref. point for x axis
							double y0, // ref. point for y axis
							double z0, // ref. point for z axis
							double x1, // target point for x axis
							double y1, // target point for y axis
							double z1, // target point for z axis
							double rx, // target point for x orientation
							double ry, // target point for y orientation
							double rz, // target point for z orientation
							DWORD posture = 0,
							WORD wGroupIndex = 0,
							DWORD dwAxisMask = EMP_AXIS_ALL
							);

MCC_API int    MCC_CALL MCC_ArcTheta_V6(
							double cx,    // center point for x axis
							double cy,    // center point for y axis
							double cz,    // center point for z axis
							double nv0,   // normal vector for x direction
							double nv1,   // normal vector for y direction
							double nv2,   // normal vector for z direction
							double theta, // degree, +/- stands for direction
							double rx,    // target point for x orientation
							double ry,    // target point for y orientation
							double rz,    // target point for z orientation
							DWORD posture = 0,
							WORD wGroupIndex = 0,
							DWORD dwAxisMask = EMP_AXIS_ALL
							);

MCC_API int    MCC_CALL MCC_CircleXY_V6(
							double cx, // center point for x axis
							double cy, // center point for y axis
							double rx, // target point for x orientation
							double ry, // target point for y orientation
							double rz, // target point for z orientation
							BYTE byCirDir, // CW or CCW
							DWORD posture = 0,
							WORD wGroupIndex = 0);

MCC_API int    MCC_CALL MCC_CircleYZ_V6(
							double cy, // center point for y axis
							double cz, // center point for z axis
							double rx, // target point for x orientation
							double ry, // target point for y orientation
							double rz, // target point for z orientation
							BYTE byCirDir, // CW or CCW
							DWORD posture = 0,
							WORD wGroupIndex = 0);

MCC_API int    MCC_CALL MCC_CircleZX_V6(
							double cz, // center point for z axis
							double cx, // center point for x axis
							double rx, // target point for x orientation
							double ry, // target point for y orientation
							double rz, // target point for z orientation
							BYTE byCirDir, // CW or CCW
							DWORD posture = 0,
							WORD wGroupIndex = 0);

MCC_API int    MCC_CALL MCC_Circle_V6(
							double x0, // 1st ref. point for x axis
							double y0, // 1st ref. point for y axis
							double z0, // 1st ref. point for z axis
							double x1, // 2nd ref. point for x axis
							double y1, // 2nd ref. point for y axis
							double z1, // 2nd ref. point for z axis
							double rx, // target point for x orientation
							double ry, // target point for y orientation
							double rz, // target point for z orientation
							DWORD posture = 0,
							WORD wGroupIndex = 0,
							DWORD dwAxisMask = EMP_AXIS_ALL
							);


#ifdef __cplusplus
	}
#endif


#endif // _MCCL_FUN_H_
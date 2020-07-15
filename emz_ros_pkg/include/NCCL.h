// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the NCCL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// NCCL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.


#ifndef __NCCL_H__
#define __NCCL_H__

#ifdef __cplusplus
	extern "C" {
#endif


#ifdef STATIC_LIB
	#define MCC_CALL 
	#define MCC_API
#else
	#define MCC_CALL		__stdcall
	#ifdef NCCL_DLL_EXPORTS
		#ifdef _x64
			#define MCC_API	//for x64	
		#else
			#define MCC_API		__declspec(dllexport) //for x86
		#endif 		
	#else
		#define MCC_API		__declspec(dllimport)
	#endif // NCCL_DLL_EXPORTS
#endif // STATIC_LIB

// The class is exported from NCCL.dll/NCCL64.dll

/////////////////////////////////////////////////////////////////////

// NCC
MCC_API	int MCC_CALL MCC_Init(char * pszIP, int *nCardIndex, int nSlaveCount);
MCC_API	int MCC_CALL MCC_Close(void);

// Create/Close motion groups
MCC_API int MCC_CALL MCC_CreateGroup(int xMapToCh, int yMapToCh, int zMapToCh, int uMapToCh, int vMapToCh, int wMapToCh, int aMapToCh, int bMapToCh,  int nCardIndex);
MCC_API int MCC_CALL MCC_CreateGroupEx(int xMapToCh, int yMapToCh, int zMapToCh, int uMapToCh, int vMapToCh, int wMapToCh, int aMapToCh, int bMapToCh, int nCardIndex, int nMotionQueueSize = MOTION_QUEUE_SIZE);
MCC_API	int MCC_CALL MCC_CloseGroup(int nGroupIndex);
MCC_API	int MCC_CALL MCC_CloseAllGroups();

// Set/Get mechanism parameters
MCC_API	int	MCC_CALL MCC_SetMacParam(SYS_MAC_PARAM *pstMacParam, WORD wChannel, WORD wCardIndex = 0);
MCC_API	int MCC_CALL MCC_GetMacParam(SYS_MAC_PARAM *pstMacParam, WORD wChannel, WORD wCardIndex = 0);
MCC_API	int MCC_CALL MCC_SetEncoderConfig(SYS_ENCODER_CONFIG *pstEncoderConfig, WORD wChannel, WORD wCardIndex = 0);
MCC_API	int MCC_CALL MCC_GetEncoderConfig(SYS_ENCODER_CONFIG *pstEncoderConfig, WORD wChannel, WORD wCardIndex = 0);
MCC_API	int MCC_CALL MCC_UpdateParam();

// Set/Get size of motion command queue
MCC_API	int MCC_CALL MCC_SetCmdQueueSize(int nSize, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_GetCmdQueueSize(WORD wGroupIndex = 0);

// Initialize/Close system
MCC_API	int MCC_CALL MCC_InitSystem(int nInterpolateTime,SYS_CARD_CONFIG *pstCardConfig, WORD wCardNo);
MCC_API	int MCC_CALL MCC_InitSystemEx(double nInterpolateTime, SYS_CARD_CONFIG *pstCardConfig, WORD wCardNo);
MCC_API int MCC_CALL MCC_InitSimulation(int nInterpolateTime, SYS_CARD_CONFIG *pstCardConfig, WORD wCardNo);
MCC_API	int MCC_CALL MCC_CloseSystem();

// Reset MCCL
MCC_API	int MCC_CALL MCC_ResetMotion();

// Enable/Disable dry-run
MCC_API	int MCC_CALL MCC_EnableDryRun();
MCC_API	int MCC_CALL MCC_DisableDryRun();
MCC_API	int MCC_CALL MCC_CheckDryRun();

// Set/Get maximum feed speed
MCC_API	int MCC_CALL MCC_SetSysMaxSpeed(double dfMaxSpeed);
MCC_API	double MCC_CALL MCC_GetSysMaxSpeed();

//////////////////////////////////////////////////////////////////////////////
// Local I/O Control

// Servo On/Off
MCC_API	int MCC_CALL MCC_SetServoOn(WORD wChannel, WORD wCardIndex = 0);
MCC_API	int MCC_CALL MCC_SetServoOff(WORD wChannel, WORD wCardIndex = 0);

// Get signal state of the emergency stop
MCC_API	int MCC_CALL MCC_GetEmgcStopStatus(WORD *pwStatus, WORD wCardIndex = 0);


//////////////////////////////////////////////////////////////////////////////
// Coordinate Management


// Set/Get coordinate type
MCC_API	int MCC_CALL MCC_SetAbsolute(WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_SetIncrease(WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_GetCoordType(WORD wGroupIndex = 0);

// Get current position & pulse position
MCC_API	int MCC_CALL MCC_GetCurRefPos(double *pdfX, double *pdfY, double *pdfZ, double *pdfU, double *pdfV, double *pdfW, double *pdfA, double *pdfB, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_GetCurPos(double *pdfX, double *pdfY, double *pdfZ, double *pdfU, double *pdfV, double *pdfW, double *pdfA, double *pdfB, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_GetPulsePos(long *plX, long *plY, long *plZ, long *plU, long *plV, long *plW, long *plA, long *plB, WORD wGroupIndex = 0);

// Regard here as origin
MCC_API	int MCC_CALL MCC_DefineOrigin(WORD wAxis, WORD wGroupIndex = 0);

// Change command and actual positions according to specified value
MCC_API	int MCC_CALL MCC_DefinePos(WORD wAxis, double dfPos, WORD wGroupIndex = 0);

// Align command position with actual position
MCC_API	int MCC_CALL MCC_DefinePosHere(WORD wGroupIndex = 0, DWORD dwAxisMask = IMP_AXIS_ALL);


//////////////////////////////////////////////////////////////////////////////
// Software Over Travel Check & Hardware Limit Protection

// Enable/Disable hardware limit protection
MCC_API	int MCC_CALL MCC_EnableLimitSwitchCheck(int nMode = 0);
MCC_API	int MCC_CALL MCC_DisableLimitSwitchCheck();

// Enable/Disable software over-travel check
MCC_API	int MCC_CALL MCC_SetOverTravelCheck(int nOTCheck0, int nOTCheck1, int nOTCheck2, int nOTCheck3, int nOTCheck4, int nOTCheck5, int nOTCheck6, int nOTCheck7, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_GetOverTravelCheck(int *pnOTCheck0, int *pnOTCheck1, int *pnOTCheck2, int *pnOTCheck3, int *pnOTCheck4, int *pnOTCheck5, int *pnOTCheck6, int *pnOTCheck7, WORD wGroupIndex = 0);

// Get signal state of the limit switches
MCC_API	int MCC_CALL MCC_GetLimitSwitchStatus(WORD *pwStatus, WORD wUpDown, WORD wChannel, WORD wCardIndex = 0);

//////////////////////////////////////////////////////////////////////////////
// General Motions(Line, Arc, Circle, Helical Motions)

// Set/Get acceleration & deceleration types
MCC_API	int MCC_CALL MCC_SetAccType(char cAccType, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_GetAccType(WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_SetDecType(char cDecType, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_GetDecType(WORD wGroupIndex = 0);

// Set/Get acceleration & deceleration times
MCC_API	int		MCC_CALL MCC_SetAccTime(double dfAccTime, WORD wGroupIndex = 0);
MCC_API	double	MCC_CALL MCC_GetAccTime(WORD wGroupIndex = 0);
MCC_API	int		MCC_CALL MCC_SetDecTime(double dfDecTime, WORD wGroupIndex = 0);
MCC_API	double	MCC_CALL MCC_GetDecTime(WORD wGroupIndex = 0);

// Set/Get feed speed
MCC_API double	MCC_CALL MCC_SetFeedSpeed(double dfFeedSpeed, WORD wGroupIndex = 0);
MCC_API	double	MCC_CALL MCC_GetFeedSpeed(WORD wGroupIndex = 0);
MCC_API	double	MCC_CALL MCC_GetCurFeedSpeed(WORD wGroupIndex = 0);
MCC_API	int		MCC_CALL MCC_GetSpeed(double *pdfVel0, double *pdfVel1, double *pdfVel2, double *pdfVel3, double *pdfVel4, double *pdfVel5, double *pdfVel6, double *pdfVel7, WORD wGroupIndex = 0);

// Set/Get speed ratio
MCC_API	double MCC_CALL MCC_SetPtPSpeed(double dfRatio, WORD wGroupIndex = 0);
MCC_API	double MCC_CALL MCC_GetPtPSpeed(WORD wGroupIndex = 0);

// Point-to-Point motion
MCC_API	int MCC_CALL MCC_PtP(double dfX0, double dfX1, double dfX2, double dfX3, double dfX4, double dfX5, double dfX6, double dfX7, WORD wGroupIndex = 0, DWORD dwAxisMask = IMP_AXIS_ALL);

// Linear motion
MCC_API	int MCC_CALL MCC_Line(double dfX0, double dfX1, double dfX2, double dfX3, double dfX4, double dfX5, double dfX6, double dfX7, WORD wGroupIndex = 0, DWORD dwAxiesMask = IMP_AXIS_ALL);

// Arc motion
MCC_API	int MCC_CALL MCC_ArcXYZ(double dfRX0, double dfRX1, double dfRX2, double dfX0, double dfX1, double dfX2, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_ArcXY(double dfRX0, double dfRX1, double dfX0, double dfX1, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_ArcYZ(double dfRX1, double dfRX2, double dfX1, double dfX2, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_ArcZX(double dfRX2, double dfRX0, double dfX2, double dfX0, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_ArcXYZ_Aux(double dfRX0, double dfRX1, double dfRX2, double dfX0, double dfX1, double dfX2, double dfX3, double dfX4, double dfX5, double dfX6, double dfX7, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_ArcXY_Aux(double dfRX0, double dfRX1, double dfX0, double dfX1, double dfX3, double dfX4, double dfX5, double dfX6, double dfX7, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_ArcYZ_Aux(double dfRX1, double dfRX2, double dfX1, double dfX2, double dfX3, double dfX4, double dfX5, double dfX6, double dfX7, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_ArcZX_Aux(double dfRX2, double dfRX0, double dfX2, double dfX0, double dfX3, double dfX4, double dfX5, double dfX6, double dfX7, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_ArcThetaXY(double dfCX, double dfCY, double dfTheta, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_ArcThetaYZ(double dfCY, double dfCZ, double dfTheta, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_ArcThetaZX(double dfCZ, double dfCX, double dfTheta, WORD wGroupIndex = 0);

// Circular motion
MCC_API int MCC_CALL MCC_CircleXY(double dfCX, double dfCY, BYTE byCirDir, WORD wGroupIndex = 0);
MCC_API int MCC_CALL MCC_CircleYZ(double dfCY, double dfCZ, BYTE byCirDir, WORD wGroupIndex = 0);
MCC_API int MCC_CALL MCC_CircleZX(double dfCZ, double dfCX, BYTE byCirDir, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_CircleXY_Aux(double dfCX, double dfCY, double dfU, double dfV, double dfW, double dfA, double dfB, BYTE byCirDir, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_CircleYZ_Aux(double dfCY, double dfCZ, double dfU, double dfV, double dfW, double dfA, double dfB, BYTE byCirDir, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_CircleZX_Aux(double dfCZ, double dfCX, double dfU, double dfV, double dfW, double dfA, double dfB, BYTE byCirDir, WORD wGroupIndex = 0);

// Helical motion
MCC_API	int MCC_CALL MCC_HelicalXY_Z(double dfCX, double dfCY, double dfPitch, double dfTheta, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_HelicalYZ_X(double dfCY, double dfCZ, double dfPitch, double dfTheta, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_HelicalZX_Y(double dfCZ, double dfCX, double dfPitch, double dfTheta, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_HelicalXY_Z_Aux( double dfCX, double dfCY, double dfPitch, double dfU, double dfV, double dfW, double dfA, double dfB, double dfTheta, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_HelicalYZ_X_Aux( double dfCY, double dfCZ, double dfPitch, double dfU, double dfV, double dfW, double dfA, double dfB, double dfTheta, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_HelicalZX_Y_Aux( double dfCZ, double dfCX, double dfPitch, double dfU, double dfV, double dfW, double dfA, double dfB, double dfTheta, WORD wGroupIndex = 0);	


//////////////////////////////////////////////////////////////////////////////
// Jog Motion
MCC_API	int MCC_CALL MCC_JogPulse(int nPulse, char cAxis, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_JogSpace(double dfOffset, double dfRatio, char cAxis, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_JogConti(int nDir, double dfRatio, char cAxis, WORD wGroupIndex = 0);


//////////////////////////////////////////////////////////////////////////////
// Point-to-Point Motion

// Set/Get acceleration & deceleration types
MCC_API	int MCC_CALL MCC_SetPtPAccType(char cAccType0, char cAccType1, char cAccType2, char cAccType3, char cAccType4, char cAccType5, char cAccType6, char cAccType7, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_GetPtPAccType(char *pcAccType0, char *pcAccType1, char *pcAccType2, char *pcAccType3, char *pcAccType4, char *pcAccType5, char *pcAccType6, char *pcAccType7, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_SetPtPDecType(char cDecType0, char cDecType1, char cDecType2, char cDecType3, char cDecType4, char cDecType5, char cDecType6, char cDecType7, WORD wGroupIndex = 0);
MCC_API	int	MCC_CALL MCC_GetPtPDecType(char *pcDecType0, char *pcDecType1, char *pcDecType2, char *pcDecType3, char *pcDecType4, char *pcDecType5, char *pcDecType6, char *pcDecType7, WORD wGroupIndex = 0);

// Set/Get acceleration & deceleration times
MCC_API	int MCC_CALL MCC_SetPtPAccTime(double dfAccTime0, double dfAccTime1, double dfAccTime2, double dfAccTime3, double dfAccTime4, double dfAccTime5, double dfAccTime6, double dfAccTime7, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_GetPtPAccTime(double *pdfAccTime0, double *pdfAccTime1, double *pdfAccTime2, double *pdfAccTime3, double *pdfAccTime4, double *pdfAccTime5, double *pdfAccTime6, double *pdfAccTime7, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_SetPtPDecTime(double dfDecTime0, double dfDecTime1, double dfDecTime2, double dfDecTime3, double dfDecTime4, double dfDecTime5, double dfDecTime6, double dfDecTime7, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_GetPtPDecTime(double *pdfDecTime0, double *pdfDecTime1, double *pdfDecTime2, double *pdfDecTime3, double *pdfDecTime4, double *pdfDecTime5, double *pdfDecTime6, double *pdfDecTime7, WORD wGroupIndex = 0);

//////////////////////////////////////////////////////////////////////////////
// Motion Status

// Get current motion status
MCC_API	int MCC_CALL MCC_GetMotionStatus(WORD wGroupIndex = 0);

// Get information of the currently running motion command
MCC_API	int MCC_CALL MCC_GetCurCommand(COMMAND_INFO *pstCurCommand, WORD wGroupIndex = 0);

// Reset index of motion commands coming after
MCC_API	int MCC_CALL MCC_ResetCommandIndex(WORD wGroupIndex = 0);

// Get number of the motion commands in queue
MCC_API	int MCC_CALL MCC_GetCommandCount(int *pnCmdCount, WORD wGroupIndex = 0);

// Set/Get maximum number of the hardware pulse stock
MCC_API	int MCC_CALL MCC_SetMaxPulseStockNum(int nMaxStockNum);
MCC_API	int MCC_CALL MCC_GetMaxPulseStockNum();

// Get hardware pulse stock count
MCC_API	int MCC_CALL MCC_GetCurPulseStockCount(WORD *pwStockCount, WORD wChannel, WORD wCardIndex = 0);

// Get/Clear error code
MCC_API	int MCC_CALL MCC_GetErrorCode(WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_ClearError(WORD wGroupIndex = 0);


//////////////////////////////////////////////////////////////////////////////
// Position Control

// Set/Get maximum pulse speed
MCC_API	int MCC_CALL MCC_SetMaxPulseSpeed(int nPulse0, int nPulse1, int nPulse2, int nPulse3, int nPulse4, int nPulse5, int nPulse6, int nPulse7, WORD wCardIndex = 0);
MCC_API	int MCC_CALL MCC_GetMaxPulseSpeed(int *pnSpeed0, int *pnSpeed1, int *pnSpeed2, int *pnSpeed3, int *pnSpeed4, int *pnSpeed5,  int *pnSpeed6, int *pnSpeed7, WORD wCardIndex = 0);

// Set/Get maximum pulse acceleration
MCC_API	int MCC_CALL MCC_SetMaxPulseAcc(int nPulseAcc0, int nPulseAcc1, int nPulseAcc2, int nPulseAcc3, int nPulseAcc4, int nPulseAcc5, int nPulseAcc6, int nPulseAcc7, WORD wCardIndex = 0);
MCC_API	int MCC_CALL MCC_GetMaxPulseAcc(int *pnPulseAcc0, int *pnPulseAcc1, int *pnPulseAcc2, int *pnPulseAcc3, int *pnPulseAcc4, int *pnPulseAcc5, int *pnPulseAcc6, int *pnPulseAcc7, WORD wCardIndex = 0);

// Define compensation table
MCC_API	int MCC_CALL MCC_SetCompParam(SYS_COMP_PARAM *pstCompParam, WORD wChannel, WORD wCardIndex = 0);
MCC_API	int MCC_CALL MCC_UpdateCompParam();


//////////////////////////////////////////////////////////////////////////////
// Trajectory Planning

// Hold/Continue/Abort motion
MCC_API	int MCC_CALL MCC_HoldMotion(WORD wGroupIndex = 0, BOOL bAfterCurCmd = 0);
MCC_API	int MCC_CALL MCC_ContiMotion(WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_AbortMotionEx(double dfDecTime, WORD wGroupIndex = 0);

// Enable/Disable motion blending
MCC_API	int MCC_CALL MCC_EnableBlend(WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_DisableBlend(WORD wGroupIndex = 0); 
MCC_API	int MCC_CALL MCC_CheckBlend(WORD wGroupIndex = 0);

// Set Delay Time
MCC_API	int MCC_CALL MCC_DelayMotion(DWORD dwTime, WORD wGroupIndex = 0);
MCC_API int MCC_CALL MCC_CheckDelay(WORD wGroupIndex = 0);
MCC_API void MCC_CALL MCC_TimeDelay(DWORD dwTime);

// Set/Get over-speed ratio for general motions
MCC_API	double MCC_CALL MCC_OverrideSpeed(double dfRate, WORD wGroupIndex = 0);
MCC_API double MCC_CALL MCC_OverrideSpeedEx(double dfRate, BOOL bInstant = 1, WORD wGroupIndex = 0);
MCC_API	double MCC_CALL MCC_GetOverrideRate(WORD wGroupIndex = 0);


//////////////////////////////////////////////////////////////////////////////
// Encoder Control

MCC_API	int MCC_CALL MCC_GetENCValue(long *plValue, WORD wChannel, WORD wCardIndex = 0);


//////////////////////////////////////////////////////////////////////////////
// Obsolete functions in earlier MCCL version (just for compatibility)

MCC_API	int MCC_CALL MCC_PtPX(double dfX, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_PtPY(double dfY, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_PtPZ(double dfZ, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_PtPU(double dfU, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_PtPV(double dfV, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_PtPW(double dfW, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_PtPA(double dfA, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_PtPB(double dfB, WORD wGroupIndex = 0);

MCC_API	int MCC_CALL MCC_LineX(double dfX, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_LineY(double dfY, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_LineZ(double dfZ, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_LineU(double dfU, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_LineV(double dfV, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_LineW(double dfW, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_LineA(double dfA, WORD wGroupIndex = 0);
MCC_API	int MCC_CALL MCC_LineB(double dfB, WORD wGroupIndex = 0);

MCC_API	int MCC_CALL MCC_AbortMotion(WORD wGroupIndex, BOOL bAfterCurCmd = 0);


//////////////////////////////////////////////////////////////////////////////
//EtherCAT

//Control Code
MCC_API int  MCC_CALL MCC_EcatGetControlCode(WORD *pwStatus, int nChannel);
MCC_API int  MCC_CALL MCC_EcatSetControlCode(WORD wStatus, int nChannel);

//Status Code
MCC_API int    MCC_CALL MCC_EcatGetStatusCode(WORD *pwStatus, int nChannel);

//Home
MCC_API int  MCC_CALL MCC_EcatHome();
MCC_API int  MCC_CALL MCC_EcatSetHomeMode(int nMode, int nChannel);
MCC_API int  MCC_CALL MCC_EcatSetHomeSwitchSpeed(int nSpeed, int nChannel);
MCC_API int  MCC_CALL MCC_EcatSetHomeZeroSpeed(int nSpeed, int nChannel);
MCC_API int  MCC_CALL MCC_EcatSetHomeAxis(BYTE cAxisX, BYTE cAxisY = 0, BYTE cAxisZ = 0, BYTE cAxisU = 0, BYTE cAxisV = 0, BYTE cAxisW = 0, BYTE cAxisA = 0, BYTE cAxisB = 0,
	BYTE cAxisX1 = 0, BYTE cAxisY1 = 0, BYTE cAxisZ1 = 0, BYTE cAxisU1 = 0, BYTE cAxisV1 = 0, BYTE cAxisW1 = 0, BYTE cAxisA1 = 0, BYTE cAxisB1 = 0);
MCC_API int  MCC_CALL MCC_EcatGetGoHomeStatus();

//TargetPos
MCC_API int  MCC_CALL MCC_EcatSetTargetPos(int nPosition, int nChannel);
MCC_API int  MCC_CALL MCC_EcatGetTargetPos(int *nTargetPos, DWORD dwSlaveId);

//CoE
MCC_API int  MCC_CALL MCC_EcatSetOpMode(int nOperation, int nChannel);
MCC_API int  MCC_CALL MCC_EcatCoeSdoDownload(DWORD dwSlaveId, WORD wObIndex, BYTE byObSubIndex , BYTE* pbyData, DWORD dwDataLen);
MCC_API int  MCC_CALL MCC_EcatCoeSdoUpload(DWORD dwSlaveId, WORD wObIndex, BYTE byObSubIndex, BYTE* pbyData, DWORD dwDataLen, DWORD* pdwOutDataLen);
MCC_API int  MCC_CALL MCC_EcatStartAction(DWORD dwSlaveId);

//Input/Output
MCC_API int  MCC_CALL MCC_EcatSetOutput(DWORD dwSlaveId, DWORD dwOutData);
MCC_API int  MCC_CALL MCC_EcatGetOutput(DWORD dwSlaveId, DWORD *dwInData);
MCC_API int  MCC_CALL MCC_EcatGetInput(DWORD dwSlaveId, DWORD *dwInData);

//Reset
MCC_API int  MCC_CALL MCC_EcatReset(DWORD dwSlaveId);

#ifdef __cplusplus
}
#endif


#endif /*__NCCL_H__*/

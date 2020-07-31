using System;
using System.Runtime.InteropServices;


namespace EtherCATSeries
{
    //++
    //
    // Description:
    //
    //     The prototype of custom motion profiling functions.
    //
    // Arguments:
    //
    //     nGroup - [in] Group index.
    //     nSynAxisNum - [in] Amount of the axes of this group.
    //
    // Return Value:
    //
    //     Total distance this motion will walk through (in user-unit).
    //
    // Remarks:
    //
    //
    //--

	/// <summary>
	/// Summary description for MCCL.
	/// </summary>
    internal static class MCCL
    {
        #region Constants
        /////////////////////////////////////////////////////////////////////
        // Common Definitions

        public const int MAX_CARD_NUM = 12;
        public const int MAX_AXIS_NUM = 8;
        public const int MAX_GROUP_NUM = 96;

        public const int MOTION_QUEUE_SIZE = 10000;
		        
        // EtherCAT version
        private const string LibNameVersion = "MCCL_Client.dll"; 
        
        public const int GROUP_VALID = 0;
        public const int GROUP_INVALID = -1;
        public const int AXIS_INVALID = -1;

        public const int _YES_ = 1;
        public const int _NO_ = 0;
        
        public const string MCC_VERSION = "EC V1.0";
        

        /////////////////////////////////////////////////////////////////////
        // Range Definitions of Interpolation Period (ms)
        public const int IPO_PERIOD_MIN = 1;
        public const int IPO_PERIOD_DEFAULT = 5;
        public const int IPO_PERIOD_MAX = 50;

        /////////////////////////////////////////////////////////////////////
        // Definitions of Output Command Modes
        public const int OCM_PULSE = 0;
        public const int OCM_VOLTAGE = 1;
        public const int OCM_SWCLOSELOOP = 2;

        /////////////////////////////////////////////////////////////////////
        // Definitions of Sensor Logic
        public const int SL_NORMAL_OPEN = 0;
        public const int SL_NORMAL_CLOSE = 1;
        public const int SL_UNUSED = 2;

        /////////////////////////////////////////////////////////////////////
        // Definition(s) of motion card types
        public const int EPCIO_4_AXIS_ISA_CARD  = 0;
        public const int EPCIO_6_AXIS_ISA_CARD  = 1;
        public const int EPCIO_4_AXIS_PCI_CARD  = 2;
        public const int EPCIO_6_AXIS_PCI_CARD  = 3;
        public const int IMP_II_8_AXIS_PCI_CARD = 4;
        public const int IMP_III_8_AXIS_PCI_CARD = 5;
        public const int EPCIO_6_AXIS_PCIE_CARD = 6;
        public const int EMP_MULTI_AXES = 7;

        
        /////////////////////////////////////////////////////////////////////
        // Definitions of circular directions
        public const int CIR_CW   = 0; // clockwise
        public const int CIR_CCW = 1; // counter-clockwise

        /////////////////////////////////////////////////////////////////////
        // Definitions of Group Motion Status
        public const int GMS_RUNNING = 0;
        public const int GMS_STOP = 1;
        public const int GMS_HOLD = 2;
        public const int GMS_DELAYING = 3;

        /////////////////////////////////////////////////////////////////////
        // Definitions of MCCL Axis Flag
        public const uint EMP_AXIS_X = 0x0001;
        public const uint EMP_AXIS_Y = 0x0002;
        public const uint EMP_AXIS_Z = 0x0004;
        public const uint EMP_AXIS_U = 0x0008;
        public const uint EMP_AXIS_V = 0x0010;
        public const uint EMP_AXIS_W = 0x0020;
        public const uint EMP_AXIS_A = 0x0040;
        public const uint EMP_AXIS_B = 0x0080;
        public const uint EMP_AXIS_ALL = 0x00FF;
        
        /////////////////////////////////////////////////////////////////////
        // MCCL Error Codes Definitions
        public const int NO_ERR = 0;
        public const int INITIAL_MOTION_ERR = -1;
        public const int COMMAND_BUFFER_FULL_ERR = -2;
        public const int COMMAND_NOTACCEPTED_ERR = -3;
        public const int COMMAND_NOTFINISHED_ERR = -4;
        public const int PARAMETER_ERR = -5;
        public const int GROUP_PARAMETER_ERR = -6;
        public const int FEED_RATE_ERR = -7;
        public const int BLEND_COMMAND_NOTCALLED_ERR = -8;
        public const int VOLTAGE_COMMAND_CHANNEL_ERR = -9;
        public const int HOME_COMMAND_NOTCALLED_ERR = -10;
        public const int HOLD_ILLEGAL_ERR = -11;
        public const int CONTI_ILLEGAL_ERR = -12;
        public const int ABORT_ILLEGAL_ERR = -13;
        public const int RUN_TIME_ERR = -14;
        public const int ABORT_NOT_FINISH_ERR = -15;
        public const int GROUP_RAN_OUT_ERR = -16;

        /////////////////////////////////////////////////////////////////////
        // Compensation-related Definitions
        public const int MAX_COMP_POINT = 256;
        

        /////////////////////////////////////////////////////////////////////
        // EtherCAT Type Definitions

        public const int ECAT_ERROR_NONE			=	0;
        public const int ECAT_ERROR_FAULT			=	1;
        public const int ECAT_ERROR_WARNING			=	2;

        #endregion Constants
        
        #region Functions

        //////////////////////////////////////////////////////////////////////////////
        // System Management
        
        // RTX functions
        [DllImport(LibNameVersion)]
        public static extern int MCC_StartEcServer(int nSleepSec = 0);

        [DllImport(LibNameVersion)]
        public static extern int MCC_StartEcServerEx(String EcServerPath, int nSleepSec = 0);

        [DllImport(LibNameVersion)]
        public static extern int MCC_RtxInit(int nAxis);

        [DllImport(LibNameVersion)]
        public static extern int MCC_RtxClose();
                        

		// Get Library Version
		[ DllImport( LibNameVersion )]
		public static extern void MCC_GetVersion(ref char strVersion);


        // Create/Close Motion Groups
		[ DllImport( LibNameVersion )]
        public static extern int MCC_CreateGroup(int xMapToCh, int yMapToCh, int zMapToCh, int uMapToCh, int vMapToCh, int wMapToCh, int aMapToCh, int bMapToCh, int nCardIndex = 0);
        
        [DllImport(LibNameVersion)]
        public static extern int MCC_CreateGroupEx(int xMapToCh, int yMapToCh, int zMapToCh, int uMapToCh, int vMapToCh, int wMapToCh, int aMapToCh, int bMapToCh, int nCardIndex = 0, int nMotionQueueSize = MCCL.MOTION_QUEUE_SIZE);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_CloseGroup(int nGroupIndex = 0);
		
		[ DllImport( LibNameVersion )]
		public static extern int MCC_CloseAllGroups();


		// Set/Get Mechanism Parameters
		[ DllImport( LibNameVersion )]
        public static extern int MCC_SetMacParam(ref SYS_MAC_PARAM pstMacParam, ushort wChannel, ushort wCardIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_GetMacParam(ref SYS_MAC_PARAM pstMacParam, ushort wChannel, ushort wCardIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern int MCC_UpdateParam();


		// Get size of motion command queue
		[ DllImport( LibNameVersion )]
        public static extern int MCC_SetCmdQueueSize(int nSize, ushort wGroupIndex = 0);
        

		// Initialize/Close System
		[ DllImport( LibNameVersion )]
        public static extern int MCC_InitSystem(int nInterpolateTime, ref SYS_CARD_CONFIG psCardConfig, ushort wCardNo = 0);

        [DllImport( LibNameVersion )]
        public static extern int MCC_InitSystemEx(double dInterpolateTime, ref SYS_CARD_CONFIG psCardConfig, ushort wCardNo = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_InitSimulation(int nInterpolateTime, ref SYS_CARD_CONFIG pstCardConfig, ushort wCardNo = 0);

		[ DllImport( LibNameVersion )]
		public static extern int MCC_CloseSystem();


		// Reset MCCL
		[ DllImport( LibNameVersion )]
		public static extern int MCC_ResetMotion();


        // Set/Get Max. Feed Speed
        [DllImport(LibNameVersion)]
        public static extern int MCC_SetSysMaxSpeed(double dfMaxSpeed);

        [DllImport(LibNameVersion)]
        public static extern double MCC_GetSysMaxSpeed();

        
		//////////////////////////////////////////////////////////////////////////////
		// Coordinate Management

		// Set/Get Coordinate Type
		[ DllImport( LibNameVersion )]
        public static extern int MCC_SetAbsolute(ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_SetIncrease(ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_GetCoordType(ushort wGroupIndex = 0);


		// Get Current Position & Pulse Position
		[ DllImport( LibNameVersion )]
        public static extern int MCC_GetCurRefPos(ref double pdfX, ref double pdfY, ref double pdfZ, ref double pdfU, ref double pdfV, ref double pdfW, ref double pdfA, ref double pdfB, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_GetCurPos(ref double pdfX, ref double pdfY, ref double pdfZ, ref double pdfU, ref double pdfV, ref double pdfW, ref double pdfA, ref double pdfB, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_GetPulsePos(ref int plX, ref int plY, ref int plZ, ref int plU, ref int plV, ref int plW, ref int plA, ref int plB, ushort wGroupIndex = 0);
        

        //Get Encoder value
        [DllImport(LibNameVersion)]
        public static extern int MCC_GetENCValue(ref int plValue, ushort wChannel, ushort wCardIndex = 0);


        // Regard here as origin
		[ DllImport( LibNameVersion )]
        public static extern int MCC_DefineOrigin(ushort wAxis, ushort wGroupIndex = 0);


        // Align command position with actual position
		[ DllImport( LibNameVersion )]
        public static extern int MCC_DefinePosHere(ushort wGroupIndex = 0, uint dwAxisMask = MCCL.EMP_AXIS_ALL);


        // Change command and actual positions according to specified value
		[ DllImport( LibNameVersion )]
        public static extern int MCC_DefinePos(ushort wAxis, double dfCart, ushort wGroupIndex = 0);


		//////////////////////////////////////////////////////////////////////////////
		// Software Over Travel Check & Hardware Limit Switch Check

		// Enable/Disable Hardware Limit Switch Check
		[ DllImport( LibNameVersion )]
		public static extern int MCC_EnableLimitSwitchCheck(int nMode);

		[ DllImport( LibNameVersion )]
		public static extern int MCC_DisableLimitSwitchCheck();


		// Enable/Disable Software Over Travel Check
		[ DllImport( LibNameVersion )]
        public static extern int MCC_SetOverTravelCheck(int nOTCheck0, int nOTCheck1, int nOTCheck2, int nOTCheck3, int nOTCheck4, int nOTCheck5, int nOTCheck6, int nOTCheck7, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_GetOverTravelCheck(ref int pnOTCheck0, ref int pnOTCheck1, ref int pnOTCheck2, ref int pnOTCheck3, ref int pnOTCheck4, ref int pnOTCheck5, ref int pnOTCheck6, ref int pnOTCheck7, ushort wGroupIndex = 0);


		// Get Limit Switch Sensor Signal
		[ DllImport( LibNameVersion )]
        public static extern int MCC_GetLimitSwitchStatus(ref ushort pwStatus, ushort wUpDown, ushort wChannel, ushort wCardIndex = 0);
    

		//////////////////////////////////////////////////////////////////////////////
		// General Motions(Line, Arc, Circle, and Helical Motions)

		// Set/Get Accleration & Deceleration Type
		[ DllImport( LibNameVersion )]
        public static extern int MCC_SetAccType(char cAccType, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_GetAccType(ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_SetDecType(char cDecType, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_GetDecType(ushort wGroupIndex = 0);		


		// Set/Get Accleration & Deceleration Time
		[ DllImport( LibNameVersion )]
        public static extern int MCC_SetAccTime(double dfAccTime, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern double MCC_GetAccTime(ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_SetDecTime(double dfDecTime, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern double MCC_GetDecTime(ushort wGroupIndex = 0);


		// Set/Get Feed Speed
		[ DllImport( LibNameVersion )]
        public static extern double MCC_SetFeedSpeed(double dfFeedSpeed, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern double MCC_GetFeedSpeed(ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern double MCC_GetCurFeedSpeed(ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_GetSpeed(ref double pdfVel0, ref double pdfVel1, ref double pdfVel2, ref double pdfVel3, ref double pdfVel4, ref double pdfVel5, ref double pdfVel6, ref double pdfVel7, ushort wGroupIndex = 0);


        // Linear Motion
		[ DllImport( LibNameVersion )]
        public static extern int MCC_Line(double dfX0, double dfX1, double dfX2, double dfX3, double dfX4, double dfX5, double dfX6, double dfX7, ushort wGroupIndex = 0, uint dwAxisMask = MCCL.EMP_AXIS_ALL);


		// Arc Motion
		[ DllImport( LibNameVersion )]
        public static extern int MCC_ArcXYZ(double dfRX0, double dfRX1, double dfRX2, double dfX0, double dfX1, double dfX2, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
		public static extern int MCC_ArcXY(double dfRX0, double dfRX1, double dfX0, double dfX1, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
		public static extern int MCC_ArcYZ(double dfRX1, double dfRX2, double dfX1, double dfX2, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
		public static extern int MCC_ArcZX(double dfRX2, double dfRX0, double dfX2, double dfX0, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_ArcXYZ_Aux(double dfRX0, double dfRX1, double dfRX2, double dfX0, double dfX1, double dfX2, double dfX3, double dfX4, double dfX5, double dfX6, double dfX7, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_ArcXY_Aux(double dfRX0, double dfRX1, double dfX0, double dfX1, double dfX3, double dfX4, double dfX5, double dfX6, double dfX7, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_ArcYZ_Aux(double dfRX1, double dfRX2, double dfX1, double dfX2, double dfX3, double dfX4, double dfX5, double dfX6, double dfX7, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_ArcZX_Aux(double dfRX2, double dfRX0, double dfX2, double dfX0, double dfX3, double dfX4, double dfX5, double dfX6, double dfX7, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_ArcThetaXY(double dfCX, double dfCY, double dfTheta, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_ArcThetaYZ(double dfCY, double dfCZ, double dfTheta, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_ArcThetaZX(double dfCZ, double dfCX, double dfTheta, ushort wGroupIndex = 0);


        // Circular Motion
		[ DllImport( LibNameVersion )]
        public static extern int MCC_CircleXY(double dfCX, double dfCY, byte byCirDir, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_CircleYZ(double dfCY, double dfCZ, byte byCirDir, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_CircleZX(double dfCZ, double dfCX, byte byCirDir, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_CircleXY_Aux(double dfCX, double dfCY, double dfU, double dfV, double dfW, double dfA, double dfB, byte byCirDir, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_CircleYZ_Aux(double dfCY, double dfCZ, double dfU, double dfV, double dfW, double dfA, double dfB, byte byCirDir, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_CircleZX_Aux(double dfCZ, double dfCX, double dfU, double dfV, double dfW, double dfA, double dfB, byte byCirDir, ushort wGroupIndex = 0);


		// Helical Motion
		[ DllImport( LibNameVersion )]
        public static extern int MCC_HelicalXY_Z(double dfCX, double dfCY, double dfPitch, double dfTheta, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_HelicalYZ_X(double dfCY, double dfCZ, double dfPitch, double dfTheta, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_HelicalZX_Y(double dfCZ, double dfCX, double dfPitch, double dfTheta, ushort wGroupIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern int MCC_HelicalXY_Z_Aux(double dfCX, double dfCY, double dfPitch, double dfU, double dfV, double dfW, double dfA, double dfB, double dfTheta, ushort wGroupIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern int MCC_HelicalYZ_X_Aux(double dfCZ, double dfCX, double dfPitch, double dfU, double dfV, double dfW, double dfA, double dfB, double dfTheta, ushort wGroupIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern int MCC_HelicalZX_Y_Aux(double dfCZ, double dfCX, double dfPitch, double dfU, double dfV, double dfW, double dfA, double dfB, double dfTheta, ushort wGroupIndex = 0);


        //////////////////////////////////////////////////////////////////////////////
        // Jog Motion

        [DllImport(LibNameVersion)]
        public static extern int MCC_JogPulse(int nPulse, char cAxis, ushort wGroupIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern int MCC_JogSpace(double dfOffset, double dfRatio, char cAxis, ushort wGroupIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern int MCC_JogConti(int nDir, double dfRatio, char cAxis, ushort wGroupIndex = 0);
        

        //////////////////////////////////////////////////////////////////////////////
		// Point to Point Motion

        //Set/Get Point-to-Point Motion  Accleration & Deceleration Type
		[ DllImport( LibNameVersion )]
        public static extern int MCC_SetPtPAccType(char cAccType0, char cAccType1, char cAccType2, char cAccType3, char cAccType4, char cAccType5, char cAccType6, char cAccType7, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_GetPtPAccType(ref char pcAccType0, ref char pcAccType1, ref char pcAccType2, ref char pcAccType3, ref char pcAccType4, ref char pcAccType5, ref char pcAccType6, ref char pcAccType7, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_SetPtPDecType(char cDecType0, char cDecType1, char cDecType2, char cDecType3, char cDecType4, char cDecType5, char cDecType6, char cDecType7, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_GetPtPDecType(ref char pcDecType0, ref char pcDecType1, ref char pcDecType2, ref char pcDecType3, ref char pcDecType4, ref char pcDecType5, ref char pcDecType6, ref char pcDecType7, ushort wGroupIndex = 0);

        // Set/Get Point-to-Point Motion  Accleration & Deceleration Time
		[ DllImport( LibNameVersion )]
        public static extern int MCC_SetPtPAccTime(double dfAccTime0, double dfAccTime1, double dfAccTime2, double dfAccTime3, double dfAccTime4, double dfAccTime5, double dfAccTime6, double dfAccTime7, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_GetPtPAccTime(ref double pdfAccTime0, ref double pdfAccTime1, ref double pdfAccTime2, ref double pdfAccTime3, ref double pdfAccTime4, ref double pdfAccTime5, ref double pdfAccTime6, ref double pdfAccTime7, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_SetPtPDecTime(double dfDecTime0, double dfDecTime1, double dfDecTime2, double dfDecTime3, double dfDecTime4, double dfDecTime5, double dfDecTime6, double dfDecTime7, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_GetPtPDecTime(ref double pdfDecTime0, ref double pdfDecTime1, ref double pdfDecTime2, ref double pdfDecTime3, ref double pdfDecTime4, ref double pdfDecTime5, ref double pdfDecTime6, ref double pdfDecTime7, ushort wGroupIndex = 0);

        // Set/Get Point-to-Point Motion Speed Ratio
        [DllImport(LibNameVersion)]
        public static extern double MCC_SetPtPSpeed(double dfRatio, ushort wGroupIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern double MCC_GetPtPSpeed(ushort wGroupIndex = 0);

        // Point to Point Motion
        [DllImport(LibNameVersion)]
        public static extern int MCC_PtP(double dfX0, double dfX1, double dfX2, double dfX3, double dfX4, double dfX5, double dfX6, double dfX7, ushort wGroupIndex = 0, uint dwAxisMask = MCCL.EMP_AXIS_ALL);

        [DllImport(LibNameVersion)]
        public static extern int MCC_PtPX(double dfX, ushort wGroupIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern int MCC_PtPY(double dfY, ushort wGroupIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern int MCC_PtPZ(double dfZ, ushort wGroupIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern int MCC_PtPU(double dfU, ushort wGroupIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern int MCC_PtPV(double dfV, ushort wGroupIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern int MCC_PtPW(double dfW, ushort wGroupIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern int MCC_PtPA(double dfA, ushort wGroupIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern int MCC_PtPB(double dfB, ushort wGroupIndex = 0);
	

		//////////////////////////////////////////////////////////////////////////////
		// Motion Status

		// Get Current Motion Status
		[ DllImport( LibNameVersion )]
        public static extern int MCC_GetMotionStatus(ushort wGroupIndex = 0);


		// Get Current Executing Motion Command
		[ DllImport( LibNameVersion )]
        public static extern int MCC_GetCurCommand(ref COMMAND_INFO pstCurCommand, ushort wGroupIndex = 0);


        // Get/Reset Motion Command Stock Count
		[ DllImport( LibNameVersion )]
        public static extern int MCC_GetCommandCount(ref int pnCmdCount, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_ResetCommandIndex(ushort wGroupIndex = 0);


        // Get/Reset Motion Command Stock Count
		[ DllImport( LibNameVersion )]
        public static extern int MCC_SetMaxPulseStockNum(int nMaxStockNum = 0);

		[ DllImport( LibNameVersion )]
		public static extern int MCC_GetMaxPulseStockNum();


        // Set/Get Hardware Pulse Stock Count
		[ DllImport( LibNameVersion )]
        public static extern int MCC_GetCurPulseStockCount(ref ushort pwStockCount, ushort wChannel, ushort wCardIndex = 0);


		// Get/Clear Error Code
		[ DllImport( LibNameVersion )]
        public static extern int MCC_GetErrorCode(ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_ClearError(ushort wGroupIndex = 0);
        

        //////////////////////////////////////////////////////////////////////////////
        // Position Control
        
        // Set Compensation Table
        [DllImport(LibNameVersion)]
        public static extern int MCC_SetCompParam(ref SYS_COMP_PARAM pstCompParam, ushort wChannel, ushort wCardIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern int MCC_UpdateCompParam();


        // Set/Get Maximum Pulse Speed & Accleration
		[ DllImport( LibNameVersion )]
        public static extern int MCC_SetMaxPulseSpeed(int nPulse0, int nPulse1, int nPulse2, int nPulse3, int nPulse4, int nPulse5, int nPulse6, int nPulse7, ushort wCardIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_GetMaxPulseSpeed(ref int pnSpeed0, ref int pnSpeed1, ref int pnSpeed2, ref int pnSpeed3, ref int pnSpeed4, ref int pnSpeed5, ref int pnSpeed6, ref int pnSpeed7, ushort wCardIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_SetMaxPulseAcc(int nPulseAcc0, int nPulseAcc1, int nPulseAcc2, int nPulseAcc3, int nPulseAcc4, int nPulseAcc5, int nPulseAcc6, int nPulseAcc7, ushort wCardIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_GetMaxPulseAcc(ref int pnPulseAcc0, ref int pnPulseAcc1, ref int pnPulseAcc2, ref int pnPulseAcc3, ref int pnPulseAcc4, ref int pnPulseAcc5, ref int pnPulseAcc6, ref int pnPulseAcc7, ushort wCardIndex = 0);


		//////////////////////////////////////////////////////////////////////////////
		// Advanced Trajectory Planning

		// Hold/Continue/Abort Motion
		[ DllImport( LibNameVersion )]
		public static extern int MCC_HoldMotion(ushort wGroupIndex, bool bAfterCmd);        

		[ DllImport( LibNameVersion )]
        public static extern int MCC_ContiMotion(ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_AbortMotionEx(double dfDecTime, ushort wGroupIndex = 0);


		// Enable/Disable Motion Blending
		[ DllImport( LibNameVersion )]
        public static extern int MCC_EnableBlend(ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern int MCC_DisableBlend(ushort wGroupIndex = 0); 

		[ DllImport( LibNameVersion )]
        public static extern int MCC_CheckBlend(ushort wGroupIndex = 0);


		// Set Delay Time
		[ DllImport( LibNameVersion )]
        public static extern int MCC_DelayMotion(uint dwTime, ushort wGroupIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern int MCC_CheckDelay(ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
		public static extern void   MCC_TimeDelay(uint dwTime);

		// Set/Get Over-Speed Ratio for General Motions
		[ DllImport( LibNameVersion )]
        public static extern double MCC_OverrideSpeed(double dfRate, ushort wGroupIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern double MCC_OverrideSpeedEx(double dfRate, bool bInstant, ushort wGroupIndex = 0);

		[ DllImport( LibNameVersion )]
        public static extern double MCC_GetOverrideRate(ushort wGroupIndex = 0);



        //////////////////////////////////////////////////////////////////////////////
        // EtherCAT System function

        // Servo On/Off
        [DllImport(LibNameVersion)]
        public static extern int MCC_SetServoOn(ushort wChannel, ushort wCardIndex = 0);
        [DllImport(LibNameVersion)]
        public static extern int MCC_SetServoOff(ushort wChannel, ushort wCardIndex = 0);


        //EtherCAT Reset Fault
        [DllImport(LibNameVersion)]
        public static extern ushort MCC_EcatFaultReset(uint dwSlaveId);
        
        
        //EtherCAT Error Code
        [DllImport(LibNameVersion)]
        public static extern ushort MCC_EcatGetErrorCode(ref int pnType, ref ushort pwCode, ushort wChannel);

        
        //Watch Dog
        [DllImport(LibNameVersion)]
        public static extern int MCC_EcatEnableWatchDog(int nExpireTime);

        [DllImport(LibNameVersion)]
        public static extern int MCC_EcatDisableWatchDog();


        //EtherCAT I/O
        [DllImport(LibNameVersion)]
        public static extern int MCC_EcatGetInput(uint dwSlaveId, ref uint pdwInData);

        [DllImport(LibNameVersion)]
        public static extern int MCC_EcatGetOutput(uint dwSlaveId, ref uint pdwOutData);

        [DllImport(LibNameVersion)]
        public static extern int MCC_EcatSetOutput(uint dwSlaveId, uint dwOutData);

        [DllImport(LibNameVersion)]
        public static extern int MCC_EcatSetOutputEnqueue(uint dwSlaveId, uint dwOutData, ushort wGroupIndex);


        //EtherCAT SDO
        [DllImport(LibNameVersion)]
        public static extern int MCC_EcatCoeSdoDownload(uint dwSlaveId, ushort wObIndex, byte byObSubIndex, System.IntPtr pbyData, uint dwDataLen);

        [DllImport(LibNameVersion)]
        public static extern int MCC_EcatCoeSdoUpload(uint dwSlaveId, ushort wObIndex, byte byObSubIndex, System.IntPtr pbyData, uint dwDataLen, ref uint pdwOutDataLen);


        //////////////////////////////////////////////////////////////////////////////
        // EtherCAT Home function

        [DllImport(LibNameVersion)]
        public static extern int MCC_EcatSetHomeAxis(byte byAxisX, byte byAxisY, byte byAxisZ, byte byAxisU, byte byAxisV, byte byAxisW, byte byAxisA, byte byAxisB,
                                                        byte byAxisX1, byte byAxisY1, byte byAxisZ1, byte byAxisU1, byte byAxisV1, byte byAxisW1, byte byAxisA1, byte byAxisB1);

        [DllImport(LibNameVersion)]
        public static extern int MCC_EcatSetHomeMode(int nMode, int nChannel);

        [DllImport(LibNameVersion)]
        public static extern int MCC_EcatSetHomeZeroSpeed(int nZeroSpeed, int nChannel);

        [DllImport(LibNameVersion)]
        public static extern int MCC_EcatSetHomeSwitchSpeed(int nSwitchSpeed, int nChannel);

        [DllImport(LibNameVersion)]
        public static extern int MCC_EcatHome();

        [DllImport(LibNameVersion)]
        public static extern int MCC_EcatGetGoHomeStatus();

        [DllImport(LibNameVersion)]
        public static extern int MCC_EcatAbortHome();


        // Get Home Sensor Signal
        [DllImport(LibNameVersion)]
        public static extern int MCC_GetHomeSensorStatus(ref ushort pwStatus, ushort wChannel, ushort wCardIndex = 0);
        
        #endregion Functions
        
        #region Obsolete functions

        //////////////////////////////////////////////////////////////////////////////
        // Obsolete functions in earlier MCCL version (just for compatibility)

        // Set/Get configuration of the homing process
        [DllImport(LibNameVersion)]
        public static extern int MCC_SetHomeConfig(ref SYS_HOME_CONFIG pstHomeConfig, ushort wChannel, ushort wCardIndex = 0);
        [DllImport(LibNameVersion)]
        public static extern int MCC_GetHomeConfig(ref SYS_HOME_CONFIG pstHomeConfig, ushort wChannel, ushort wCardIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern int MCC_SetEncoderConfig(ref SYS_ENCODER_CONFIG pstEncoderConfig, ushort wChannel, ushort wCardIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern int MCC_GetEncoderConfig(ref SYS_ENCODER_CONFIG pstEncoderConfig, ushort wChannel, ushort wCardIndex = 0);


        [DllImport(LibNameVersion)]
        public static extern int MCC_LineX(double dfX, ushort wGroupIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern int MCC_LineY(double dfY, ushort wGroupIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern int MCC_LineZ(double dfZ, ushort wGroupIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern int MCC_LineU(double dfU, ushort wGroupIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern int MCC_LineV(double dfV, ushort wGroupIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern int MCC_LineW(double dfW, ushort wGroupIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern int MCC_LineA(double dfA, ushort wGroupIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern int MCC_LineB(double dfB, ushort wGroupIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern int MCC_AbortMotion(ushort wGroupIndex, bool bAfterCurCmd);
        

        // Helica motion
        [DllImport(LibNameVersion)]
        public static extern int MCC_HelicaXY_Z(double dfCX, double dfCY, double dfPos, double dfPitch, byte byCirDir, ushort wGroupIndex = 0);
        
        [DllImport(LibNameVersion)]
        public static extern int MCC_HelicaYZ_X(double dfCY, double dfCZ, double dfPos, double dfPitch, byte byCirDir, ushort wGroupIndex = 0);
        
        [DllImport(LibNameVersion)]
        public static extern int MCC_HelicaZX_Y(double dfCZ, double dfCX, double dfPos, double dfPitch, byte byCirDir, ushort wGroupIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern int MCC_ConeXY_Z(double dfCX, double dfCY, double dfPos, double dfPitch, double CenterRate, byte byCirDir, ushort wGroupIndex = 0);
        
        [DllImport(LibNameVersion)]
        public static extern int MCC_ConeYZ_X(double dfCY, double dfCZ, double dfPos, double dfPitch, double CenterRate, byte byCirDir, ushort wGroupIndex = 0);
        
        [DllImport(LibNameVersion)]
        public static extern int MCC_ConeZX_Y(double dfCZ, double dfCX, double dfPos, double dfPitch, double CenterRate, byte byCirDir, ushort wGroupIndex = 0);
        

        [DllImport(LibNameVersion)]
        public static extern double MCC_ChangeFeedSpeed(double dfSpeed, ushort wGroupIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern double MCC_SetOverSpeed(double dfRate, ushort wGroupIndex = 0);

        [DllImport(LibNameVersion)]
        public static extern double MCC_GetOverSpeed(ushort wGroupIndex = 0);
        
        [DllImport(LibNameVersion)]
        public static extern double MCC_FixSpeed(bool bFix, ushort wGroupIndex = 0);
        
        [DllImport(LibNameVersion)]
        public static extern int MCC_EnableAccDecAfterIPO(ushort wGroupIndex);
        
        [DllImport(LibNameVersion)]
        public static extern int MCC_DisableAccDecAfterIPO(ushort wGroupIndex);

        [DllImport(LibNameVersion)]
        public static extern int MCC_EnableMovingAverage(ushort wGroupIndex);

        [DllImport(LibNameVersion)]
        public static extern int MCC_DisableMovingAverage(ushort wGroupIndex);
        

        [DllImport(LibNameVersion)]
        public static extern int MCC_EcatSetControlCode(ushort wControlCode, int nChannel);

        [DllImport(LibNameVersion)]
        public static extern int MCC_EcatGetStatusCode(ref ushort pwStatusCode, int nChannel);

        [DllImport(LibNameVersion)]
        public static extern int MCC_EcatGetControlCode(ref ushort pwControlCode, int nChannel);

        [DllImport(LibNameVersion)]
        public static extern int MCC_EcatSetTargetPos(int nTargetPos, int nChannel);

        [DllImport(LibNameVersion)]
        public static extern int MCC_EcatStartAction(uint dwSlaveId);

        [DllImport(LibNameVersion)]
        public static extern int MCC_EcatGetTargetPos(ref int pnTargetPos, uint dwSlaveId);
        
        [DllImport(LibNameVersion)]
        public static extern int MCC_EcatSetInverterPowerOn(int bEnable);//for HS


        //////////////////////////////////////////////////////////////////////////////
        // Robot Func.
        // Customize kinematics transformation rules

        // Point-to-Point Motion
        [DllImport(LibNameVersion)]
        public static extern int MCC_PtP_V6(double j0, double j1, double j2, double j3, double j4, double j5, double j6, double j7, ushort wGroupIndex = 0, UInt32 dwAxisMask = EtherCATSeries.MCCL.EMP_AXIS_ALL);

        // Linear Motion
        [DllImport(LibNameVersion)]
        public static extern int MCC_Line_V6(double x, double y, double z, double rx, double ry, double rz, bool bOverrideMotion = false, ushort wGroupIndex = 0, UInt32 dwAxisMask = EtherCATSeries.MCCL.EMP_AXIS_ALL);

        // Circular Motion
        [DllImport(LibNameVersion)]
        public static extern int MCC_Arc_V6(double x0, double y0, double z0, double x1, double y1, double z1, double rx, double ry, double rz, UInt32 posture = 0, ushort wGroupIndex = 0, UInt32 dwAxisMask = EtherCATSeries.MCCL.EMP_AXIS_ALL);
        // x0, ref. point for x axis
        // y0, ref. point for y axis
        // z0, ref. point for z axis
        // x1, target point for x axis
        // y1, target point for y axis
        // z1, target point for z axis
        // rx, target point for x orientation
        // ry, target point for y orientation
        // rz, target point for z orientation

        // Circular Motion
        [DllImport(LibNameVersion)]
        public static extern int MCC_ArcTheta_V6(double cx, double cy, double cz, double nv0, double nv1, double nv2, double theta, double rx, double ry, double rz, UInt32 posture = 0, ushort wGroupIndex = 0, UInt32 dwAxisMask = EtherCATSeries.MCCL.EMP_AXIS_ALL);
        // cx,   center point for x axis
        // cy,   center point for y axis
        // cz,   center point for z axis
        // nv0,  normal vector for x direction
        // nv1,  normal vector for y direction
        // nv2,  normal vector for z direction
        // theta,degree, +/- stands for direction
        // rx,   target point for x orientation
        // ry,   target point for y orientation
        // rz,   target point for z orientation

        // Circular Motion
        [DllImport(LibNameVersion)]
        public static extern int MCC_CircleXY_V6(double cx, double cy, double rx, double ry, double rz, byte byCirDir, UInt32 posture = 0, ushort wGroupIndex = 0);
        // cx, center point for x axis
        // cy, center point for y axis
        // rx, target point for x orientation
        // ry, target point for y orientation
        // rz, target point for z orientation

        // Circular Motion
        [DllImport(LibNameVersion)]
        public static extern int MCC_CircleYZ_V6(double cy, double cz, double rx, double ry, double rz, byte byCirDir, UInt32 posture = 0, ushort wGroupIndex = 0);
        // cy, center point for y axis
        // cz, center point for z axis
        // rx, target point for x orientation
        // ry, target point for y orientation
        // rz, target point for z orientation
        // CW or CCW

        // Circular Motion
        [DllImport(LibNameVersion)]
        public static extern int MCC_CircleZX_V6(double cz, double cx, double rx, double ry, double rz, byte byCirDir, UInt32 posture = 0, ushort wGroupIndex = 0);
        // cz, center point for z axis
        // cx, center point for x axis
        // rx, target point for x orientation
        // ry, target point for y orientation
        // rz, target point for z orientation
        // CW or CCW

        // Circular Motion
        [DllImport(LibNameVersion)]
        public static extern int MCC_Circle_V6(double x0, double y0, double z0, double x1, double y1, double z1, double rx, double ry, double rz, UInt32 posture = 0, ushort wGroupIndex = 0, UInt32 dwAxisMask = EtherCATSeries.MCCL.EMP_AXIS_ALL);
        // x0, 1st ref. point for x axis
        // y0, 1st ref. point for y axis
        // z0, 1st ref. point for z axis
        // x1, 2nd ref. point for x axis
        // y1, 2nd ref. point for y axis
        // z1, 2nd ref. point for z axis
        // rx, target point for x orientation
        // ry, target point for y orientation
        // rz, target point for z orientation

        #endregion //Obsolete functions

    }

    #region Structure definitions

    [StructLayout(LayoutKind.Sequential)]
    public struct COMMAND_INFO
    {
        public int nType;
        public int nCommandIndex;
        public double dfFeedSpeed;
        
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = EtherCATSeries.MCCL.MAX_AXIS_NUM)]
        public double[] dfPos;
        
        public int nMotionPhase;
    };

    [StructLayout(LayoutKind.Sequential)]
    public struct SYS_MAC_PARAM
    {
        public ushort wPosToEncoderDir;
        public ushort wRPM;
        public uint dwPPR;
        public double dfPitch;
        public double dfGearRatio;
        public double dfHighLimit;
        public double dfLowLimit;
        public double dfHighLimitOffset;
        public double dfLowLimitOffset;

        public ushort wPulseMode;
        public ushort wPulseWidth;
        public ushort wCommandMode;
        public ushort wPaddle;

        public ushort wOverTravelUpSensorMode;
        public ushort wOverTravelDownSensorMode;
    };
    
    [StructLayout(LayoutKind.Sequential)]
    public struct SYS_COMP_PARAM
    {
        public uint dwInterval;
        public ushort wHome_No;
        public ushort wPaddle;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = EtherCATSeries.MCCL.MAX_COMP_POINT)]
        public int[] nForwardTable;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = EtherCATSeries.MCCL.MAX_COMP_POINT)]
        public int[] nBackwardTable;
    };

    [StructLayout(LayoutKind.Sequential)]
    public struct SYS_CARD_CONFIG
    {
        public ushort wCardType;
        public ushort wCardAddress;
        public ushort wIRQ_No;
        public ushort wPaddle;
    };

    #region Obsolete Structure definitions
    //////////////////////////////////////////////////////////////////////////////
    // Obsolete functions in earlier MCCL version (just for compatibility)

    [StructLayout(LayoutKind.Sequential)]
    public struct SYS_ENCODER_CONFIG
    {
        public ushort wType;
        public ushort wAInverse;
        public ushort wBInverse;
        public ushort wCInverse;
        public ushort wABSwap;
        public ushort wInputRate;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
        public ushort[] wPaddle;
    };

    [StructLayout(LayoutKind.Sequential)]
    public struct SYS_HOME_CONFIG
    {
        public ushort wMode;
        public ushort wDirection;
        public ushort wSensorMode;
        public ushort wPaddel0;

        public int nIndexCount;
        public int nPaddel1;

        public double dfAccTime;
        public double dfDecTime;
        public double dfHighSpeed;
        public double dfLowSpeed;
        public double dfOffset;
    };
    
    #endregion //Obsolete Structure definitions

    #endregion //Structure definitions

}
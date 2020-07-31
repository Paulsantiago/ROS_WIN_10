#ifndef _MCCL_H_
#define _MCCL_H_

#ifdef __cplusplus
	extern "C" {
#endif
		
/////////////////////////////////////////////////////////////////////
// Variable definitions

// Comment the next line if the platform is non-Windows
#define UNDER_WINDOWS

// Comment the next line if supports single card only
#define MULTI_CARD_SUPPORT

#define _WINSOCKAPI_
#ifdef UNDER_WINDOWS
	#include <windows.h>
#else
	#undef DWORD
	#undef WORD
	#undef BYTE
	#undef BOOL

  #define STATIC_LIB 
	typedef unsigned long			DWORD;
	typedef unsigned short			WORD;
	typedef unsigned char			BYTE;
	typedef int						BOOL;
#endif


#ifdef STATIC_LIB
	#define MCC_CALLBACK
#else
	#define MCC_CALLBACK				__stdcall
#endif


#ifdef MULTI_CARD_SUPPORT
	#define MAX_CARD_NUM				12
#else
	#define MAX_CARD_NUM				1
#endif


/////////////////////////////////////////////////////////////////////
// Common Definitions

#define MAX_AXIS_NUM                    8
#define MAX_GROUP_NUM				(MAX_CARD_NUM * MAX_AXIS_NUM)

#define MOTION_QUEUE_SIZE			10000

#define GROUP_VALID						0
#define GROUP_INVALID					-1
#define AXIS_INVALID					-1

                              
#define _YES_                           1
#define _NO_                            0
	

#define NO_ERROR							0
#define ERROR_NO_DOG						1
#define ERROR_RTX_INIT_ERROR				2
#define	MCC_VERSION						"EC V1.0"
	

/////////////////////////////////////////////////////////////////////
// Range Definitions of Interpolation Period (ms)

#define IPO_PERIOD_MIN                  1
#define IPO_PERIOD_MAX					50
#define IPO_PERIOD_DEFAULT				5


/////////////////////////////////////////////////////////////////////
// Definitions of Output Command Modes

#define OCM_PULSE						0
#define OCM_VOLTAGE						1
#define OCM_SWCLOSELOOP                 2



/////////////////////////////////////////////////////////////////////
// Definitions of Sensor Logic

#define SL_NORMAL_OPEN					0
#define SL_NORMAL_CLOSE					1
#define SL_UNUSED						2



/////////////////////////////////////////////////////////////////////
// Definition(s) of motion card types

#define EPCIO_4_AXIS_ISA_CARD		0
#define EPCIO_6_AXIS_ISA_CARD		1
#define EPCIO_4_AXIS_PCI_CARD		2
#define EPCIO_6_AXIS_PCI_CARD		3
#define IMP_II_8_AXIS_PCI_CARD      4
#define IMP_III_8_AXIS_PCI_CARD     5
#define EPCIO_6_AXIS_PCIE_CARD	    6
#define EMP_MULTI_AXES			    7

	

/////////////////////////////////////////////////////////////////////
// Definitions of circular directions

#define CIR_CW										0 // clockwise
#define CIR_CCW										1 // counter-clockwise


/////////////////////////////////////////////////////////////////////
// Definitions of Group Motion Status

#define GMS_RUNNING							0
#define GMS_STOP							1
#define GMS_HOLD							2
#define GMS_DELAYING						3

	
/////////////////////////////////////////////////////////////////////
// Definitions of MCCL Axis Flag

#define EMP_AXIS_X								0x0001
#define EMP_AXIS_Y								0x0002
#define EMP_AXIS_Z								0x0004
#define EMP_AXIS_U								0x0008
#define EMP_AXIS_V								0x0010
#define EMP_AXIS_W								0x0020
#define EMP_AXIS_A                  			0x0040
#define EMP_AXIS_B                 				0x0080
#define EMP_AXIS_ALL             				0x00FF


/////////////////////////////////////////////////////////////////////
// MCCL Error Codes Definitions

#define NO_ERR							  0
#define INITIAL_MOTION_ERR				 -1
#define COMMAND_BUFFER_FULL_ERR			 -2
#define COMMAND_NOTACCEPTED_ERR			 -3 
#define COMMAND_NOTFINISHED_ERR			 -4
#define PARAMETER_ERR					 -5
#define GROUP_PARAMETER_ERR				 -6
#define FEED_RATE_ERR					 -7
#define BLEND_COMMAND_NOTCALLED_ERR		 -8
#define VOLTAGE_COMMAND_CHANNEL_ERR		 -9
#define HOME_COMMAND_NOTCALLED_ERR		-10
#define HOLD_ILLEGAL_ERR				-11
#define CONTI_ILLEGAL_ERR				-12
#define ABORT_ILLEGAL_ERR				-13
#define RUN_TIME_ERR					-14
#define ABORT_NOT_FINISH_ERR			-15
#define GROUP_RAN_OUT_ERR				-16



/////////////////////////////////////////////////////////////////////
// Compensation-related Definitions

#define MAX_COMP_POINT					256


/////////////////////////////////////////////////////////////////////
// EtherCAT Type Definitions

#define ECAT_ERROR_NONE						0
#define ECAT_ERROR_FAULT					1
#define ECAT_ERROR_WARNING					2


/////////////////////////////////////////////////////////////////////
// Type Definitions

typedef struct _COMMAND_INFO
{
	int     nType;
	int     nCommandIndex;
	double  dfFeedSpeed;
	double  dfPos[MAX_AXIS_NUM];
	int     nMotionPhase;
} COMMAND_INFO;

typedef struct _SYS_MAC_PARAM
{
	WORD    wPosToEncoderDir;
	WORD    wRPM;
	DWORD   dwPPR;
	double  dfPitch;
	double  dfGearRatio;
	double  dfHighLimit;
	double  dfLowLimit;
	double  dfHighLimitOffset;
	double  dfLowLimitOffset;

	WORD    wPulseMode;
	WORD    wPulseWidth;
	WORD    wCommandMode;
	WORD    wPaddle;

	WORD    wOverTravelUpSensorMode;
	WORD    wOverTravelDownSensorMode;
} SYS_MAC_PARAM;

typedef struct _SYS_COMP_PARAM
{
	DWORD   dwInterval;
	WORD    wHome_No;
	WORD    wPaddle;
	int     nForwardTable[MAX_COMP_POINT];
	int     nBackwardTable[MAX_COMP_POINT];
} SYS_COMP_PARAM;

typedef struct _SYS_CARD_CONFIG
{
	WORD    wCardType;
	WORD    wCardAddress;
	WORD    wIRQ_No;
	WORD    wPaddle;
} SYS_CARD_CONFIG;



//////////////////////////////////////////////////////////////////////////////
// Obsolete Structure definitions in earlier version of MCCL (existed only for
// compatibility and should not be used anymore)

typedef struct _SYS_ENCODER_CONFIG
{              
	WORD    wType;
	WORD    wAInverse;
	WORD    wBInverse;
	WORD    wCInverse;
	WORD    wABSwap;
	WORD    wInputRate;
	WORD    wPaddle[2];
} SYS_ENCODER_CONFIG, ENCODER_CONFIG;

typedef struct _SYS_HOME_CONFIG
{              
	WORD    wMode;   
	WORD    wDirection; 
 	WORD    wSensorMode;
	WORD    wPaddel0;

	int     nIndexCount;
	int     nPaddel1;

	double  dfAccTime;
	double  dfDecTime;
	double  dfHighSpeed;
	double  dfLowSpeed;
	double  dfOffset;
} SYS_HOME_CONFIG;


#ifdef __cplusplus
	}
#endif

#endif // _MCCL_H_


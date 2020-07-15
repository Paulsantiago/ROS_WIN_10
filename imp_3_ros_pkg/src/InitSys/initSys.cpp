
 // %Tag(FULLTEXT)%
 // %Tag(ROS_HEADER)%
#include "ros/ros.h"
// %EndTag(ROS_HEADER)%
// %Tag(MSG_HEADER)%
#include "std_msgs/String.h"
// %EndTag(MSG_HEADER)%
#include <conio.h>
#include <stdio.h>
#include <sstream>
#include "../include/MCCL_Fun.h"
//#include "../include/MCCL_Fun.h"
// %Tag(MCC_HEADER)%
//definitions
#define CARD_INDEX				    0
#define BASE_ADDRESS            0x240  //in PCI, user can input arbitrary number
#define IRQ_NO                      5  //in PCI, user can input arbitrary number
#define INTERPOLATION_TIME          2  // ms
#define ESC_KEY                    27
#define AXIS_NUM                    2
int g_nGroupIndex = -1;
double dfCurPosX, dfCurPosY, dfCurPosZ, dfCurPosU, dfCurPosV, dfCurPosW, dfCurPosA, dfCurPosB;
double dfPosX ;
double dfPosY ;
int initStatus = -1 ; 


/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
int main(int argc, char** argv)
{
	
  //#pragma Initialization MCCL 
    char cKey;
	WORD wCardType;
	int	 nRet, nCommandMode;

	SYS_MAC_PARAM      stMacParam;
	SYS_ENCODER_CONFIG stENCConfig;
	SYS_CARD_CONFIG    stCardConfig[AXIS_NUM];
	 wCardType    = 2;
	nCommandMode = OCM_PULSE;//  P Command
	int ncard=0;

	//int connectOk = MCC_Init("192.168.0.2", &ncard, 8); //0:succful // Set IP address for EMP-Z

	//if (connectOk == 0)
	//{
	//	printf("To connect EMP-Z is successful!\r\n");
	//MCC_CloseSystem();//  close motion control C library (MCCL)
	int resp = MCC_SetSysMaxSpeed(100);//  set max. feed rate
	printf("resp %d" , resp);
	    //  set mechanism parameters
	stMacParam.wPosToEncoderDir          = 0;
	stMacParam.dwPPR			         = 1000;
	stMacParam.wRPM                      = 3000; 
	stMacParam.dfPitch                   = 1.0;
	stMacParam.dfGearRatio               = 1.0;
	stMacParam.dfHighLimit               = 50000.0;
	stMacParam.dfLowLimit                = -50000.0; 
	stMacParam.dfHighLimitOffset         = 0;
	stMacParam.dfLowLimitOffset          = 0;
	stMacParam.wPulseMode                = DDA_FMT_PD; 
	stMacParam.wPulseWidth               = 100; 
	stMacParam.wCommandMode              = nCommandMode;
	stMacParam.wOverTravelUpSensorMode   = 2;//  not checking
	stMacParam.wOverTravelDownSensorMode = 2;

	//  set encoder configures
	stENCConfig.wType                    = ENC_TYPE_AB;
	stENCConfig.wAInverse                = _NO_;
	stENCConfig.wBInverse                = _NO_;
	stENCConfig.wCInverse                = _NO_;
	stENCConfig.wABSwap                  = _NO_;
	stENCConfig.wInputRate               = 4;//  set encoder input rate : x4

	for (WORD wChannel = 0;wChannel < AXIS_NUM;wChannel++)
	{
		nRet = MCC_SetMacParam(&stMacParam, wChannel, CARD_INDEX);      //  mechanism parameters are the same for all axes
			printf(" \n MCC_SetMacParam : %d", nRet);
		nRet = MCC_SetEncoderConfig(&stENCConfig, wChannel, CARD_INDEX);//  encoder configures are the same for all axes
			printf(" \n MCC_SetEncoderConfig : %d", nRet);
	}


	//  set group parameters
	MCC_CloseAllGroups();
	g_nGroupIndex = MCC_CreateGroup(0, 1, 2, 3, 4, 5, 6, 7, CARD_INDEX);
printf(" \n g_nGroupIndex : %d", g_nGroupIndex);
	if( g_nGroupIndex < GROUP_VALID )
	{
		printf("Groups create error !\n\n");
		return -1;
	}


	//  stCardConfig is used to set card's base address and card style, and set one card's attributes now
	stCardConfig[CARD_INDEX].wCardType    = 4;   //  4 : 8-axis card
	stCardConfig[CARD_INDEX].wCardAddress = 0x240;//  base address, PCI card ignores this setting
	stCardConfig[CARD_INDEX].wIRQ_No      = 5;      //  IRQ No., PCI card ignores this setting
	stCardConfig[CARD_INDEX].wPaddle      = 0;
	
	nRet = MCC_InitSystem(INTERPOLATION_TIME,// set interpolation time interval and get some errors happening or not
						  stCardConfig,
						  1);		         //  only use one card

	printf(" \n Simu error !  Return Value : %d", nRet);
	if (nRet == NO_ERR)
	{
		printf("Initialization is successfull !\n\n");
		MCC_SetServoOn(0, CARD_INDEX);//  set channel 0 servv on 
		MCC_SetServoOn(1, CARD_INDEX);//  set channel 1 servv on 

		MCC_SetAbsolute(g_nGroupIndex);     //  use Absolute coordinate mode
		//  you must regulate accleration and deceleration time depending on different speed for a smooth moving		
		//  set line, arc and circle motion's accleration time
		MCC_SetAccTime(300, g_nGroupIndex);//  set accleration time to be 300 ms
		 
		//  set line, arc and circle motion's deceleration time
		MCC_SetDecTime(300, g_nGroupIndex);//  set decleration time to be 300 ms

		MCC_SetFeedSpeed(10, g_nGroupIndex);//  set line, arc and circle motion's feed rate (unit : mm/sec)

		nRet = MCC_Line(100, 10, 0, 0, 0, 0, 0, 0, g_nGroupIndex);
		if (nRet < NO_ERR)
			printf(" MCC_Line() error !  Return Value : %d", nRet);
		MCC_DelayMotion(200);

	}
	else
	{
		MCC_CloseSystem();//  close motion control C library (MCCL)
		printf("Motion Initialization Error !\n\n");
		return 0;									  
	}

	while (1)
	{
		if (_kbhit())
		{
			cKey = getch();

			if (cKey == ESC_KEY)
			{
				MCC_AbortMotionEx(150, g_nGroupIndex);//  stop current motion with a decelerating style and discard all motion commands in a command queue
				// if you want to stop current motion immediatly, use the first parameter be 0
				break;
			}
		}

		//  get current position (unit : mm)
		MCC_GetCurPos(&dfCurPosX, &dfCurPosY, &dfCurPosZ, 
					  &dfCurPosU, &dfCurPosV, &dfCurPosW, 
					  &dfCurPosA, &dfCurPosB, g_nGroupIndex);

		printf("Current Pos. : x = %6.2f    y = %6.2f    \r", dfCurPosX, dfCurPosY);
		MCC_TimeDelay(10);
	}
	MCC_SetServoOff(0, CARD_INDEX);//  set channel 0 servv on 
	MCC_SetServoOff(1, CARD_INDEX);//  set channel 1 servv on 

	MCC_CloseSystem();//  close motion control C library (MCCL)


    printf("rsp %d" , resp);
    
	
	return 0;
}
// %EndTag(FULLTEXT)%
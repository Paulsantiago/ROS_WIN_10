// // // %Tag(ROS_HEADER)%
#include "ros/ros.h"
// %EndTag(ROS_HEADER)%
 #include "std_msgs/String.h"
 #include "std_msgs/Int32.h"
 #include "std_msgs/Bool.h"
// %EndTag(MCC_HEADER)%
#include <conio.h>
 #include <stdio.h>
 #include <sstream>
 #include <geometry_msgs/Twist.h>
 #include <geometry_msgs/Pose.h>
 #include <iostream>

#include "emz_ros_pkg/Joint.h"
#include <emz_ros_pkg/CartesianPoint.h>

#include "../include/MCCL.h"
 #include "../include/NCCL.h"



 using namespace std;
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

emz_ros_pkg::CartesianPoint val_to_Publish;
ros::Publisher inPos_publisher_;
std_msgs::Int32 inPos;


void tUpdatePosition(const ros::TimerEvent& event ,ros::Publisher CurCpos_publisher_ )
{
	if(initStatus== NO_ERR)
	{
		MCC_GetCurPos(&dfCurPosX, &dfCurPosY, &dfCurPosZ, 
					&dfCurPosU, &dfCurPosV, &dfCurPosW, 
					&dfCurPosA, &dfCurPosB, g_nGroupIndex);
		inPos.data = MCC_GetMotionStatus(g_nGroupIndex);
		
		val_to_Publish.X=dfCurPosX; 
		val_to_Publish.Y=dfCurPosY; 
		val_to_Publish.Z=dfCurPosZ; 
		val_to_Publish.RX=dfCurPosU;
		val_to_Publish.RY=dfCurPosV;
		val_to_Publish.RZ=dfCurPosW;
		printf("x: %f , y %F \n",dfCurPosX,dfCurPosY);
		CurCpos_publisher_.publish(val_to_Publish);
		inPos_publisher_.publish(inPos);
		
	}
}



void ptp_subscriberCallback(const emz_ros_pkg::Joint & jnt_msg)
{
	int nRet = MCC_PtP(jnt_msg.J1,
						jnt_msg.J2,
						jnt_msg.J3,
						jnt_msg.J4, 
						jnt_msg.J5, 
						jnt_msg.J6, 0, 0, g_nGroupIndex);

}
void line_subscriberCallback(const emz_ros_pkg::CartesianPoint & pointMsg)
{
	int nRet = MCC_Line(pointMsg.X,
						pointMsg.Y,
						pointMsg.Z,
						pointMsg.RX, 
						pointMsg.RY, 
						pointMsg.RZ, 0, 0, g_nGroupIndex);

	if (nRet < NO_ERR)
			printf(" MCC_Line() error !  Return Value : %d", nRet);
	MCC_DelayMotion(2);
}



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

	int connectOk = MCC_Init("192.168.0.2", &ncard, 8); //0:succful // Set IP address for EMP-Z

	if (connectOk == 0)
	{
		printf("To connect EMP-Z is successful!\r\n");

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

		for (WORD wChannel = 0;wChannel < MAX_AXIS_NUM;wChannel++)
		{
			MCC_SetMacParam(&stMacParam, wChannel, CARD_INDEX);      //  mechanism parameters are the same for all axes
			MCC_SetEncoderConfig(&stENCConfig, wChannel, CARD_INDEX);//  encoder configures are the same for all axes
		}


		//  set group parameters
		MCC_CloseAllGroups();
		g_nGroupIndex = MCC_CreateGroup(0, 1, 2, 3, 4, 5, 6, 7, CARD_INDEX);

		if( g_nGroupIndex < GROUP_VALID )
		{
			printf("Groups create error !\n\n");
			return -1;
		}


		//  stCardConfig is used to set card's base address and card style, and set one card's attributes now
		stCardConfig[CARD_INDEX].wCardType    = 2;   //  4 : 8-axis card
		stCardConfig[CARD_INDEX].wCardAddress = 0x240;//  base address, PCI card ignores this setting
		stCardConfig[CARD_INDEX].wIRQ_No      = 5;      //  IRQ No., PCI card ignores this setting
		stCardConfig[CARD_INDEX].wPaddle      = 0;

		nRet = MCC_InitSystem(INTERPOLATION_TIME,//  interpolation time interval
			stCardConfig,
			1);		         //  only use one card

		if (nRet == NO_ERR)
		{
			printf("Initialization is successfull !\n\n");

			MCC_SetServoOn(0, CARD_INDEX);//  set channel 0 servv on 
			MCC_SetServoOn(1, CARD_INDEX);//  set channel 1 servv on 

			//MCC_SetIncrease(g_nGroupIndex);//  use increment coordinate mode(增量座標)

			printf("Press any key to start, and ESC key to quit !\n\n");
			//while (!_kbhit());

			//  you must regulate accleration and deceleration time depending on different speed for a smooth moving
			//  set line, arc and circle motion's accleration time
			MCC_SetAccTime(300, g_nGroupIndex);//  set accleration time to be 300 ms

			//  set line, arc and circle motion's deceleration time
			MCC_SetDecTime(300, g_nGroupIndex);//  set decleration time to be 300 ms

			//  set "point to point" motion's accleration time
			MCC_SetPtPAccTime(300, 300, 300, 300, 300, 300, 300, 300, g_nGroupIndex);//  300 ms

			//  set "point to point" motion's deceleration time
			MCC_SetPtPDecTime(300, 300, 300, 300, 300, 300, 300, 300, g_nGroupIndex);//  300 ms

			MCC_SetFeedSpeed(10, g_nGroupIndex);//  set line, arc and circle motion's feed rate (unit : mm/sec)

			MCC_Line(20, 10, 0, 0, 0, 0, 0, 0, g_nGroupIndex);
		}
		else
		{
			printf("Motion Initialization Error !\n\n");
			MCC_SetServoOff(0, CARD_INDEX);//  set channel 0 servv on 
  				MCC_SetServoOff(1, CARD_INDEX);//  set channel 1 servv on 
  				MCC_CloseSystem();
			return -1;									  
		}
	}
  
  //MCC_SetServoOff(0, CARD_INDEX);//  set channel 0 servv on 
  //MCC_SetServoOff(1, CARD_INDEX);//  set channel 1 servv on 
  //MCC_CloseSystem();
  // #pragma endregion
  //int resp = MCC_Line(20, 10, 0, 0, 0, 0, 0, 0, g_nGroupIndex);
  //printf("lin resp: %d", resp);
    ///////////////// mccl initializaton /////////////////////
   dfPosX = 1 ;
   dfPosY = 2 ;
   //		Ros definitions

	ros::init(argc, argv, "InitNCCL");
  	ros::NodeHandle nh;
	// %Tag(PUBLISHER)%
	ros::Subscriber line_subs = nh.subscribe("line_MCCL", 1, 	line_subscriberCallback);  
	ros::Subscriber ptp_subs = nh.subscribe("ptp_MCCL", 1, 		ptp_subscriberCallback);   
	ros::Publisher CurCpos_publisher_ = nh.advertise<emz_ros_pkg::CartesianPoint>("curcpos_MCCL", 1, true); 
	inPos_publisher_ = nh.advertise<std_msgs::Int32>("inPositionStatus", 1, true); 
	
	ros::Timer timer = nh.createTimer(ros::Duration(0.016), boost::bind(tUpdatePosition, _1, CurCpos_publisher_));
	 ros::spin();

	
return 0;	
}


				
				
#include "MCCL_ROS_MASTER.h"
#include "std_msgs/String.h"
ROS_MCCL::ROS_MCCL () {
  width = 5;
  height = 5;
}

ROS_MCCL::ROS_MCCL (int a, int b) {
  width = a;
  height = b;
}
ROS_MCCL::ROS_MCCL(ros::NodeHandle* nodehandle):nh_(*nodehandle)
{ // constructor
    ROS_INFO("***********in class constructor of ROS_MCCL initializating .... ");
    if(initParameters())
	{
		initializeSubscribers(); // package up the messy work of creating subscribers; do this overhead in constructor
		initializePublishers();
		initializeServices();
		//initTimer();
		//initialize variables here, as needed
		val_Point.X=0.0; 
		val_Point.Y=0.0; 
		val_Point.Z=0.0; 
		val_Point.RX=0.0;
		val_Point.RY=0.0;
		val_Point.RZ=0.0;

		val_Joint.J1 = 0.0;
		val_Joint.J2 = 0.0;
		val_Joint.J3 = 0.0;
		val_Joint.J4 = 0.0;
		val_Joint.J5 = 0.0;
		val_Joint.J6 = 0.0;
		// can also do tests/waits to make sure all required services, topics, etc are alive
		initStatus = true;
	}
	else
	{
		initStatus = false;

	}
    
}
void ROS_MCCL::initTimer()
{
	/*timerUpdate = nh_.createTimer(ros::Duration(0.01), boost::bind(&ROS_MCCL::tupdate, _1));
	
	ros::spinOnce();

	//update();

	if (!ros::ok())
	{
		  ROS_INFO("tiemr update problems");
	}*/

}
void ROS_MCCL::initializeSubscribers()
{
    ROS_INFO("Initializing Subscribers");
    line_subs = nh_.subscribe("line_MCCL", 1, 		&ROS_MCCL::line_subscriberCallback,this);  
	ptp_subs = nh_.subscribe("ptp_MCCL", 1, 		&ROS_MCCL::ptp_subscriberCallback,this);  
	close_subs = nh_.subscribe("close_MCCL", 1, 		&ROS_MCCL::close_subscriberCallback,this);  
	//inPos_subs = nh_.subscribe("inPositionStatus", 1, 		&ROS_MCCL::inPos_subscriberCallback,this);  
	//circle_subs = nh_.subscribe("circle_MCCL", 1, 	&ROS_MCCL::circle_subscriberCallback,this);  
	//arc_subs = nh_.subscribe("arc_MCCL", 1, 		&ROS_MCCL::arc_subscriberCallback,this);  
    // add more subscribers here, as needed
}

//member helper function to set up services:
// similar syntax to subscriber, required for setting up services outside of "main()"
void ROS_MCCL::initializeServices()
{
    ROS_INFO("Initializing Services");
    minimal_service_ = nh_.advertiseService("exampleMinimalService",
                                                   &ROS_MCCL::serviceCallback,
                                                   this);  
    // add more services here, as needed
}

//member helper function to set up publishers;
void ROS_MCCL::initializePublishers()
{
    ROS_INFO("Initializing Publishers");
    CurCpos_publisher_ = nh_.advertise<imp_3_ros_pkg::CartesianPoint>("curcpos_MCCL", 1, true); 
	//inPos_publisher_ = nh_.advertise<int>("inPositionStatus", 1, true); 
    //add more publishers, as needed
    // note: COULD make minimal_publisher_ a public member function, if want to use it within "main()"
}
void ROS_MCCL::line_subscriberCallback(const imp_3_ros_pkg::CartesianPoint& pointMsg) {
    // the real work is done in this callback function
    // it wakes up every time a new message is published on "exampleMinimalSubTopic"
    val_Point = pointMsg; // copy the received data into member variable, so ALL member funcs of ExampleRosClass can access it
    ROS_INFO("myCallback activated: received value x: %f , y : %f ",pointMsg.X,pointMsg.Y);
	nRet = MCC_Line( pointMsg.X,
				 	pointMsg.Y,
					pointMsg.Z,
					pointMsg.RX, 
					pointMsg.RY, 
					pointMsg.RZ, 0, 0, g_nGroupIndex);
  	ROS_INFO("Line callback : %d" , nRet);

}
void ROS_MCCL::ptp_subscriberCallback(const imp_3_ros_pkg::Joint& jointMsg) {
    // it wakes up every time a new message is published on "exampleMinimalSubTopic"
    val_Joint = jointMsg; // copy the received data into member variable, so ALL member funcs of ExampleRosClass can access it
    ROS_INFO("myCallback activated: received value J1: %f , J2 : %f ",jointMsg.J1,jointMsg.J2);
	nRet = MCC_PtP(jointMsg.J1,
				 	jointMsg.J2,
					jointMsg.J3,
					jointMsg.J4, 
					jointMsg.J5, 
					jointMsg.J6, 0, 0, g_nGroupIndex);
  	ROS_INFO("MCC_PtP callback : %d" , nRet);
}

void ROS_MCCL::close_subscriberCallback(const std_msgs::Bool &close) {
    // it wakes up every time a new message is published on "exampleMinimalSubTopic"
   
    ROS_INFO("------------Close system ----------------------");
	MCC_CloseSystem();
	MCC_SetServoOff(0, CARD_INDEX);//  set channel 0 servv on 
	MCC_SetServoOff(1, CARD_INDEX);//  set channel 1 servv on 
	
  	ROS_INFO("MCC_CLOSE callback");
}


//member function implementation for a service callback function
bool ROS_MCCL::serviceCallback(imp_3_ros_pkg::SetSpeedRequest& request, imp_3_ros_pkg::SetSpeedResponse& response) {
    ROS_INFO("service callback activated");
    response.status = true; // boring, but valid response info
    return true;
}

void ROS_MCCL::tupdate(const ros::TimerEvent& event)
{
	//printf("initStatus: %d",initStatus);
	ros::spinOnce();
	if(initStatus)
	{
		MCC_GetCurPos(&dfCurPosX, &dfCurPosY, &dfCurPosZ, 
					&dfCurPosU, &dfCurPosV, &dfCurPosW, 
					&dfCurPosA, &dfCurPosB, g_nGroupIndex);

		int res = MCC_GetMotionStatus(g_nGroupIndex);
		val_to_Publish.X=dfCurPosX; 
		val_to_Publish.Y=dfCurPosY; 
		val_to_Publish.Z=dfCurPosZ; 
		val_to_Publish.RX=dfCurPosU;
		val_to_Publish.RY=dfCurPosV;
		val_to_Publish.RZ=dfCurPosW;
		//printf("x: %f",dfCurPosX);
		inPos_publisher_.publish(res);
		CurCpos_publisher_.publish(val_to_Publish);
		
	}
}

bool ROS_MCCL::initParameters()
{
    int resp = MCC_SetSysMaxSpeed(100);//  set max. feed rate
	printf("resp %d \n" , resp);
     wCardType    = 2;
	nCommandMode = OCM_PULSE;//  P Command
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


	//mechanism and ENC parameters are seperated from each other, we set them all the same.
	for (WORD wChannel = 0;wChannel < AXIS_NUM;wChannel++)
	{
		MCC_SetMacParam(&stMacParam, wChannel, CARD_INDEX);      //  mechanism parameters are the same for all axes
		MCC_SetEncoderConfig(&stENCConfig, wChannel, CARD_INDEX);//  encoder configures are the same for all axes
	}
    
	//  set group parameters
	MCC_CloseAllGroups();
	g_nGroupIndex = MCC_CreateGroup(0, 1, 2, 3, 4, 5, 6, 7,  CARD_INDEX);

	if( g_nGroupIndex < GROUP_VALID )
	{
		printf("Groups create error !\n\n");
		return false;
	}

	
	//  stCardConfig is used to set card's base address and card style, and set one card's attributes now
	stCardConfig[CARD_INDEX].wCardType    = wCardType;   //  2 : 4-axis card
								                         //  3 : 6-axis card
								                         //  4 : 8-axis card

	stCardConfig[CARD_INDEX].wCardAddress = BASE_ADDRESS;//  base address, PCI card ignores this setting
	stCardConfig[CARD_INDEX].wIRQ_No      = IRQ_NO;      //  IRQ No., PCI card ignores this setting
	stCardConfig[CARD_INDEX].wPaddle      = 0;
    // INIT
	//MCC_InitSimulation
	nRet = MCC_InitSystem(INTERPOLATION_TIME,// set interpolation time interval and get some errors happening or not
						  stCardConfig,
						  1);		         //  only use one card
	printf(" \n  MCC_InitSystem  Return Value : %d", nRet);

	if (nRet == NO_ERR)
	{
		printf("  Initialization is successfull initVal  = %d !\n\n" , nRet );
		MCC_SetServoOn(0, CARD_INDEX);//  set channel 0 servv on 
		MCC_SetServoOn(1, CARD_INDEX);//  set channel 1 servv on 

		MCC_SetAbsolute(g_nGroupIndex);     //  use Absolute coordinate mode
		//  you must regulate accleration and deceleration time depending on different speed for a smooth moving		
		//  set line, arc and circle motion's accleration time
		MCC_SetAccTime(300, g_nGroupIndex);//  set accleration time to be 300 ms
		//  set line, arc and circle motion's deceleration time
		MCC_SetDecTime(300, g_nGroupIndex);//  set decleration time to be 300 ms

		MCC_SetFeedSpeed(10, g_nGroupIndex);//  set line, arc and circle motion's feed rate (unit : mm/sec)

        return true;
	}
	else
	{
		printf("Motion Initialization Error , initError  = %d  !\n\n", nRet);
		return false;									  
	}
}





////////////////////////////////////////////////////////////////////////******************/////////////////////////////////////////////////////

/*void timerCallback(const ros::TimerEvent&, ROS_MCCL RosMCCLobj)
{
	RosMCCLobj.update();
}*/

int main (int argc, char **argv)
 {
    ros::init(argc, argv, "MCCL_ROS"); //node name
    
    
    ros::NodeHandle nh; // create a node handle; need to pass this to the class constructor
    ROS_INFO("main: instantiating an object of type MCCL_ROS");
    
	ROS_MCCL MCCL_ROS(&nh);  //instantiate an ExampleRosClass object and pass in pointer to nodehandle for constructor to use
	
	/*ros::Publisher chatter_publisher = nh.advertise<imp_3_ros_pkg::CartesianPoint>("chatter",1000);
	ros::Rate loop_rate(1.0);
	
	///ros::Timer timer = nh.createTimer(ros::Duration(0.016), boost::bind(timerCallback, _1, MCCL_ROS));
	//ros::Timer timer = nh.createTimer(ros::Duration(0.01), boost::bind(&ROS_MCCL::tupdate, _1, &MCCL_ROS));
	while (ros::ok())
	{
		imp_3_ros_pkg::CartesianPoint tpoint ; 
	tpoint.X = 50 ; 
	tpoint.Y = 30; 
	tpoint.Z = 80; 
	tpoint.RX = 40;
	tpoint.RY = 0;
	tpoint.RZ = 0;
	chatter_publisher.publish(tpoint);
	ROS_INFO(" Publishing" );
	ros::spinOnce();
	loop_rate.sleep();
	}*/

	ros::Timer timer = nh.createTimer(ros::Duration(0.015), &ROS_MCCL::tupdate, &MCCL_ROS);

	imp_3_ros_pkg::CartesianPoint tpoint ; 
	tpoint.X = 50 ; 
	tpoint.Y = 30; 
	tpoint.Z = 80; 
	tpoint.RX = 40;
	tpoint.RY = 0;
	tpoint.RZ = 0;
	MCCL_ROS.line_publisher.publish(tpoint);

	//exampleRosClassMCCL.CurCpos_publisher

	//ros::Publisher mccl_pub = nh.advertise<imp_3_ros_pkg::Point>("line_MCCL",1,true);

	//ros::Subscriber pose_sub = nh.subscribe("Curpos_MCCL", 1, poseCallback);
	

    ROS_INFO("main: going into spin; let the callbacks do all the work");
    ros::spin();


  return 0;
}




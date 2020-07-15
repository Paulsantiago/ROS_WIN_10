
// %EndTag(MSG_HEADER)%
//#include <conio.h>
//#include <stdio.h>
#include <ros/ros.h> //ALWAYS need to include this
#include "geometry_msgs/Twist.h"
 #include "std_msgs/Int32.h"
 #include "std_msgs/Bool.h"
#include "turtlesim/Pose.h"

#include "imp_3_ros_pkg/Joint.h"
#include <imp_3_ros_pkg/CartesianPoint.h>
#include <conio.h>
 #include <stdio.h>
 #include <sstream>
using namespace std;
/* topics : 
line_MCCL
ptp_MCCL
curcpos_MCCL
*/
ros::Publisher line_pub ; 
ros::Publisher close_pub ; 
ros::Publisher velocity_publisher;
ros::Subscriber cur_pos ;
ros::Subscriber in_pos ;
turtlesim::Pose turtlesim_pose ;

int inPos ; 

void getCurPosCallBack (const imp_3_ros_pkg::CartesianPoint & pointMsg)
{
    turtlesim_pose.x = pointMsg.X;
    turtlesim_pose.y = pointMsg.Y;
    turtlesim_pose.theta = 0 ;
  	printf("\n X: %f , Y: %f ", turtlesim_pose.x , turtlesim_pose.y);
}


/*void inPositionCallback (const int &resp)
{
  
   printf("position %d",resp);
   if (resp)
   {
	     ROS_INFO("\n\n\n******CLOSING TESTING************\n");
	   std_msgs::Bool ok;
	 //  ok.data = true;
	 // close_pub.publish(ok);
   }
   
}*/

int main(int argc, char **argv)
{
    ros::init(argc, argv, "use_init_mccl"); //node name
    ros::NodeHandle nh; // create a node handle; need to pass this to the class constructor
    line_pub = nh.advertise<imp_3_ros_pkg::CartesianPoint>("line_MCCL", 1, true); 
	close_pub = nh.advertise<std_msgs::Bool>("close_MCCL", 1, true); 
    
    cur_pos = nh.subscribe("curcpos_MCCL",200,getCurPosCallBack);
    //in_pos = nh.subscribe("inPositionStatus",500,inPositionCallback);
    imp_3_ros_pkg::CartesianPoint cPoint ; 
    /** test your code here **/
    ROS_INFO("\n\n\n******START TESTING************\n");
    cout<<"enter position X: ";
    cin>>cPoint.X;
    cout<<"enter position Y : ";
    cin>>cPoint.Y;

    cPoint.Z = 0 ;
    cPoint.RX = 0 ;
    cPoint.RY= 0 ;
    cPoint.RZ = 0 ;
    
     line_pub.publish(cPoint);
     
    ros::spin();
    return 0;
}

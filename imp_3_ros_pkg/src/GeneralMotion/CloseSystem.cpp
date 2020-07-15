
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

int inPos ; 

ros::Publisher close_pub ; 
int main(int argc, char **argv)
{
    ros::init(argc, argv, "close"); //node name
    ros::NodeHandle nh; // create a node handle; need to pass this to the class constructor
	close_pub = nh.advertise<std_msgs::Bool>("close_MCCL", 1, true); 
    
     ROS_INFO("\n\n\n******CLOSING TESTING************\n");
	 std_msgs::Bool ok;
	 ok.data = true;
	 close_pub.publish(ok);

    ros::spin();
    return 0;
}

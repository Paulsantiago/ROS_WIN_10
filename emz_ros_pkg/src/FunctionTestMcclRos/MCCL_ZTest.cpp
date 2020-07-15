
// %EndTag(MSG_HEADER)%
//#include <conio.h>
//#include <stdio.h>
#include <ros/ros.h> //ALWAYS need to include this
#include "geometry_msgs/Twist.h"
 #include "std_msgs/Int32.h"
 #include "std_msgs/Bool.h"
#include "turtlesim/Pose.h"

#include "emz_ros_pkg/Joint.h"
#include <emz_ros_pkg/CartesianPoint.h>
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
ros::Publisher velocity_publisher;
ros::Subscriber cur_pos ;
ros::Subscriber in_pos ;
turtlesim::Pose turtlesim_pose ;

int inPos ; 
void moveGoal(turtlesim::Pose  goal_pose, double distance_tolerance);
void movef(emz_ros_pkg::CartesianPoint cPoint);

void getCurPosCallBack (const emz_ros_pkg::CartesianPoint & pointMsg)
{
    turtlesim_pose.x = pointMsg.X;
    turtlesim_pose.y = pointMsg.Y;
    turtlesim_pose.theta = 0 ;
  //	printf("X: %f , Y: %f ", turtlesim_pose.x , turtlesim_pose.y);
}

void inPositionCallback (const std_msgs::Int32 & inPosr)
{
   inPos = inPosr.data;
   //printf("position %d",inPos);
}



int main(int argc, char **argv)
{
    ros::init(argc, argv, "use_init_mccl"); //node name
    ros::NodeHandle nh; // create a node handle; need to pass this to the class constructor
    line_pub = nh.advertise<emz_ros_pkg::CartesianPoint>("line_MCCL", 1, true); 
    velocity_publisher = nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1000);
    cur_pos = nh.subscribe("curcpos_MCCL",100,getCurPosCallBack);
    in_pos = nh.subscribe("inPositionStatus",100,inPositionCallback);
    emz_ros_pkg::CartesianPoint cPoint ; 
    printf("Test is successfull !\n\n");
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
    turtlesim::Pose pose;
    pose.x=cPoint.X;
    pose.y=cPoint.Y;
    pose.theta=0;

    //line_pub.publish(cPoint);
    //movef(cPoint);
      line_pub.publish(cPoint);
      moveGoal(pose, 0.01);
  //  ROS_INFO("main: instantiating an object of type ExampleRosClass");
    //int a = 5;
    //ROS_MCCL ros_(a);  //instantiate an ExampleRosClass object and pass in pointer to nodehandle for constructor to use

   // ROS_INFO("main: going into spin; let the callbacks do all the work");
    ros::spin();
    return 0;
}
void movef(emz_ros_pkg::CartesianPoint cPoint)
{
  // printf("move !\n\n");
  //set a random linear velocity in the x-axis
 geometry_msgs::Twist vel_msg;
	vel_msg.linear.x =10;
	vel_msg.linear.y =0;
	vel_msg.linear.z =0;
	//set a random angular velocity in the y-axis
	vel_msg.angular.x = 0;
	vel_msg.angular.y = 0;
	vel_msg.angular.z = 0;

	//set a position desired
  double t0 = ros::Time::now().toSec();
  line_pub.publish(cPoint);

	double current_distance = 0.0;
	ros::Rate loop_rate(1);
	do{
		velocity_publisher.publish(vel_msg);
		double t1 = ros::Time::now().toSec();
    vel_msg.linear.x = turtlesim_pose.x ;
    vel_msg.linear.y = turtlesim_pose.y ;
		ros::spinOnce();
		loop_rate.sleep();
		cout<<(t1-t0)<<", "<<"vel:"<<vel_msg.linear.x<<", "<<endl;
	}while(inPos==0);
	vel_msg.linear.x =0;
	velocity_publisher.publish(vel_msg);

}
/*void poseCallback(const turtlesim::Pose::ConstPtr & pose_message){
	turtlesim_pose.x=pose_message->x;
	turtlesim_pose.y=pose_message->y;
	turtlesim_pose.theta=pose_message->theta;
}
*/
double getDistance(double x1, double y1, double x2, double y2){
	return sqrt(pow((x1-x2),2)+pow((y1-y2),2));
}

void moveGoal(turtlesim::Pose  goal_pose, double distance_tolerance){

	geometry_msgs::Twist vel_msg;

	ros::Rate loop_rate(100);
	double E = 0.0;
	do{
		/****** Proportional Controller ******/
		//linear velocity in the x-axis
		double Kp=0.5;
		double Ki=0.02;
		//double v0 = 2.0;
		//double alpha = 0.5;
		double e = getDistance(turtlesim_pose.x, turtlesim_pose.y, goal_pose.x, goal_pose.y);
		double E = E+e;
		//Kp = v0 * (exp(-alpha)*error*error)/(error*error);
		vel_msg.linear.x = (Kp*e);
		vel_msg.linear.y =0;
		vel_msg.linear.z =0;
		//angular velocity in the z-axis
		vel_msg.angular.x = 0;
		vel_msg.angular.y = 0;
		vel_msg.angular.z =4*(atan2(goal_pose.y-turtlesim_pose.y, goal_pose.x-turtlesim_pose.x)-turtlesim_pose.theta);

		velocity_publisher.publish(vel_msg);

		ros::spinOnce();
		loop_rate.sleep();

	}while(getDistance(turtlesim_pose.x, turtlesim_pose.y, goal_pose.x, goal_pose.y)>distance_tolerance);
	cout<<"end move goal"<<endl;
	vel_msg.linear.x =0;
	vel_msg.angular.z = 0;
	velocity_publisher.publish(vel_msg);
}

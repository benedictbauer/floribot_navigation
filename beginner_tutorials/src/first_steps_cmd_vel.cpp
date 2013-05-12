/*
 * first_steps_cmd_vel.cpp
 *
 *  Created on: Apr 29, 2013
 *      Author: benedict
 */


#include <ros/ros.h>
#include <geometry_msgs/Twist.h>


int main(int argc, char **argv)
{

	ros::init(argc, argv, "first_steps_cmd_vel");

	ros::NodeHandle n;

	ros::Publisher cmd_vel_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1);

	ros::Rate loop_rate(10);



	while (ros::ok())
	{
		geometry_msgs::Twist cmd_vel;

		cmd_vel.linear.x = 1;

		cmd_vel_pub.publish(cmd_vel);

		ros::spinOnce();

		loop_rate.sleep();

	}


	return 0;
}



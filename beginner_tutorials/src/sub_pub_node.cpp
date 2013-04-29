/*
 * sub_pub_node.cpp
 *
 *  Created on: Apr 29, 2013
 *      Author: benedict
 */
/*
 * 	size_t num_ranges = scan->ranges.size();
	double ranges[num_ranges];

	for (int i = 0; i < num_ranges; i++)
	{
		ranges[i] = 1 / scan->ranges[i];
	}

	double polar_histo[num_ranges];

	for (int i = 0; i < num_ranges; i++)
	{
		ranges[i] = 1 / scan->ranges[i];
	}

	double min;
	int i;
	float search_range = ;

	min = polar_histo[0];

	for (i = 0; i < num_ranges; i++)
	{
		if (polar_histo[i]<search_range)
		{
			if (polar_histo[i] < min)
			{
			    min = polar_histo[i];
			}
		}
	}

	printf("%i\n", i);

	if (i > num_ranges/2)
	{
		angular = 0.2;
	}
	else
	{
		angular =-0.2;
	}
 */


#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Twist.h>



ros::Publisher cmd_vel_pub;

void scan_sub_callback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
	size_t num_ranges = scan->ranges.size();
	double polar_histo[num_ranges];

	for (int i = 0; i < num_ranges; i++)
	{
		polar_histo[i] = 1 / scan->ranges[i];
	}

	double min;
	int i;
	float search_range = 1 / 2.0;

	min = polar_histo[0];

	for (i = 0; i < num_ranges; i++)
	{
		if (polar_histo[i]<search_range)
		{
			if (polar_histo[i] < min)
			{
			    min = polar_histo[i];
			}
		}
	}

	printf("%i\n", i);

	float drehung;

	if (i > num_ranges/2)
	{
		drehung = 0.2;
	}
	else
	{
		drehung =-0.2;
	}

	while (ros::ok())
	{
		geometry_msgs::Twist cmd_vel;

		cmd_vel.linear.x = 1;
		cmd_vel.angular.z = drehung;

		cmd_vel_pub.publish(cmd_vel);
	}
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "first_steps_cmd_vel");
	ros::NodeHandle n;

	cmd_vel_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1);

	ros::Subscriber scan_sub = n.subscribe("scan", 1, scan_sub_callback);

	ros::spin();
}

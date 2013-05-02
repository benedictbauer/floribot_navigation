/*
 * find_goal.cpp
 *
 *  Created on: Apr 29, 2013
 *      Author: benedict
 */




#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>
#include "boost/thread/mutex.hpp"
#include "boost/thread/thread.hpp"
#include "ros/console.h"

class FindGoal
{
public:
	FindGoal();

private:
	void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan);
	void publish();

	double linear;
	double angular;

	float y;
	float y_l;
	float y_r;


	ros::NodeHandle n;

	ros::Publisher vel_pub;
	ros::Subscriber scan_sub;

	ros::Timer timer;

	geometry_msgs::Twist last_published;
};

FindGoal::FindGoal():
												linear(1),
												angular(0),
												y_l(0),
												y_r(0)
{
	vel_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1);
	scan_sub = n.subscribe<sensor_msgs::LaserScan>("scan", 10, &FindGoal::scanCallback, this);

	if (y_l == 0 && y_r == 0)
	{

		linear = 1;
		angular = 0;

		geometry_msgs::Twist vel;


		vel.angular.z = angular;
		vel.linear.x = linear;
		last_published = vel;

		timer = n.createTimer(ros::Duration(1), boost::bind(&FindGoal::publish, this));


		linear = 0.3;
		angular = 0.8;


		vel.angular.z = angular;
		vel.linear.x = linear;
		last_published = vel;

		timer = n.createTimer(ros::Duration(2), boost::bind(&FindGoal::publish, this));
	}

	timer = n.createTimer(ros::Duration(0.1), boost::bind(&FindGoal::publish, this));
}

void FindGoal::scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
	int num_ranges = scan->ranges.size();
	float angle_increment = scan->angle_increment;

	float search_range = 1.0;
	float ranges[num_ranges];

	for (int i = 0; i < num_ranges; i++)
	{
		ranges[i] = scan->ranges[i];
	}

	int count_l = 0, count_r = 0;
	y_r = 0;
	y_l = 0;

	for (int i = 0; i < num_ranges; i++)
	{
		if (ranges[i] < search_range)
		{
			if (i < num_ranges/2)
			{
				y_r += (sin((i-(num_ranges/2)) * angle_increment) * ranges[i]);
				count_r++;
			}
			else
			{
				y_l += (sin((i-(num_ranges/2)) * angle_increment) * ranges[i]);
				count_l++;
			}
		}
	}

	if (count_r != 0)
	{
		y_r /= count_r;
	}
	if (count_l != 0)
	{
		y_l /= count_l;
	}

	y = y_r + y_l;

	printf("%f \n",y);

	if (y < -0.05)
	{
		angular = -1;
	}
	else if ((y >= -0.1 && y <= 0.1))
	{
		angular = 0.0;
	}
	else
	{
		angular = 1;
	}


	linear = 1;

	if (y_r == 0 && y_l == 0)
	{

		linear = 0.5;
		angular = 10;

	}


	geometry_msgs::Twist vel;

	vel.angular.z = angular;
	vel.linear.x = linear;
	last_published = vel;
}

void FindGoal::publish()
{
	vel_pub.publish(last_published);
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "find_goal");
	FindGoal find_goal;

	ros::spin();
}

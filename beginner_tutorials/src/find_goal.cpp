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

		ros::NodeHandle n;

		ros::Publisher vel_pub;
		ros::Subscriber scan_sub;

		ros::Timer timer;

		geometry_msgs::Twist last_published;
};

FindGoal::FindGoal():
		linear(0.2),
		angular(0)
{
	vel_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1);
	scan_sub = n.subscribe<sensor_msgs::LaserScan>("scan", 10, &FindGoal::scanCallback, this);

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

	float y = 0;

	for (int i = 0; i < num_ranges; i++)
	{
		if (ranges[i] < search_range)
		{
			y = y + (sin((i-(num_ranges/2)) * angle_increment) * ranges[i]);
		}
	}

	if (y < -2.5)
	{
		angular = -0.25;
	}
	else if (y >= -2.5 && y <= 2.5)
	{
		angular = 0.0;
	}
	else
	{
		angular = 0.25;
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

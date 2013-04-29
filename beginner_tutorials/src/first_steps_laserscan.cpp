/*
 * first_steps_laserscann.cpp
 *
 *  Created on: Apr 22, 2013
 *      Author: benedict
 */




#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Twist.h>


void scanCallback (const sensor_msgs::LaserScan::ConstPtr& scan)

{
  size_t num_ranges = scan->ranges.size();
  double polar_histo[num_ranges];

  for (int x = 0; x < num_ranges; x++)
  {
	  polar_histo[x] = 1 / scan->ranges[x];
  }

  double min;
  int i;

  min = polar_histo[0];
  for (int i = 0; i < num_ranges; i++)
  {
      if (polar_histo[i] < min)
          min = polar_histo[i];
  }
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "first_steps_laserscan");
  ros::NodeHandle n;
  ros::Subscriber scan_sub = n.subscribe("scan", 100, scanCallback);
  ros::spin();
}

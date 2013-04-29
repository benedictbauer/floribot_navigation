/*
 * File: Floribot_find_goal.cpp
 * node floribot_find_goal
 *
 * @author: floribot_acceleo
 * @generated
 */

#include "Floribot_find_goal.h"
// Start of user code specific includes
// TODO: include your own headers


// End of user code don't delete this line

namespace floribot_navigation {

Floribot_find_goal::Floribot_find_goal(ros::NodeHandle n) : n_(n)
{
	scan_sub = n_.subscribe("scan", 1,
			&Floribot_find_goal::scan_message, this);
    tick_rate = 100;
    n_.getParam("/floribot_find_goal/tick_rate", tick_rate);
} // end of constructor

Floribot_find_goal::~Floribot_find_goal()
{
    // Start of user code destructor
    // TODO: fill with your code
    // End of user code don't delete this line
} // end of destructor

/**
 * process messages from topic scan
 *
 * @generated
 */
void Floribot_find_goal::scan_message (const sensor_msgs::LaserScan::ConstPtr& msg)
{
	// Start of user code process message
	// TODO: fill with your code

	// First steps with the Laserscan
	// Author: Benedict Bauer

	int s = msg->ranges.size();

	float PI = 3.14159265359;

	float a_max_deg = msg->angle_max * 180 / PI;
	float a_min_deg = msg->angle_min * 180 / PI;
	float a_inc_deg = msg->angle_increment * 180 / PI;

	float r[s];

	for (int i=0; i < s; i++)
	{
		r[i] = msg->ranges[i];
		//printf("Laenge Strahl %i: %f:\n",i, r[i]);
	}

	float r_invers[s];

	for (int i=0; i<s; i++)
	{
		r_invers[i] = 1/r[i];
	}

	float search_range = 2.0;


/*  //Erster Versuch eine Zielrichtung Vorzugeben:
	float min_1 = r[0];
	float min_2 = r[0];

	int pos_1, pos_2;

	for (int i=0; i < s; i++)
	{
		if (r[i] <= search_range)
		{
			if (i < s/2 && r[i] < min_1)
			{
				min_1 = r[i];
				pos_1 = i;
			}
			else if (i >= s/2 && r[i] < min_2)
			{
				min_2 = r[i];
				pos_2 = i;
			}
		}
	}
*/


	float min = r_invers[0];

	int aim;

	for (int i=0; i < s; i++)
	{
		if (r_invers[i] <= search_range)
		{
			if (r_invers[i] < min)
			{
				min = r_invers[i];
				aim = i;
			}
		}
	}

	min = r[aim];




/*
	//printf("######################################################################\n");
	//printf("max Winkel: %f\n", a_max_deg);
	//printf("min Winkel: %f\n", a_min_deg);
	//printf("Winkel Increment: %f\n", a_inc_deg);
	printf("######################################################################\n");
	printf("Min : %f\n", min);
	printf("aim : %i\n", aim);
	printf("######################################################################\n");
*/



	// End of user code don't delete this line
}

/**
 * tick is triggered 
 *
 * @generated
 */
void Floribot_find_goal::tick ()
{
	// Start of user code call your own code
	// TODO: fill with your code
	// End of user code don't delete this line
}

/**
 * returns the tick rate
 *
 * @generated
 */
int Floribot_find_goal::get_tick_rate ()
{
	return tick_rate;
}

// Start of user code additional members

// TODO: define your methods

// End of user code don't delete this line

} // end of namespace

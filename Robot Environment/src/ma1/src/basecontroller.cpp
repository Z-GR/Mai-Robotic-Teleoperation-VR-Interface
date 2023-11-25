#include "ros/ros.h"
#include "std_msgs/String.h"
#include <pigpiod_if2.h>
#include <thread>
#include <chrono>
#include "hbridge.hpp"
#include "geometry_msgs/Twist.h"
#include <math.h>

int pi_;

HBridge right_;
HBridge left_;
HBridge right2_;
HBridge left2_;

using namespace std::chrono_literals;

void setPin(int a, int b, int c, int d, int e, int f, int g, int h)
{
  right_.setPin(pi_, a, b);
  right2_.setPin(pi_, c, d);
  left_.setPin(pi_, e, f);
  left2_.setPin(pi_, g, h);
}

float rpm2pwm(float wheel)
{
  float RPM = wheel * 9.549297;
  if (RPM > 200)
    {
      RPM = 200;
    }
  else if (RPM < -200)
    {
      RPM = -200;
    }
  return RPM;
}

void chatterCallback(const geometry_msgs::Twist::ConstPtr& msg)
{
  //pin 19&26 have been burnt out cause: shorted motor driver
  // original mapping incorrect setPin(5, 6, 17, 18, 9, 25, 13, 12);
  setPin(9,25,13,12,5,6,17,18);

  //wheel n = (1/WHEEL_RADIUS) * (linear.x – linear.y – (WHEEL_SEPARATION_WIDTH + WHEEL_SEPARATION_LENGTH)*angular.z);
  float wheel_front_left = (1/0.50) * (msg->linear.x - msg->linear.y - (0.400 + 0.400)*msg->angular.z);
  float wheel_front_right = (1/0.50) * (msg->linear.x + msg->linear.y + (0.400 + 0.400)*msg->angular.z);
  float wheel_rear_left = (1/0.50) * (msg->linear.x + msg->linear.y - (0.400 + 0.400)*msg->angular.z);
  float wheel_rear_right = (1/0.50) * (msg->linear.x - msg->linear.y + (0.400 + 0.400)*msg->angular.z);

  //no need to invert right side (inverted in hardware)

  float wfr = rpm2pwm(wheel_front_right);
  float wfl = rpm2pwm(wheel_front_left);
  float wrr = rpm2pwm(wheel_rear_right);
  float wrl = rpm2pwm(wheel_rear_left);
  
  right_.drive(wfr);
  left_.drive(wfl);
  right2_.drive(wrr);
  left2_.drive(wrl);

  ROS_INFO("I heard x: [%f]", msg->linear.x);
  ROS_INFO("I heard y: [%f]", msg->linear.y);
  ROS_INFO("I heard y: [%f]", msg->angular.z);
  ROS_INFO("I heard wheel fl: [%f]", wfl);
  ROS_INFO("I heard wheel fr: [%f]", wfr);
  ROS_INFO("I heard wheel rl: [%f]", wrl);
  ROS_INFO("I heard wheel rr: [%f]", wrr);
  
}

void chatterCallback2(const geometry_msgs::Twist::ConstPtr& msg)
{
  if (msg->linear.x == 0)
  {
    right_.drive(0);
    left_.drive(0);
    right2_.drive(0);
    left2_.drive(0);
  }
}
int main(int argc, char **argv)
{
  pi_ = pigpio_start(NULL, NULL);

  /**
   * Must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "listener");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle baseController;

  ros::AsyncSpinner spinner(0); // multi-threaded 
  spinner.start();
  //ros::Subscriber sub = baseController.subscribe("cmd_vel", 1000, chatterCallback);
  ros::Subscriber sub2 = baseController.subscribe("cmd_vel", 1000, chatterCallback2);
  ros::SubscribeOptions ops;
  ops.template init<geometry_msgs::Twist>("cmd_vel", 1000, chatterCallback);
  ops.allow_concurrent_callbacks = true;
  ros::Subscriber sub = baseController.subscribe(ops);
  
  //ros::spin();	// single threaded
  ros::waitForShutdown();

  //stop all motors on exit
  right_.drive(0);
  left_.drive(0);
  right2_.drive(0);
  left2_.drive(0);

  pigpio_stop(pi_);

  return 0;
}
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <pigpiod_if2.h>
#include <thread>
#include <chrono>
#include "hbridge.hpp"
#include "geometry_msgs/Twist.h"
#include <math.h>

int pi_;
int pin_;
int MotorSpeed1 = 200;
int MotorSpeed2;
float Vel;
int CurrentSpeed;

HBridge right_;
HBridge left_;
HBridge right2_;
HBridge left2_;

using namespace std::chrono_literals;

void rampUp()
{
  int i = CurrentSpeed;
  int x = 5;
    while (Vel > 0 && i < MotorSpeed1)
    {
      i = i + 1;
      CurrentSpeed = i;
      right_.drive(i);
      left_.drive(i+x);
      right2_.drive(i);
      left2_.drive(i);
      x = 0;
      if (i == MotorSpeed1)
      {
        x = -5;
      }
      std::this_thread::sleep_for(1ms);
    }
}

void rampUpN()
{
  int i = CurrentSpeed;
  int x = 10;
    while (Vel < 0 && i > MotorSpeed1)
    {
      i = i - 1;
      CurrentSpeed = i;
      right_.drive(i);
      left_.drive(i+x);
      right2_.drive(i);
      left2_.drive(i);
      x = 0;
      if (i == MotorSpeed1)
      {
        x = -10;
      }
      std::this_thread::sleep_for(1ms);
    }
}
//ramp up angular counter clockwise
void rampUpACC()
{
  int i = CurrentSpeed;
  int x = 5;
    while (Vel == 0 && i < MotorSpeed1)
    {
      i = i + 1;
      CurrentSpeed = i;
      right_.drive(i);
      left_.drive(-i-x);
      right2_.drive(i);
      left2_.drive(-i);
      x = 0;
      if (i == MotorSpeed1)
      {
        x = -5;
      }
      std::this_thread::sleep_for(1ms);
    }
}

void rampUpACW()
{
  int i = CurrentSpeed;
  int x = 5;
    while (Vel == 0 && i < MotorSpeed1)
    {
      i = i + 1;
      CurrentSpeed = i;
      right_.drive(-i);
      left_.drive(i-x);
      right2_.drive(-i);
      left2_.drive(i);
      x = 0;
      if (i == MotorSpeed1)
      {
        x = -5;
      }
      std::this_thread::sleep_for(1ms);
    }
}

void rampDown()
{
  int i = CurrentSpeed;
    while (Vel == 0 && i != 0)
    {
      if (CurrentSpeed > 0)
      {
        i = i - 1;
      }
      else if (CurrentSpeed < 0)
      {
        i = i + 1;
      }
      CurrentSpeed = i;
      right_.drive(i);
      left_.drive(i);
      right2_.drive(i);
      left2_.drive(i);
      std::this_thread::sleep_for(1ms);
    }
}


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
  Vel = msg->linear.x;
  MotorSpeed1 = msg->linear.x * 200;
  MotorSpeed2 = msg->linear.y * 200;
  setPin(19, 26, 17, 18, 9, 25, 22, 23);
  //wheel_front_left = (1/WHEEL_RADIUS) * (linear.x – linear.y – (WHEEL_SEPARATION_WIDTH + WHEEL_SEPARATION_LENGTH)*angular.z);
  float wheel_front_left = (1/0.50) * (msg->linear.x - msg->linear.y - (0.400 + 0.400)*msg->angular.z);
  float wheel_front_right = (1/0.50) * (msg->linear.x + msg->linear.y + (0.400 + 0.400)*msg->angular.z);
  float wheel_rear_left = (1/0.50) * (msg->linear.x + msg->linear.y - (0.400 + 0.400)*msg->angular.z);
  float wheel_rear_right = (1/0.50) * (msg->linear.x - msg->linear.y + (0.400 + 0.400)*msg->angular.z);

  //wheel_front_right = -1 * wheel_front_right;
  //wheel_rear_right = -1 * wheel_rear_right;

  //rad/s to RPM = rad/s * 9.549297
  float wfr = rpm2pwm(wheel_front_right);
  float wfl = rpm2pwm(wheel_front_left);
  float wrr = rpm2pwm(wheel_rear_right);
  float wrl = rpm2pwm(wheel_rear_left);
  

  
  right_.drive(wfr);
  left_.drive(wfl);
  right2_.drive(wrl);
  left2_.drive(wrr);


  ROS_INFO("I heard x: [%f]", msg->linear.x);
  ROS_INFO("I heard y: [%f]", msg->linear.y);
  ROS_INFO("I heard y: [%f]", msg->angular.z);
  ROS_INFO("I heard wheel 1: [%f]", wfl);
  ROS_INFO("I heard wheel 1: [%f]", wfr);
  ROS_INFO("I heard wheel 1: [%f]", wrl);
  ROS_INFO("I heard wheel 1: [%f]", wrr);
  
}

void chatterCallback2(const geometry_msgs::Twist::ConstPtr& msg)
{
  setPin(19, 26, 17, 18, 9, 25, 22, 23);
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

  /**
   * The subscribe() call is how you tell ROS that you want to receive messages
   * on a given topic.  This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing.  Messages are passed to a callback function, here
   * called chatterCallback.  subscribe() returns a Subscriber object that you
   * must hold on to until you want to unsubscribe.  When all copies of the Subscriber
   * object go out of scope, this callback will automatically be unsubscribed from
   * this topic.
   *
   * The second parameter to the subscribe() function is the size of the message
   * queue.  If messages are arriving faster than they are being processed, this
   * is the number of messages that will be buffered up before beginning to throw
   * away the oldest ones.
   * 
   */
  ros::AsyncSpinner spinner(0);
  spinner.start();
  //ros::Subscriber sub = baseController.subscribe("cmd_vel", 1000, chatterCallback);
  ros::Subscriber sub2 = baseController.subscribe("cmd_vel", 1000, chatterCallback2);
  ros::SubscribeOptions ops;
  ops.template init<geometry_msgs::Twist>("cmd_vel", 1000, chatterCallback);
  ops.allow_concurrent_callbacks = true;
  ros::Subscriber sub = baseController.subscribe(ops);
  

  /**
   * ros::spin() will enter a loop, pumping callbacks.  With this version, all
   * callbacks will be called from within this thread (the main one).  ros::spin()
   * will exit when Ctrl-C is pressed, or the node is shutdown by the master.
   */
  //ros::spin();	
  ros::waitForShutdown();

  pigpio_stop(pi_);

  //stop all motors on exit
  right_.drive(0);
  left_.drive(0);
  right2_.drive(0);



  return 0;
}
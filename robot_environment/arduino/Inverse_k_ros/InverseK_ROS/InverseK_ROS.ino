// Arduino, IK for Arduino Braccio & ROS

// Include the library InverseK.h
#include <InverseK.h>

#include <ros.h>
#include <Arduino.h>
#include <BraccioLibMai1.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/UInt32.h>
#include <geometry_msgs/Point32.h>
#include <Servo.h>

ros::NodeHandle nh;

Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_ver;
Servo wrist_rot;
Servo gripper;

unsigned int _baseAngle = 90;
unsigned int _shoulderAngle = 45;
unsigned int _elbowAngle = 180;
unsigned int _wrist_verAngle = 90;
unsigned int _wrist_rotAngle = 0;
unsigned int _gripperAngle = 40; //closed

float x;
float y;
float z;

void BraccioMove( const geometry_msgs::Point32& msg){
  x = msg.x;
  y = msg.y;
  z = msg.z;
}

void GripperMove( const std_msgs::UInt32& msg){
  _gripperAngle = msg.data;
}

ros::Subscriber<geometry_msgs::Point32> sub("Ik_p", &BraccioMove);
ros::Subscriber<std_msgs::UInt32> sub2("GripperAngle", &GripperMove);

void setup() {
  Braccio.begin();
  //Serial.begin(9600);
  nh.initNode();
  nh.subscribe(sub);
  nh.subscribe(sub2);
  
  
  // Setup the lengths and rotation limits for each link
  Link base, upperarm, forearm, hand;

  base.init(0, b2a(0.0), b2a(180.0));
  upperarm.init(200, b2a(15.0), b2a(165.0));
  forearm.init(200, b2a(0.0), b2a(180.0));
  hand.init(270, b2a(0.0), b2a(180.0));

  // Attach the links to the inverse kinematic model
  InverseK.attach(base, upperarm, forearm, hand);

  float a0, a1, a2, a3;

  // InverseK.solve() return true if it could find a solution and false if not.

  // Calculates the angles without considering a specific approach angle
  // InverseK.solve(x, y, z, a0, a1, a2, a3)
  if(InverseK.solve(-600, 0, 0, a0, a1, a2, a3)) {
    _baseAngle = a2b(a0);
    _shoulderAngle = a2b(a1);
    _elbowAngle = a2b(a2);
    _wrist_verAngle = a2b(a3);
  }
}
void loop() 
{
  Braccio.ServoMovement(10,_baseAngle,_shoulderAngle,_elbowAngle,_wrist_verAngle,_wrist_rotAngle,_gripperAngle);

  float a0, a1, a2, a3;

  if(InverseK.solve(x, y, z, a0, a1, a2, a3)) {
  _baseAngle = a2b(a0);
  _shoulderAngle = a2b(a1);
  _elbowAngle = a2b(a2);
  _wrist_verAngle = a2b(a3);
 
  }
  
  nh.spinOnce();
}

// Quick conversion from the Braccio angle system to radians
float b2a(float b){
  return b / 180.0 * PI - HALF_PI;
}

// Quick conversion from radians to the Braccio angle system
float a2b(float a) {
  return (a + HALF_PI) * 180 / PI;
}

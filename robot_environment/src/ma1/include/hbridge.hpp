/**
 * @file HBridge.hpp
 * @author Zak G Rackham (zakgrackham@gmail.com)
 * @brief H-bridge class controlling a DC-motor for ROS2
 * @version 0.2
 * @date 2021-04-07
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef HBRIDGE_H
#define HBRIDGE_H

#include <pigpiod_if2.h>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

/**
 * @brief Hbridge class controlling a DC-motor for ROS
 */
class HBridge
{
private:
  int pi_; // pigpio id

  // INPUT pin numbers for motor driver
  int pin_in1_;
  int pin_in2_;
  int pin_pwm_;

public:
  /**
   * @brief default constructor (dummy)
   */
  HBridge()
  {
  }

  ~HBridge()
  {
    set_mode(pi_, pin_in1_, PI_INPUT);
    set_mode(pi_, pin_in2_, PI_INPUT);
    if (pin_pwm_ >= 0)
    {
      set_mode(pi_, pin_pwm_, PI_INPUT);
    }
  }


  /**
   * @brief set gpio pin numbers for L298N driver
   * @param pi       pigpio id
   * @param pin_in1 pin number connected to IN1 of L298N
   * @param pin_in2 pin number connected to IN2 of L298N
   */
  void setPin(int pi,
              int pin_in1,
              int pin_in2)
  {
    pi_ = pi;
    pin_in1_ = pin_in1;
    pin_in2_ = pin_in2;


    set_mode(pi_, pin_in1_, PI_OUTPUT);
    set_mode(pi_, pin_in2_, PI_OUTPUT);
  }

  /**
   * @brief drive a motor
   * add const for ros2 callback
   * @param power power (duty ratio) of a motor (-255 -- 255)
   */
  void drive(int power) const
  {
    // L298N
    if (power > 0)
    {
      set_PWM_dutycycle(pi_, pin_in1_, power);
      gpio_write(pi_, pin_in2_, 0);
    }
    else
    {
      power = -power;
      gpio_write(pi_, pin_in1_, 0);
      set_PWM_dutycycle(pi_, pin_in2_, power);
    }
  }
};

#endif
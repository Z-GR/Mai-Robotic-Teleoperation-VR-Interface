#!/usr/bin/env python3
import RPi.GPIO as GPIO
from RpiMotorLib import RpiMotorLib
import time
import rospy
from std_msgs.msg import String

################################
# RPi and Motor Pre-allocations
################################
#
#define GPIO pins
direction= 11 #23 # Direction (DIR) GPIO Pin
step = 8 #24 #8 # Step GPIO Pin
EN_pin = 7 #26 #7 # enable pin (LOW to enable)

# Declare a instance of class pass GPIO pins numbers and the motor type
mymotortest = RpiMotorLib.A4988Nema(direction, step, (-1,-1,-1), "DRV8825")
GPIO.setup(EN_pin,GPIO.OUT) # set enable pin as output
GPIO.setup(step, GPIO.OUT)
GPIO.setup(direction, GPIO.OUT)

GPIO.output(EN_pin,GPIO.LOW) # pull enable to low to enable motor
GPIO.output(direction,GPIO.LOW)# low = up// high = down
pwm = GPIO.PWM(8, 3000)

x = 0
while x < 1:
    x = x + 1
    GPIO.output(direction,GPIO.LOW)
    pwm.start(10)
    time.sleep(2)
    GPIO.output(direction,GPIO.HIGH)
    pwm.start(90)
    time.sleep(2)

GPIO.cleanup()

def callback(data):
    rospy.loginfo(rospy.get_caller_id() + 'I heard %s', data.data)

def listener():

    

    GPIO.cleanup() # clear GPIO allocations after run

    # In ROS, nodes are uniquely named. If two nodes with the same
    # name are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('listener', anonymous=True)

    rospy.Subscriber('chatter', String, callback)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    listener()
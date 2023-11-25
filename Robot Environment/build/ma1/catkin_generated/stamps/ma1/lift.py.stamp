#!/usr/bin/env python3
from threading import local
import RPi.GPIO as GPIO
import time
import rospy
from std_msgs.msg import Int8

################################
# RPi and Motor Pre-allocations
################################
#
#define GPIO pins
direction= 11 # Direction (DIR) GPIO Pin
step = 8 # Step GPIO Pin
EN_pin = 7 # enable pin (LOW to enable)
globaldata0 = 1
position = 0.0

GPIO.setmode(GPIO.BCM)
GPIO.setup(EN_pin,GPIO.OUT) # set pins as output
GPIO.setup(step, GPIO.OUT)
GPIO.setup(direction, GPIO.OUT)

GPIO.output(EN_pin,GPIO.LOW) # pull enable to low to enable motor
GPIO.output(direction,GPIO.LOW) # low = up// high = down

def limiter(direction):
    global position
    global globaldata0
    upperBound = 10.0 #cm
    lowerBound = -10.0 #cm
    if direction == 1:
        position = position + 0.7 #cm - approximate of 7mm translation per 360 degrees rotation
    elif direction == 2:
        position = position - 0.7 #cm - approximate of 7mm translation per 360 degrees rotation
    if position < lowerBound:
        print(position)
        print("false")
        globaldata0 = 0
        return True
    elif position > upperBound:
        print(position)
        print("false")
        globaldata0 = 0
        return True
    else:
        print(position)
        return False

#full step 360 degrees clockwise rotation (lift up)
limiter(1)
for x in range(200):
    GPIO.output(step,GPIO.HIGH)
    time.sleep(0.0005) # delay 500 microseconds
    GPIO.output(step,GPIO.LOW)
    time.sleep(0.0005) # delay 500 microseconds

GPIO.cleanup()

# each callback fuction operates in its own thread
def callback2(data):
    data0 = data.data
    global globaldata0
    globaldata0 = data0

def callback(data):
    data0 = data.data
    global globaldata0
    rospy.loginfo(rospy.get_caller_id() + 'I heard %s', data.data)

    GPIO.setmode(GPIO.BCM)
    GPIO.setup(EN_pin,GPIO.OUT) # set enable pin as output
    GPIO.setup(step, GPIO.OUT)
    GPIO.setup(direction, GPIO.OUT)
    GPIO.output(EN_pin,GPIO.LOW)

    if data0 == 1:
        GPIO.output(direction,GPIO.LOW)
        limiter(1)
        for x in range(200):
            GPIO.output(step,GPIO.HIGH)
            time.sleep(0.0005) # delay 500 microseconds
            GPIO.output(step,GPIO.LOW)
            time.sleep(0.0005) # delay 500 microseconds
            if globaldata0 == 0:
                break
    if data0 == 2:
        GPIO.output(direction,GPIO.HIGH)
        limiter(2)
        for x in range(200):
            GPIO.output(step,GPIO.HIGH)
            time.sleep(0.0005) # delay 500 microseconds
            GPIO.output(step,GPIO.LOW)
            time.sleep(0.0005) # delay 500 microseconds
            if globaldata0 == 0:
                break
    
    globaldata0 = 1
        

def listener():

    # In ROS, nodes are uniquely named. If two nodes with the same
    # name are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('lift_listener', anonymous=True)

    rospy.Subscriber('lift', Int8, callback)

    rospy.Subscriber('lift', Int8, callback2)

    rospy.spin() # spin() simply keeps python from exiting until this node is stopped
    GPIO.cleanup() # clear GPIO allocations after run

if __name__ == '__main__':
    listener()
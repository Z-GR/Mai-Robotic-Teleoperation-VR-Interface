# Robotics project -- Mobile Arm v1.1 (Mai)
Notes: 
Please ignore .DS_Store files (Associated with Mac OS, these files store custom attributes of its containing folder)
## Setup

1. First flash ubuntu server 20.4.02 onto the rpi micro SD card using the rpi imager
2. Setup network-config file on SD card (after flashing)(headless)
3. Or alternatively setup with

- nano /etc/netplan/50-cloud-init.yaml

- sudo netplan generate
- sudo netplan apply
- reboot

4. setup user account ma1 & grant the user sudo privaliges

- sudo adduser newuser (replace "newuser" with mai (or any name you prefer))
- sudo usermod -aG sudo newuser
  
5. Edit hostname and create relevent user accounts, grant permissions where nessaccary
6. Install ROS neotic http://wiki.ros.org/noetic/Installation/Ubuntu
7. On install install ros base (ros base is used here as we wont be maing use of GUI elements)

- sudo apt install ros-noetic-ros-base

8. Add setup to bash.rc (this sources (executes) the ros middleware so we can run ros command line arguments) this sets up the shell for use in your particular environment (ROS enivironment).

- echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc
- source ~/.bashrc (sourced on login of each new shell)

9. Install dependencys -- (library's)

- sudo apt install python3-pip (pip is a python package manager)

- sudo pip3 install rpimotorlib

10. Install ros serial for communication with arduino 

- sudo apt-get install ros-<rosdistro>-rosserial

11. Grant permission for serial port

- sudo chmod a+rw /dev/ttyACM0

## Kinect

1. Install libfreenect
2. Install kinect_aux (For tilt control)
3. Kinect_aux (BUG Report: wrong data type used in function setTiltAngle uint16_t change to int16_t)

- rostopic pub /tilt_angle std_msgs/Float64 -- -15 {-30:30} // move to 15 degrees down
- rostopic pub /tilt_angle std_msgs/Float64 -- -30 {-30:30} //full tilt (Expected result) // no movement (returned result) Fix: uint16_t changed to int16_t

## Webserver using websocket API (Unity <-> ROS)

1. Install rosbridge via debian

- sudo apt-get install ros-<rosdistro>-rosbridge-server

## Launch
  
Current boot sequence (source ros workspace on each shell):
  
- sudo pigpiod

- roslaunch freenect_launch freenect.launch depth_registration:=true

- roslaunch rosbridge_server rosbridge_websocket.launch

- roslaunch ma1 ma1.launch

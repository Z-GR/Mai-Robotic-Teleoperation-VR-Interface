# ma1
## Robotics project --Mobile Arm v0.1 (Mai)

===========setup============

1. First flash ubuntu server 20.4.02 onto the rpi micro SD card using the rpi imager
2. Setup network-config file on SD card (after flashing)
3. setup user account ma1 & grant the user sudo privaliges

- sudo adduser newuser (replace "newuser" with mai (or any name you prefer))
- sudo usermod -aG sudo newuser

5. Or alternatively setup with

- nano /etc/netplan/50-cloud-init.yaml

- sudo netplan generate
- sudo netplan apply
- reboot
  
4. Edit hostname and create relevent user accounts, grant permissions where nessaccary
5. Install ROS neotic http://wiki.ros.org/noetic/Installation/Ubuntu
6. On install install ros base

- sudo apt install ros-noetic-ros-base

8. Add setup to bash.rc (this sources the ros middleware so we can run ros command line arguments)

- echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc
- source ~/.bashrc (sourced on login)

9. Install dependencys -- (library's)

- sudo apt install python3-pip (pip is a python package manager)

- sudo pip3 install rpimotorlib

10. Install ros serial for communication with arduino 

- sudo apt-get install ros-"ros-distribution"-rosserial

11. Grant permission for serial port

- sudo chmod a+rw /dev/ttyACM0

-----------kinect--------

1. Install libfreenect
2. Install kinect_aux (For tilt control)
3. Kinect_aux (BUG Report: wrong datatype used uint_32 changed to int_32)
4. Below is

- rostopic pub /tilt_angle std_msgs/Float64 -- -15 {-30:30}

-------------VR----------

1. Install rosbridge via debian

- sudo apt-get install ros-<rosdistro>-rosbridge-server


# ma1
## Robotics project --Mobile Arm v0.1

===========setup============

1. First flash ubuntu server 20.4.02 onto the rpi micro SD card using the rpi imager
2. setup user accounts -- grant sudo privaliges
3. Setup network-config file on SD card
4. Or alternatively setup with nano then

- sudo netplan generate
- sudo netplan apply
- reboot
  
4. Edit hostname and create relevent user accounts, grant permissions where nessaccary
5. Install ROS neotic http://wiki.ros.org/noetic/Installation/Ubuntu
6. On install install ros base

sudo apt install ros-noetic-ros-base

8. Add setup to bash.rc

echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc
source ~/.bashrc

9. Install dependencys -- library's

sudo apt install python3-pip

sudo pip3 install rpimotorlib

10. install ros serial for communication with arduino 

sudo apt-get install ros-indigo-rosserial



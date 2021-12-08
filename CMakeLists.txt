cmake_minimum_required(VERSION 3.0.2)
project(ma1)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp tf rospy std_msgs genmsg geometry_msgs)

## Declare a catkin package
catkin_package()

## Build basecontroller and listener
include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(basecontroller src/basecontroller.cpp)
target_link_libraries(basecontroller ${catkin_LIBRARIES})
target_link_libraries(basecontroller pigpiod_if2)

add_executable(armcontroller src/armcontroller.cpp)
target_link_libraries(armcontroller ${catkin_LIBRARIES})

add_executable(tf_broadcaster src/tf_broadcaster.cpp)
target_link_libraries(tf_broadcaster ${catkin_LIBRARIES})

catkin_python_setup()

catkin_install_python(PROGRAMS
   src/lift.py
   src/keyboard_teleop.py
   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
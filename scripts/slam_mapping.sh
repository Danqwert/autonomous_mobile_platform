#!/bin/bash
roslaunch diff_drive car.launch
roslaunch ydlidar_ros X2L1.launch
roslaunch hector_mapping mapping_default.launch
roslaunch hector_slam_launch tutorial.launch
rviz /home/pi/ws/src/diff_drive/launch/demo.rviz
rosrun teleop_twist_keyboard teleop_twist_keyboard.py
#rosrun map_server map_server -f my_map
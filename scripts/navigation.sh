#!/bin/bash
roslaunch diff_drive project.launch 
roslaunch diff_drive projectnavigate.launch 
roslaunch ydlidar_ros X2L.launch
rviz /home/pi/ws/src/diff_drive/launch/projectnavigate.rviz
rosrun teleop_twist_keyboard teleop_twist_keyboard.py 
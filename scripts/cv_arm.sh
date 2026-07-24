#!/bin/bash
cd /home/pi/Desktop/project
python demo_cv_async.py &
sleep 5s
python3 object_detection_sample_ssd.py
roslaunch diff_drive arm.launch

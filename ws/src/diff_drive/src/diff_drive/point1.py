#!/usr/bin/env python3
import rospy

from geometry_msgs.msg import Pose
import numpy as np
from math import sqrt


def publisher():
    pub = rospy.Publisher('poses', Pose, queue_size=1)
    rospy.init_node('pose_publisher', anonymous=False)
    rate = rospy.Rate(1) # Hz
    while not rospy.is_shutdown():
        p = Pose()
        p.position.x = 10
        p.position.y = 3
        p.position.z = 10
        #p.poses=p1
        #p.frame_id="d"
        pub.publish(p)
        rate.sleep()

if __name__ == '__main__':
    try:
        publisher()
    except rospy:
        pass

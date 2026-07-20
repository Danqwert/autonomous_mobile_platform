#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Pose
import numpy as np
from math import sqrt


def publisher():
    pub = rospy.Publisher('pose', Pose, queue_size=1) 
    rospy.init_node('pose_publisher', anonymous=True) 
    rate = rospy.Rate(1) # Hz 
    while not rospy.is_shutdown():
        p = Pose()
        f_x=open('x.txt')
        f_y=open('y.txt')
        x=float(f_x.read())
        y=float(f_y.read())
        f_x.close()
        f_y.close()
        depth=np.load('/home/pi/Desktop/project/depth.npy')
        x=int(x)
        y=int(y)
        wz=depth[y,x]
        factor=(2*0.1042*wz)/120
        wx=(x-320)*factor
        wy=(y-240)*factor
        y=-wx
        z=-wy
        x=sqrt(wx*wz-(z*z+y*y))
        p.position.x = x
        p.position.y = y
        p.position.z = z
        pub.publish(p)
        rate.sleep()

if __name__ == '__main__':
    try:
        publisher()
    except rospy:
        pass

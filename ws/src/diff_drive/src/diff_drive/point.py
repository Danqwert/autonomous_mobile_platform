#!/usr/bin/env python3
import rospy

from std_msgs.msg import UInt32
import numpy as np
from math import sqrt


def publisher():
    pub = rospy.Publisher('poses',UInt32, queue_size=1)
    rospy.init_node('pose_publisher', anonymous=False)
    rate = rospy.Rate(1) # Hz
    while not rospy.is_shutdown():
        f_x=open('/home/pi/Desktop/project/x.txt','r')
        f_y=open('/home/pi/Desktop/project/y.txt','r')
        x=float(f_x.read())
        y=float(f_y.read())
        f_x.close()
        f_y.close()
        depth=np.load('/home/pi/Desktop/project/depth.npy')
        x=int(x)
        y=int(y)
        wz=int(depth[y,x])
        #wz=wz/10
        print(wz)
        factor=(2*0.1042*wz)/120
        wx=(x-320)*factor
        wy=(y-240)*factor
        y=-wx
        z=-wy
        #x=sqrt(abs(wx*wz-(z*z+y*y)))
        x=wz
        x=x/10
        y=y/10
        z=z/10

        k=z
        z=35+z*0.788-x*0.615
        x=(-8+x*0.788+k*0.615)
        x=int(x)
        y=int(y)
        z=int(z)
        
        
        c=abs(x)*1000000+abs(z)*10000+abs(y)*100
        if y>0:
            c=c+1
        if z<0:
            c=c+10
        c=int(c)
        print(x)
        print(y)
        print(z)
        print(c)
        print("###################")
        

        
        
        pub.publish(c)
        rate.sleep()

if __name__ == '__main__':
    try:
        publisher()
    except rospy:
        pass

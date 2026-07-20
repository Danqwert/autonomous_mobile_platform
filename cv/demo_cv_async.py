#!/usr/bin/env python
import freenect
import numpy
import signal
from time import sleep
keep_running = True


def display_depth(dev, data, timestamp):
    global keep_running
    numpy.save('depth',data)
    sleep(0.2)



def display_rgb(dev, data, timestamp):
    global keep_running
    #cv.ShowImage('RGB', frame_convert.video_cv(data))
    numpy.save('image',data)
    sleep(0.2)



def body(*args):
    if not keep_running:
        raise freenect.Kill
def handler(signum, frame):
    """Sets up the kill handler, catches SIGINT"""
    global keep_running
    keep_running = False


print('Press Ctrl-C in terminal to stop')
signal.signal(signal.SIGINT, handler)
freenect.runloop(depth=display_depth,
                 video=display_rgb,
                 body=body)

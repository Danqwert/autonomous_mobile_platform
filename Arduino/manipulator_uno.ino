#include <Servo.h>
#include <Kinematics.h>
#include <ros.h>
#include <std_msgs/UInt32.h>


ros::NodeHandle nh;

int x, y, z = 0;
int x1,y1,z1=0;
int a1, a2, a3 = 0;
int aa1, aa2, aa3 =0;

void messageCb(const std_msgs::UInt32& msg) {
  
  y=(msg.data/100)%100;
  z=(msg.data/10000)%100;
  x=(msg.data/1000000);
  if(msg.data%10==1)
    y=-y;
  if((msg.data/10)%10==1)
    z=-z;
  //f();
  //if(x1!=x or y1!=y or z1!=z)
  //f();
}
ros::Subscriber<std_msgs::UInt32> s("poses", messageCb);


Servo myservo1; Servo myservo2; Servo myservo3;
Servo myservo4; Servo myservo5; Servo myservo6;


Kinematics k(16, 17);
Position p;
Angle a;



void setup() {
  nh.initNode();
  nh.getHardware()->setBaud(9600);
  nh.subscribe(s);

  Serial.begin(9600);
  /*
  x = 20;
  x1=20;
  y = 0;
  y1=0;
  z = -5;
  z1=-5;
  k.moveToPosition( x, y, z );
  a = k.getAngles();*/


}


void f() {  
  myservo1.attach(7);
  myservo2.attach(6);
  myservo3.attach(5);
  myservo4.attach(4);
  myservo5.attach(3);
  myservo6.attach(2);

  a1=-60;
  a2=90;
  a3=0;
  
  myservo1.write(90 - a1);
  myservo2.write(90 + a1);
  myservo3.write(a2 - 90);
  myservo4.write(90-a3);
  myservo5.write(0);
  myservo6.write(60);
  delay(500);

  k.moveToPosition( x, y, z );
  a = k.getAngles();
  
  myservo5.write(90);
  /////////////////////////////////
  
  if (90 - a.theta1/2 > 90 - a1)
    while (90 - a.theta1/2 > 90 - a1)
    { a1--;
      myservo1.write(90 - a1);
      myservo2.write(90 + a1);
      delay(15);
    }
  if (90 - a.theta1/2 < 90 - a1) {
    while (90 - a.theta1/2 < 90 - a1)
    { a1++;
      myservo1.write(90 - a1);
      myservo2.write(90 + a1);
      delay(15);
    }
  }
  //////////////////////////////////////
  if (-a.theta2 + 90 > -a2 + 90)
    while (-a.theta2 + 90 > -a2 + 90)
    { a2--;
      myservo3.write(-a2 + 90);
      delay(15);
    }
  if (-a.theta2 + 90 < -a2 + 90) {
    while (-a.theta2 + 90 < -a2 + 90)
    { a2++;
      myservo3.write(-a2 + 90);
      delay(15);
    }
  }
  /////////////////////////////////////
  if (90 - a.theta3> 90-a3)
    while (90 - a.theta3 > 90-a3)
    { a3--;
      myservo4.write(90-a3);
      delay(20);
    }
  if (90 - a.theta3 < 90-a3) {
    while (90 - a.theta3 < 90-a3)
    { a3++;
      myservo4.write(90-a3);
      delay(20);
    }
  }
  ////////////////////////////////
  myservo5.write(180 + (90 - a.theta1) - (/*180 -*/ a.theta2));
  delay(200);
  /////////////////////////////////
  if (90 - a.theta1 > 90 - a1)
    while (90 - a.theta1 > 90 - a1)
    { a1--;
      myservo1.write(90 - a1);
      myservo2.write(90 + a1);
      delay(20);
    }
  if (90 - a.theta1 < 90 - a1) {
    while (90 - a.theta1 < 90 - a1)
    { a1++;
      myservo1.write(90 - a1);
      myservo2.write(90 + a1);
      delay(20);
    }
  }
  //////////////////////////////////////
  delay(350);
  myservo5.write(180 + (90 - a.theta1) - (/*180 -*/ a.theta2)+20);
  myservo3.write(-a2 + 80);
  myservo6.write(180);
  //myservo5.write(0);
  x1=x;
  y1=y;
  z1=z;
  delay(300);
//////////////////////////////////////
//////////////////////////////////////
//////////////////////////////////////
  aa1=-60;
  aa2=90;
  aa3=0;
    /////////////////////////////////
  if (90 - aa1 > 90 - a1)
    while (90 - aa1 > 90 - a1)
    { a1--;
      myservo1.write(90 - a1);
      myservo2.write(90 + a1);
      delay(20);
    }
  if (90 - aa1 < 90 - a1) {
    while (90 - aa1 < 90 - a1)
    { a1++;
      myservo1.write(90 - a1);
      myservo2.write(90 + a1);
      delay(20);
    }
  }
  //////////////////////////////////////

  if (-aa2 + 90 > -a2 + 90)
    while (-aa2 + 90 > -a2 + 90)
    { a2--;
      myservo3.write(-a2 + 90);
      delay(20);
    }
  if (-aa2 + 90 < -a2 + 90) {
    while (-aa2 + 90 < -a2 + 90)
    { a2++;
      myservo3.write(-a2 + 90);
      delay(20);
    }
  }
  /////////////////////////////////////
  if (90 - aa3> 90-a3)
    while (90 - aa3 > 90-a3)
    { a3--;
      myservo4.write(90-a3);
      delay(15);
    }
  if (90 - aa3 < 90-a3) {
    while (90 - aa3 < 90-a3)
    { a3++;
      myservo4.write(90-a3);
      delay(15);
    }
  }
  ////////////////////////////////
  myservo1.detach();
  myservo2.detach();
  myservo3.detach();
  myservo4.detach();

delay(1000);

}




void loop() {
  nh.spinOnce();
  if(x1!=x or y1!=y or z1!=z)
  {f();}
  delay(1);



}

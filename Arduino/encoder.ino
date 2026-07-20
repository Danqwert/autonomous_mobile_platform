#define E11 10      // первый контакт
#define E12 11    // второй контак
#define E21 2
#define E22 3

int stepEnc1 = 0;  // состояние энкодера
byte buffEnc1[2];  // буффер состояния первой кнопки
byte flagEnc1 = 0; // флаг изменения
int stepEnc2 = 0;  // состояние энкодера
byte buffEnc2[2];  // буффер состояния первой кнопки
byte flagEnc2 = 0;

float cpwm1 ;
float cpwm2 ;
float rrpm1 ;
float rrpm2 ;


#include "ros.h"
#include <std_msgs/Float32.h>
#include <std_msgs/Int32.h>


int motor1pin1 = 5;
int motor1pin2 = 6;
int motor2pin1 = 7;
int motor2pin2 = 8;
const int right_pwm_pin = 4;
const int left_pwm_pin = 9;

#define COMMAND_RATE 4
#define ODOM_RATE 5

void lmessageCb( const std_msgs::Int32& msg) {
  rrpm1 = msg.data;
}
void rmessageCb( const std_msgs::Int32& msg) {
  rrpm2 = msg.data;
}

ros::NodeHandle nh;

ros::Subscriber<std_msgs::Int32> lwheel_rrpm("lwheel_desired_rate", &lmessageCb);
ros::Subscriber<std_msgs::Int32> rwheel_rrpm("rwheel_desired_rate", &rmessageCb);
std_msgs::Float32 r_crpm;
std_msgs::Float32 l_crpm;
std_msgs::Int32 r_ticks;
std_msgs::Int32 l_ticks;
ros::Publisher lwheel_ticks("lwheel_ticks", &l_ticks);
ros::Publisher rwheel_ticks("rwheel_ticks", &r_ticks);
ros::Publisher lwheel_rate("lwheel_rate", &l_crpm);
ros::Publisher rwheel_rate("rwheel_rate", &r_crpm);

void setup() {
  pinMode(E11, INPUT);
  pinMode(E12, INPUT);
  pinMode(E21, INPUT);
  pinMode(E22, INPUT);
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
  pinMode(right_pwm_pin, OUTPUT);
  pinMode(left_pwm_pin, OUTPUT);
  nh.initNode();
  nh.getHardware()->setBaud(57600);
  nh.subscribe(lwheel_rrpm);
  nh.subscribe(rwheel_rrpm);
  nh.advertise(lwheel_ticks);
  nh.advertise(rwheel_ticks);
  nh.advertise(lwheel_rate);
  nh.advertise(rwheel_rate);
  //Serial.begin(9600);
  r_ticks.data = 0;
  l_ticks.data = 0;
  rrpm1 = 0;
  rrpm2 = 0;
  cpwm1 = 0.0;
  cpwm2 = 0.0;
}

void loop() {
  static unsigned long prev_control_time = 0;
  static unsigned long odom_prev_control_time = 0;


  //this block drives the robot based on defined rate
  if ((millis() - prev_control_time) >= (1000 / COMMAND_RATE))
  {
    float crpm1;
    float crpm2;
    crpm1 = (float)stepEnc1 * COMMAND_RATE;
    crpm2 = (float)stepEnc2 * COMMAND_RATE;
    /*Serial.print(crpm1);
      Serial.print("_________");
      Serial.print(stepEnc1);
      Serial.print("##########");
      Serial.print(crpm2);
      Serial.print("_________");
      Serial.print(stepEnc2);*/
    f1(crpm1);
    f2(crpm2);
    m1(cpwm1);
    m2(cpwm2);
    prev_control_time = millis();
    stepEnc1 = 0;
    stepEnc2 = 0;

    /*Serial.println();
      Serial.print(cpwm1);
      Serial.print("_____");
      Serial.print(rrpm1);
      Serial.print("________");
      Serial.print(cpwm2);
      Serial.print("_____");
      Serial.print(rrpm2);
      Serial.println();*/


  }


  enc1(E11, E12);
  enc2(E21, E22);

  if ((millis() - odom_prev_control_time) >= (1000 / ODOM_RATE)) {
    lwheel_rate.publish( &r_crpm );
    rwheel_rate.publish( &l_crpm );
    float crpm1;
    float crpm2;
    crpm1 = ((float)stepEnc1 * ODOM_RATE);
    crpm2 = ((float)stepEnc2 * ODOM_RATE);
    l_crpm.data = crpm1;
    r_crpm.data = crpm2;
    lwheel_ticks.publish( &l_ticks );
    rwheel_ticks.publish( &r_ticks );
    odom_prev_control_time = millis();
    nh.spinOnce();
  }

}

void enc1(int C1, int C2) {
  //static unsigned long timer;
  buffEnc1[0] = buffEnc1[1];
  buffEnc1[1] = digitalRead(C1);
  if (buffEnc1[0] != buffEnc1[1]) {                       
    stepEnc1 += digitalRead(C2) == buffEnc1[1] ? 1 : -1;  
    l_ticks.data += digitalRead(C2) == buffEnc1[1] ? 1 : -1;

  }
  //timer = millis();
  //Serial.println(stepEnc);
}

void enc2(int C1, int C2) {
  //static unsigned long timer;
  //if (timer + 1 > millis()) return;                     // замеряем каждые 5 мс
  buffEnc2[0] = buffEnc2[1];
  buffEnc2[1] = digitalRead(C1);
  if (buffEnc2[0] != buffEnc2[1]) {                       // заметили изменение сигнала на первой ножке
    stepEnc2 += digitalRead(C2) == buffEnc2[1] ? 1 : -1;  // если вторая еще в плюсе, прибавляем, если уже в минусе, убавляем
    r_ticks.data += digitalRead(C2) == buffEnc2[1] ? 1 : -1;
  }

  //Serial.println(stepEnc);
}

void f1(float crpm)
{
  float E = (rrpm1 - crpm) / 40;
  if ((E > 0 and crpm<0) or (E < 0 and crpm>0))
  {
    cpwm1 = constrain(cpwm1 + (E * 225 / 2), -255.0, 255.0);
  }

  else
  {
    cpwm1 = constrain(cpwm1 + (E * 225 / 5), -255.0, 255.0);
  }

}

void f2(float crpm)
{
  float E = (rrpm2 - crpm) / 40;
  if ((E > 0 and crpm<0) or (E < 0 and crpm>0))
  {
    cpwm2 = constrain(cpwm2 + (E * 225 / 2), -255.0, 255.0);
  }

  else
  {
    cpwm2 = constrain(cpwm2 + (E * 225 / 5), -255.0, 255.0);
  }

}


void m1(int pwm) {
  if (pwm > 0) {
    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);
    analogWrite(left_pwm_pin, pwm);

  }
  else {
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);
    analogWrite(left_pwm_pin, -pwm);

  }
}

void m2(int pwm) {
  if (pwm > 0) {
    digitalWrite(motor2pin1, HIGH);
    digitalWrite(motor2pin2, LOW);
    analogWrite(right_pwm_pin, pwm);

  }
  else {
    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
    analogWrite(right_pwm_pin, -pwm);

  }
}

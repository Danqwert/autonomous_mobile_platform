der #if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <ros.h>
#include <geometry_msgs/Twist.h>

// Pin variables for motors.
const int right_pwm_pin = 4;
const int left_pwm_pin = 9;


int motor1pin1 = 5;
int motor1pin2 = 6;
int motor2pin1 = 7;
int motor2pin2 = 8;

// Default_speed.
const int default_vel = 70;
int state_vel = default_vel;
enum State {FWD, BWD, RIGHT, LEFT, STOP};
State state;
const int max_vel = 150;

ros::NodeHandle  nh;


void MoveLeft(const size_t speed) {
  //digitalWrite(right_dir_pin, right_fwd);
  //digitalWrite(left_dir_pin, !left_fwd);
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);
  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);
  analogWrite(right_pwm_pin, speed);
  analogWrite(left_pwm_pin, speed);
}

void MoveRight(const size_t speed) {
  //digitalWrite(right_dir_pin, !right_fwd);
  //digitalWrite(left_dir_pin, left_fwd);
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, HIGH);
  analogWrite(right_pwm_pin, speed);
  analogWrite(left_pwm_pin, speed);
}

void MoveFwd(const size_t speed) {
  //digitalWrite(right_dir_pin, right_fwd);
  //digitalWrite(left_dir_pin, left_fwd);
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);
  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);
  analogWrite(right_pwm_pin, speed);
  analogWrite(left_pwm_pin, speed);
}

void MoveBwd(const size_t speed) {
  //digitalWrite(right_dir_pin, !right_fwd);
  //digitalWrite(left_dir_pin, !left_fwd);
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, HIGH);
  analogWrite(right_pwm_pin, speed);
  analogWrite(left_pwm_pin, speed);
}

void MoveStop() {
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);
  analogWrite(right_pwm_pin, 0);
  analogWrite(left_pwm_pin, 0);
}


void MoveDifferential(float x, float z_rot) {

  // x determines overall intensity. The max intensity is 255 to each motor, summing to 510 units of awesome.
  // z_rot determines how much we turn. +1 is completely left [-x*255, +x*255]. -1 is completely right [x*255, -x*255].
  // So we use the equations: (if z_rot !=0)
  // If z_rot > 0:
  //      Right = x*255
  //      Left = 255*x - 510*z_rot*x
  // If z_rot < 0:
  //      Right = 255*x - 510*|z_rot|*x
  //      Left = 255*x

  float right_vel = 0;
  float left_vel = 0;
  if (z_rot == 0.0) {
    right_vel = max_vel * x;
    left_vel = max_vel * x;
  }
  else if (z_rot > 0.0) {
    right_vel = max_vel * x;
    left_vel = max_vel * x - 2.0 * max_vel * z_rot * x;
  }
  else if (z_rot < 0.0) {
    left_vel = max_vel * x;
    right_vel = max_vel * x - 2 * max_vel * abs(z_rot) * x;
  }
  else {
    right_vel = 0.0;
    left_vel = 0.0;
  }

  if (x == 0 && z_rot != 0) {
    right_vel = (z_rot > 0) ? z_rot * max_vel : -z_rot * max_vel;
    left_vel  = (z_rot > 0) ? -z_rot * max_vel : z_rot * max_vel;
  }


  //digitalWrite(right_dir_pin, (right_vel >= 0) ? right_fwd : !right_fwd);
  if (right_vel >= 0)
  { digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);
  }
  else
  { digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);
  }

  //digitalWrite(left_dir_pin, (left_vel >= 0) ? left_fwd : !left_fwd);
  if (left_vel >= 0)
  { digitalWrite(motor2pin1, HIGH);
    digitalWrite(motor2pin2, LOW);

  }
  else
  { digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
  }

  analogWrite(right_pwm_pin, abs((int) right_vel));
  analogWrite(left_pwm_pin, abs((int) right_vel));
}

void cmd_vel_cb(const geometry_msgs::Twist & msg) {
  // Read the message. Act accordingly.
  // We only care about the linear x, and the rotational z.
  const float x = msg.linear.x;
  const float z_rotation = msg.angular.z;
    MoveDifferential(x,z_rotation);
  if (x > 0) {
    if (state == FWD) {
      state_vel += 10;
      state_vel = min(state_vel, max_vel);
    }
    MoveFwd(state_vel);
    state = FWD;
  }
  else if (x < 0) {
    if (state == BWD) {
      state_vel += 10;
      state_vel = min(state_vel, max_vel);

    }
    MoveBwd(state_vel);
    state = BWD;
  }
  else if (z_rotation < 0) {
    if (state == RIGHT) {
      state_vel += 5;
      state_vel = min(state_vel, 110);

    }
    MoveRight (state_vel);
    state = RIGHT;
  }
  else if (z_rotation > 0) {
    if (state == LEFT) {
      state_vel += 5;
      state_vel = min(state_vel, 110);

    }
    MoveLeft (state_vel);
    state = LEFT;
  }
  else {
    MoveStop();
    state_vel = default_vel;
    state = STOP;
  }

}


ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", cmd_vel_cb);

void setup() {

  pinMode(motor1pin1, OUTPUT);    // sets the digital pin 13 as output
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
  pinMode(right_pwm_pin, OUTPUT);
  pinMode(left_pwm_pin, OUTPUT);

  nh.initNode();
  nh.subscribe(sub);



}

void loop() {
  nh.spinOnce();
  delay(1);
}

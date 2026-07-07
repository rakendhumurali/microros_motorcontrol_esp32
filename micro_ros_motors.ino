#include <micro_ros_arduino.h>
#include <WiFi.h>

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <geometry_msgs/msg/twist.h>

//======================================================
// WiFi
//======================================================

char ssid[] = "realme X2";
char password[] = "alias123";

char agent_ip[] = "192.168.64.182";
uint32_t agent_port = 8888;

//======================================================
// L298N Pins
//======================================================

// Left Motor
#define ENA 33
#define IN1 25
#define IN2 26

// Right Motor
#define ENB 32
#define IN3 27
#define IN4 14

//======================================================
// PWM
//======================================================

const int PWM_FREQ = 5000;
const int PWM_RES = 8;

//======================================================
// micro-ROS
//======================================================

rcl_subscription_t subscriber;
rclc_executor_t executor;
rcl_node_t node;
rcl_allocator_t allocator;
rclc_support_t support;

geometry_msgs__msg__Twist msg;

//======================================================
// Set Left Motor
//======================================================

void setLeftMotor(float speed)
{
  speed = constrain(speed, -1.0, 1.0);

  int pwm = constrain((int)(fabs(speed) * 255), 0, 255);

  if (speed > 0)
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  }
  else if (speed < 0)
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
  else
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }

  ledcWrite(ENA, pwm);
}

//======================================================
// Set Right Motor
//======================================================

void setRightMotor(float speed)
{
  speed = constrain(speed, -1.0, 1.0);

  int pwm = constrain((int)(fabs(speed) * 255), 0, 255);

  if (speed > 0)
  {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
  else if (speed < 0)
  {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  else
  {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }

  ledcWrite(ENB, pwm);
}

//======================================================
// Stop
//======================================================

void stopMotors()
{
  setLeftMotor(0);
  setRightMotor(0);
}

//======================================================
// Callback
//======================================================
void cmd_vel_callback(const void *msgin)
{
  const geometry_msgs__msg__Twist *cmd =
      (const geometry_msgs__msg__Twist *)msgin;

  float linear = cmd->linear.x;
  float angular = cmd->angular.z;

  // Reduce turning sensitivity
  angular *= 0.5;

  // Differential drive mixing
  float leftSpeed  = linear + angular;
  float rightSpeed = linear - angular;

  // Normalize to keep values within -1.0 to 1.0
  float maxVal = max(fabs(leftSpeed), fabs(rightSpeed));

  if (maxVal > 1.0)
  {
    leftSpeed /= maxVal;
    rightSpeed /= maxVal;
  }

  Serial.println("--------------------------------");

  Serial.print("Linear  : ");
  Serial.println(linear);

  Serial.print("Angular : ");
  Serial.println(angular);

  Serial.print("Left Speed : ");
  Serial.println(leftSpeed);

  Serial.print("Right Speed : ");
  Serial.println(rightSpeed);

  Serial.print("Left PWM : ");
  Serial.println((int)(fabs(leftSpeed) * 255));

  Serial.print("Right PWM : ");
  Serial.println((int)(fabs(rightSpeed) * 255));

  setLeftMotor(leftSpeed);
  setRightMotor(rightSpeed);
}

//======================================================
// Setup
//======================================================

void setup()
{
  Serial.begin(115200);
  delay(2000);

  Serial.println("ESP32 Differential Drive");

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // PWM (Arduino ESP32 Core 3.x)
  ledcAttach(ENA, PWM_FREQ, PWM_RES);
  ledcAttach(ENB, PWM_FREQ, PWM_RES);

  stopMotors();

  // micro-ROS transport
  set_microros_wifi_transports(
      ssid,
      password,
      agent_ip,
      agent_port);

  delay(3000);

  allocator = rcl_get_default_allocator();

  rclc_support_init(
      &support,
      0,
      NULL,
      &allocator);

  rclc_node_init_default(
      &node,
      "esp32_diff_drive",
      "",
      &support);

  rclc_subscription_init_default(
      &subscriber,
      &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(
          geometry_msgs,
          msg,
          Twist),
      "/cmd_vel");

  rclc_executor_init(
      &executor,
      &support.context,
      1,
      &allocator);

  rclc_executor_add_subscription(
      &executor,
      &subscriber,
      &msg,
      &cmd_vel_callback,
      ON_NEW_DATA);

  Serial.println("micro-ROS Ready");
}

//======================================================
// Loop
//======================================================

void loop()
{
  rclc_executor_spin_some(
      &executor,
      RCL_MS_TO_NS(10));

  delay(10);
}
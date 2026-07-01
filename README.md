# ESP32 micro-ROS Differential Drive Robot Controller

## Overview

This project implements a **ROS 2-based differential drive robot controller** using an **ESP32**, **micro-ROS**, and an **L298N motor driver**. The ESP32 connects to a ROS 2 network over Wi-Fi, subscribes to the `/cmd_vel` topic, and controls two DC motors using differential drive kinematics.

## Features

- ESP32-based motor controller
- ROS 2 communication using micro-ROS
- Wi-Fi connectivity
- Subscribes to `/cmd_vel` (`geometry_msgs/Twist`)
- Differential drive kinematics
- PWM speed control
- Forward, reverse, left, right, and stop movements
- Serial debugging support

## Hardware Requirements

- ESP32 Development Board
- L298N Dual H-Bridge Motor Driver
- 2 × DC Geared Motors
- Robot Chassis
- Battery Pack
- Wi-Fi Network
- Computer running Ubuntu with ROS 2 Humble

## Software Requirements

- Arduino IDE
- ESP32 Board Package
- micro_ros_arduino Library
- ROS 2 Humble
- micro-ROS Agent

## Pin Configuration

| Function | ESP32 Pin |
|----------|-----------|
| Left Motor IN1 | GPIO 25 |
| Left Motor IN2 | GPIO 26 |
| Right Motor IN3 | GPIO 27 |
| Right Motor IN4 | GPIO 14 |
| Left Motor PWM (ENA) | GPIO 32 |
| Right Motor PWM (ENB) | GPIO 33 |

## Working Principle

The ESP32 connects to the configured Wi-Fi network and establishes communication with the micro-ROS Agent running on a ROS 2 computer. It subscribes to the `/cmd_vel` topic and receives `geometry_msgs/Twist` messages containing linear and angular velocity commands.

The controller computes the individual wheel speeds using differential drive kinematics:

```
Left Wheel  = Linear Velocity - Angular Velocity
Right Wheel = Linear Velocity + Angular Velocity
```

The wheel speeds are normalized, converted into PWM values, and sent to the L298N motor driver to control the left and right DC motors.

## Repository Structure

```
.
├── esp32_motor_controller.ino
├── README.md
└── images/
```

## Installation

### 1. Install ROS 2 Humble

Follow the official ROS 2 installation guide.

### 2. Install micro-ROS Agent

```bash
docker pull microros/micro-ros-agent:humble
```

Run the agent:

```bash
docker run -it --rm --net=host microros/micro-ros-agent:humble udp4 --port 8888
```

### 3. Install Arduino Libraries

Install:

- micro_ros_arduino
- ESP32 Board Package

### 4. Configure Wi-Fi

Edit the following values in the source code:

```cpp
char ssid_c[] = "YOUR_WIFI_NAME";
char password_c[] = "YOUR_WIFI_PASSWORD";
char agent_ip_c[] = "YOUR_COMPUTER_IP";
```

### 5. Upload the Code

Upload the sketch to the ESP32 using the Arduino IDE.

## Usage

Start the micro-ROS Agent and publish velocity commands from ROS 2.

### Move Forward

```bash
ros2 topic pub /cmd_vel geometry_msgs/msg/Twist "{linear: {x: 1.0}, angular: {z: 0.0}}"
```

### Move Backward

```bash
ros2 topic pub /cmd_vel geometry_msgs/msg/Twist "{linear: {x: -1.0}, angular: {z: 0.0}}"
```

### Turn Left

```bash
ros2 topic pub /cmd_vel geometry_msgs/msg/Twist "{linear: {x: 0.0}, angular: {z: -1.0}}"
```

### Turn Right

```bash
ros2 topic pub /cmd_vel geometry_msgs/msg/Twist "{linear: {x: 0.0}, angular: {z: 1.0}}"
```

### Stop

```bash
ros2 topic pub /cmd_vel geometry_msgs/msg/Twist "{linear: {x: 0.0}, angular: {z: 0.0}}"
```



## Author

**Rakendhu**

Developed using **ESP32**, **ROS 2 Humble**, **micro-ROS**, and **L298N** for differential drive robot control.

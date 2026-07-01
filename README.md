#  ESP32 micro-ROS Differential Drive Robot
## Overview

This project demonstrates a ROS 2 controlled differential drive robot using an ESP32 and micro-ROS. The ESP32 communicates with a ROS 2 system over Wi-Fi using the micro-ROS framework and subscribes to the `/cmd_vel` topic to receive velocity commands.

The robot is controlled using the `teleop_twist_keyboard` package in ROS 2. A micro-ROS Agent running inside a Docker container bridges the communication between the ROS 2 network and the ESP32.

The received linear and angular velocities are converted into left and right wheel speeds using differential drive kinematics, and the motors are driven through an L298N motor driver using PWM.
## Features

- ROS 2 communication using micro-ROS
- ESP32 as a micro-ROS client
- Wi-Fi based communication
- Docker-based micro-ROS Agent
- Differential drive robot control
- PWM speed control using ESP32 LEDC
- Real-time motor control
- Support for forward, backward, left, right and stop movements
- Debug messages through Serial Monitor

- ## Hardware Requirements

-ESP32 Development Board
-L298N Motor Driver
-2 DC Gear Motors
-Differential Drive Chassis
-Li-ion Battery Pack

## Software Requirements

- Ubuntu
- ROS 2 Humble
- Docker
- Arduino IDE
- micro_ros_arduino Library
- teleop_twist_keyboard package
  ## Working Principle

The ESP32 connects to the configured Wi-Fi network and establishes communication with the micro-ROS Agent running on a ROS 2 computer. It subscribes to the /cmd_vel topic and receives geometry_msgs/Twist messages containing linear and angular velocity commands.

The controller computes the individual wheel speeds using differential drive kinematics:


Left Wheel  = Linear Velocity - Angular Velocity
Right Wheel = Linear Velocity + Angular Velocity


The wheel speeds are normalized, converted into PWM values, and sent to the L298N motor driver to control the left and right DC motors.

## Repository Structure


.
├── esp32_motor_controller.ino
├── README.md
└── images/


## Installation

### 1. Install ROS 2 Humble

Follow the official ROS 2 installation guide.

### 2. Install micro-ROS Agent

bash
docker pull microros/micro-ros-agent:humble


Run the agent:

bash
docker run -it --rm --net=host microros/micro-ros-agent:humble udp4 --port 8888


### 3. Install Arduino Libraries

Install:

- micro_ros_arduino
- ESP32 Board Package

### 4. Configure Wi-Fi

Edit the following values in the source code:

cpp
char ssid_c[] = "YOUR_WIFI_NAME";
char password_c[] = "YOUR_WIFI_PASSWORD";
char agent_ip_c[] = "YOUR_COMPUTER_IP";


### 5. Upload the Code

Upload the sketch to the ESP32 using the Arduino IDE.

## Usage

Start the micro-ROS Agent and publish velocity commands from ROS 2.

### Move Forward

bash
ros2 topic pub /cmd_vel geometry_msgs/msg/Twist "{linear: {x: 1.0}, angular: {z: 0.0}}"


### Move Backward

bash
ros2 topic pub /cmd_vel geometry_msgs/msg/Twist "{linear: {x: -1.0}, angular: {z: 0.0}}"


### Turn Left

bash
ros2 topic pub /cmd_vel geometry_msgs/msg/Twist "{linear: {x: 0.0}, angular: {z: -1.0}}"


### Turn Right

bash
ros2 topic pub /cmd_vel geometry_msgs/msg/Twist "{linear: {x: 0.0}, angular: {z: 1.0}}"


### Stop

bash
ros2 topic pub /cmd_vel geometry_msgs/msg/Twist "{linear: {x: 0.0}, angular: {z: 0.0}}"


## Serial Monitor Output

The serial monitor displays:

- Wi-Fi connection status
- ESP32 IP address
- micro-ROS initialization status
- Incoming linear velocity
- Incoming angular velocity
- Left motor PWM
- Right motor PWM

## Future Improvements

- Wheel encoder integration
- PID speed control
- Odometry publishing
- Battery monitoring
- Obstacle avoidance
- Autonomous navigation

## Contributing

Contributions are welcome. Feel free to fork this repository, create a new branch, and submit a pull request.

## License

This project is licensed under the MIT License.

## Author

*Rakendhu*

Developed using *ESP32, **ROS 2 Humble, **micro-ROS, and **L298N* for differential drive robot control.

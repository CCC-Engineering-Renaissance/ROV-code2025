# Crush-Depth MATE ROV Code 2025
# Summary
MATE ROV Code is the foundational software for piloting and communicating with the ROV. It defines the logic for thruster control using PWM (Pulse Width Modulation) to send signals to all 8 thrusters, enabling precise maneuverability. The code also includes functionality to control the claw, allowing for both opening/closing and rotational movement, making it easy to grab or move objects underwater. Additionally, it employs concurrency to ensure a robust and reliable multithreaded system that continuously processes user commands in real time.

The code in this repo is clear and easy to read. External links are provided for additional repositories that must be downloaded to ensure all dependencies work correctly and the ROV navigates the pool as intended.
## Dependencies
- Eigen3 (Linear Algebra)
- Boost asio (Server)
- matrix.h (Calculates Motor Power for Thrusters)
- thruster.h (Control Electronic Speed Controllers)
## Installation Instructions
Clone Repo:
git clone https://github.com/CCC-Engineering-Renaissance/ROV-code2025.git

Install Dependencies:
- sudo apt-get update
- sudo apt-get install libeigen3-dev libboost-all-dev
- Git Clone External Libraries Provided Below

Cd Into:
```
ROV-code2025
```
Run:
```
make
./a.out
```
Start Submerging 🌊
## External Libraries to Pull From GitHub
- lgpio - GPIO Control Library (https://github.com/joan2937/lg)
- Eigen - Library for Linear Algebra for C++ (https://gitlab.com/libeigen/eigen/-/releases)
- Paho MQTT (https://github.com/eclipse-paho/paho.mqtt.cpp)
## Running The Code
```
make
./a.out
```

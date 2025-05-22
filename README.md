# Crush-Depth MATE ROV Code 2025
# Summary
MATE ROV Code is the foundational software for controlling the ROV. It defines the logic for thruster control using PWM (Pulse Width Modulation) to send signals to all 8 thrusters. The code also includes functionality to control the claw, allowing for both opening/closing and rotational movement. Additionally, it employs concurrency to ensure a strong and reliable multithreaded system that continuously processes user commands in real-time.
## Dependencies
- Eigen3 (Linear Algebra)
- Boost asio (Server)
- matrix.h (Calculates Motor Power for Thrusters)
- thruster.h (Control Electronic Speed Controllers)
## External Libraries to Pull From GitHub
- lgpio - GPIO Control Library (https://github.com/joan2937/lg)
- Eigen - Library for Linear Algebra for C++ (https://gitlab.com/libeigen/eigen/-/releases)
- Paho MQTT (https://github.com/eclipse-paho/paho.mqtt.cpp)
## Running The Code
```
make
./a.out
```

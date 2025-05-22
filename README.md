# Crush-Depth MATE ROV Code 2025
# Summary
MATE ROV Code is the foundational software for controlling the ROV. It defines the logic for thruster control using PWM (Pulse Width Modulation) to send signals to all 8 thrusters. The code also includes functionality to control the claw, allowing for both opening/closing and rotational movement. Additionally, it employs concurrency to ensure a strong and reliable multithreaded system that continuously processes user commands in real-time.
## Dependencies
- Eigen3 (Linear Algebra)
- Boost asio (Server)
## Running The Code
```
make
./a.out
```

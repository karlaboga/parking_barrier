# Arduino Parking Barrier System

This project implements an automated parking barrier system using an Arduino board. The system monitors vehicle entry and exit with the help of infrared (IR) sensors and automatically controls a servo motor that acts as a parking barrier.

When a vehicle is detected at the entrance, the system checks the number of available parking slots. If a slot is available, the barrier opens and the available slot count is decreased. When a vehicle exits, the barrier opens again and the slot count is increased, up to a predefined maximum.

The number of available parking spaces is displayed in real time on a 7-segment display. An alarm function is triggered if the slot count goes out of valid bounds, ensuring reliable system operation.

Features

- Automatic barrier control using a servo motor
- Vehicle detection at entry and exit using IR sensors
- Real-time parking slot display on a 7-segment display
- Slot limit protection with visual alarm
- Serial monitoring for debugging

Components Used:

- Arduino board (e.g. Arduino Uno)
- Servo motor
- 2 × Infrared (IR) sensors
- 7-segment display
- Resistors and jumper wires
- Breadboard
- Power supply

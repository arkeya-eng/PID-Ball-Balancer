# Golf Ball Balancing PID Controller

Designed and built a closed-loop control system that balances a golf ball on a beam in real time using an Arduino, IR distance sensor, and servo motor. Involved CAD design, 3D printing, sensor calibration, signal filtering, and PID controller tuning. Two full design iterations from prototype to final build.

![Demo Video](media/demo.gif)
<!-- Replace with an actual gif or remove this line if you don't have one yet -->

![Final Build V2](media/Ver2.gif)

## Overview

The system balances a golf ball at a target position on a beam by continuously reading the ball's position with an infrared distance sensor and adjusting the beam angle via a servo motor. A calibrated ADC-to-distance lookup table converts raw sensor readings to centimetres, and a PD control loop computes the required servo correction in real time.

Two full design iterations were completed — a rough prototype to validate the concept, followed by a refined final build with improved housing and sensor mounting.

## How It Works

1. **Sense** — Sharp IR sensor reads an analog voltage proportional to the ball's distance. Raw ADC values are smoothed with an exponential moving average (α = 0.2) and mapped to centimetres via a 16-point calibration table with linear interpolation.

2. **Compute** — A PD controller calculates the error between the target position and the current position. The derivative term damps oscillation. Output is clamped to a safe servo range (60°–180°).

3. **Actuate** — An Arduino Uno R3 writes the computed angle to a servo motor, which tilts the beam to drive the ball toward the setpoint.

## Hardware

| Component | Detail |
|---|---|
| Microcontroller | Arduino Uno R3 |
| Sensor | Sharp GP2Y0A21YK0F IR distance sensor |
| Actuator | YM-2765 servo motor |
| Housing | Custom designed in SolidWorks, 3D printed (PLA) |
| Ball | Standard golf ball |
| Structure | 2 x Steel (1m x 6mm) Rods|

## Repo Structure

```
PID-Ball-Balancer/
├── src/
│   └── main.ino          # Arduino source code
├── cad/
│   ├── prototype/        # V1 SolidWorks files
│   └── final/            # V2 SolidWorks files
├── media/
│   ├── demo.gif          # Short demo clip
│   ├── prototype.jpg     # Photo of V1 build
│   └── final.jpg         # Photo of V2 build
├── docs/
│   └── wiring.png        # Wiring diagram
└── README.md
```

## Wiring

```
Arduino Uno R3
├── A0  ←  IR sensor signal (yellow)
├── 5V  →  IR sensor VCC (red)
├── GND →  IR sensor GND (black)
├── D9  →  Servo signal (orange)
├── External Battery Holder (4xAA)  →  Servo VCC (red)
└── GND & External GND →  Servo GND (brown)
```

## Control Parameters

```
Kp = 0.2      // Proportional gain
Kd = 0.0      // Derivative gain (tuned out — system stable with P-only)
Setpoint = 46.0 cm from sensor
Neutral angle = 120°
Loop rate ≈ 100 Hz (10 ms delay)
```

## Build Process

**Prototype (V1)** — Quick proof-of-concept to validate sensor placement, servo torque, and basic control loop. Rough 3D-printed cradle, manual wiring.

**Final Build (V2)** — Redesigned housing with integrated sensor mount, cleaner cable routing, and tighter beam pivot. Improved stability and repeatability.

## Video

A build and tuning walkthrough video is included in the `/media` folder (or linked below once uploaded).

(https://youtu.be/jKo0sEahJNQ)


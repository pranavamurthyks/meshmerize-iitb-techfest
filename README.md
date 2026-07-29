# ESP32 Maze Solving Line Follower

An autonomous maze-solving line follower built using an ESP32, N20 geared DC motors, and an L298N motor driver. The robot was developed for **IIT Bombay Techfest 2025** and is capable of learning an unknown maze, optimizing the recorded path, and replaying the shortest route.

---

## Overview

The robot follows a black line using PID control and explores the maze using the **Left-Hand Rule** during its first run. As it navigates, it records every junction decision, removes unnecessary paths through maze optimization, and then completes subsequent runs using the optimized shortest path.

---

## Features

- PID-based line following
- Left-hand rule maze solving
- Automatic dead-end detection and backtracking
- Junction detection
- Maze learning (dry run)
- Path optimization
- Shortest path replay
- 8-channel IR sensor array

---

## Competition

This robot was designed and built as part of our participation in **IIT Bombay Techfest 2025**, where the objective was to autonomously solve an unknown line maze in the shortest possible time.

---

## Hardware

| Component | Description |
|-----------|-------------|
| Microcontroller | ESP32 Development Board |
| Motor Driver | L298N |
| Motors | N20 Geared DC Motors |
| Sensors | 8-Channel IR Reflectance Sensor Array |
| Power | 2S Li-ion Battery |

---

## Software

- Arduino IDE
- C++

---

## Working Principle

### Learning Phase

The robot explores the maze using the **Left-Hand Rule**.

Each junction decision is recorded as:

| Symbol | Meaning |
|---------|---------|
| L | Left |
| R | Right |
| S | Straight |
| B | Backtrack (Dead End) |

Example:

```
L → S → L → B → R
```

---

### Path Optimization

After reaching the destination, redundant turns are removed using maze reduction rules.

Examples:

```
L B L → S
L B S → R
S B L → R
R B R → S
```

The optimization continues until no further simplifications are possible.

---

### Replay Phase

The robot follows the optimized path instead of exploring the maze, significantly reducing the completion time.

---

## PID Controller

The robot estimates the line position using a weighted average of the IR sensor readings.

```
Correction = Kp × Error + Ki × Integral + Kd × Derivative
```

Current tuning:

```
Kp = 200
Ki = 0
Kd = 600
```

---

## Future Improvements

- Encoder-based motion control
- Adaptive PID tuning
- Persistent storage for learned paths
- Modular code architecture
- Higher-speed cornering

---

## Demo

*Add a short video or GIF of both the learning run and the optimized replay run.*

---

## License

MIT License

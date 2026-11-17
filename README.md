# Ball & Beam PID Controller

A closed-loop control system that balances a rolling ball on a tilting beam. An ultrasonic sensor measures the ball's position, and a custom PID controller sets the beam angle with a PWM-driven servo so the ball stays at the target point.

<!-- Add a photo or demo GIF here, e.g. ![Ball & Beam](images/demo.gif) -->

## Features

- **Custom PID controller** written from scratch
- **Ultrasonic position feedback** for real-time measurement of the ball's position
- **PWM servo actuation** to tilt the beam
- **Continuous balancing:** the system reacts to disturbances and brings the ball back to the setpoint

## How It Works

```
Setpoint ──►(+)──► PID controller ──► PWM ──► Servo ──► Beam angle ──► Ball position
            (−)                                                              │
             ▲                                                               │
             └──────────────────── Ultrasonic sensor ◄───────────────────────┘
```

On every loop iteration:

1. **Measure:** the ultrasonic sensor reads the ball's distance.
2. **Error:** `error = setpoint − position`
3. **PID output:**

   ```
   u(t) = Kp·e(t) + Ki·∫e(t)dt + Kd·de(t)/dt
   ```

   - **P (proportional):** reacts to the current error
   - **I (integral):** removes steady-state error
   - **D (derivative):** damps oscillations by reacting to how fast the error changes
4. **Actuate:** the output is turned into a PWM signal that sets the servo angle, which tilts the beam.

## Hardware

| Component | Role |
|---|---|
| Arduino microcontroller | Runs the PID loop |
| Ultrasonic distance sensor | Measures the ball's position |
| Servo motor | Tilts the beam |
| Beam + ball | The controlled system |
| Power supply | Powers the servo and the board |

## Tuning

The `Kp`, `Ki` and `Kd` gains are set in the code. A common tuning order:

1. Start with `Ki = Kd = 0` and raise `Kp` until the ball oscillates around the setpoint.
2. Raise `Kd` to damp the oscillations.
3. Add a small `Ki` to remove any remaining offset from the setpoint.

## Getting Started

1. Clone the repository:
   ```bash
   git clone https://github.com/omershtanger/Ball-Beam-PID-Controller.git
   ```
2. Open the sketch in the Arduino IDE.
3. Wire the sensor and the servo according to the pin definitions in the code.
4. Upload, place the ball on the beam, and adjust the gains if needed.

## Author

**Omer Shtanger**

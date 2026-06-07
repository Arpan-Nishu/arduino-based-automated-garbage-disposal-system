# ARDUINO BASED AUTOMATED GARBAGE DISPOSAL SYSTEM

An intelligent, sensor-driven garbage bin that automatically opens its lid when approached, monitors fill level in real-time, and alerts when full — built entirely with Arduino.

## Features

- **Hands-free operation** — IR sensor detects hand/object and automatically opens lid via servo motor
- **Fill level monitoring** — HC-SR04 ultrasonic sensor measures garbage height inside the bin
- **Real-time display** — 16×2 I2C LCD shows fill percentage with visual bar graph
- **Multi-stage alerts** — Green LED (empty), Yellow LED (half), Red LED + buzzer (full)
- **Auto-close** — Lid closes automatically after 5 seconds
- **Serial monitoring** — Real-time debug output via USB serial

## Components Required

| Component                | Quantity | Approx. Cost (₹) |
|--------------------------|----------|------------------|
| Arduino Uno R3           | 1        | 500              |
| HC-SR04 Ultrasonic Sensor| 1        | 80               |
| IR Obstacle Sensor       | 1        | 50               |
| SG90 Servo Motor         | 1        | 120              |
| 16×2 I2C LCD Module      | 1        | 250              |
| 5V Buzzer (active)       | 1        | 20               |
| LED (Green, Yellow, Red) | 3        | 15               |
| 220Ω Resistors           | 3        | 6                |
| Breadboard + Jumper Wires| 1 set    | 100              |
| 5V 2A Power Adapter      | 1        | 150              |
| **Total (approx)**       |          | **₹1,291**       |

## Circuit Connections

```
Arduino Uno          → Component
─────────────────────────────────
D5   (PWM)          → Green LED (anode)  → 220Ω → GND
D6   (PWM)          → Yellow LED (anode) → 220Ω → GND
D7   (PWM)          → Red LED (anode)    → 220Ω → GND
D8   (Digital In)   → IR Sensor OUT
D9   (Digital Out)  → HC-SR04 TRIG
D10  (Digital In)   → HC-SR04 ECHO
D11  (PWM)          → Servo Signal (yellow/orange)
D12  (Digital Out)  → Buzzer (+ve leg)
A4   (SDA)          → LCD SDA
A5   (SCL)          → LCD SCL
5V                  → HC-SR04 VCC, IR VCC, Servo VCC (red), LCD VCC
GND                 → All component grounds
```

### Schematic (Text View)

```
                    ┌─────────────────────────────┐
                    │        ARDUINO UNO          │
                    │                             │
  ┌─────────────────┤ D9  ──→ TRIG (HC-SR04)     │
  │                 │ D10 ←── ECHO (HC-SR04)      │
  │  HC-SR04        │                             │
  │  (Ultrasonic)   │ D8  ←── OUT (IR Sensor)    │
  │                 │                             │
  │                 │ D11 ──→ Signal (Servo)      │
  │                 │                             │
  │  IR Sensor      │ D12 ──→ Buzzer (+)          │
  │                 │                             │
  │  Servo SG90     │ D5  ──→ Green LED + 220Ω    │
  │                 │ D6  ──→ Yellow LED + 220Ω   │
  │                 │ D7  ──→ Red LED + 220Ω      │
  │  LCD 16×2 I2C   │                             │
  │                 │ A4 (SDA) ──→ LCD SDA        │
  │                 │ A5 (SCL) ──→ LCD SCL        │
  └─────────────────┤                             │
                    │ VCC ──→ 5V to all modules    │
                    │ GND ──→ Common ground        │
                    └─────────────────────────────┘
```

## How It Works

1. **On startup** — LCD shows *"Garbage Disposal System Ready..."*, servo calibrates to closed position, green LED turns on.
2. **Normal operation** — Ultrasonic sensor reads distance to garbage surface every 500ms. Fill percentage is calculated and shown on LCD.
3. **Lid opening** — When IR sensor detects a hand/object within ~10cm, the servo rotates to 90° (lid open). A 5-second timer starts.
4. **Lid closing** — After 5 seconds with no detection, the servo returns to 0° (lid closed). If IR detects again during this window, the timer resets.
5. **Fill alerts** — At 50% fill, yellow LED lights up. At 90%+, red LED lights up and buzzer beeps 3 times in alert mode.
6. **Full notification** — LCD shows *"! BIN FULL !"* when bin reaches 90%+ capacity.

## Code Structure

| File | Description |
|------|-------------|
| `arduino-code/automated_garbage_disposal.ino` | Main Arduino sketch |
| `docs/README.md` | Project documentation |
| `docs/circuit.fzz` | Fritzing circuit diagram source |
| `docs/schematic.pdf` | Schematic PDF |

## Pin Configuration

| Pin  | Type   | Connected To       | Purpose          |
|------|--------|--------------------|------------------|
| 5    | OUTPUT | Green LED          | Empty indicator  |
| 6    | OUTPUT | Yellow LED         | Half indicator   |
| 7    | OUTPUT | Red LED            | Full indicator   |
| 8    | INPUT  | IR Sensor OUT      | Hand detection   |
| 9    | OUTPUT | Ultrasonic TRIG    | Trigger pulse    |
| 10   | INPUT  | Ultrasonic ECHO    | Echo reception   |
| 11   | OUTPUT | Servo Signal       | Lid control      |
| 12   | OUTPUT | Buzzer             | Full alert       |
| A4   | I2C    | LCD SDA            | Display data     |
| A5   | I2C    | LCD SCL            | Display clock    |

## Customization

Edit these constants at the top of the Arduino sketch:

```cpp
#define BIN_DEPTH_CM      30    // Total bin depth in cm
#define FULL_THRESHOLD_CM  5    // Distance threshold for "full"
#define HALF_THRESHOLD_CM  15   // Distance threshold for "half"
#define OPEN_DURATION_MS   5000 // How long lid stays open (ms)
```

## Installation

```bash
# Clone the repository
git clone https://github.com/Arpan-Nishu/ARDUINO-BASED-AUTOMATED-GARBAGE-DISPOSAL-SYSTEM.git

# Open the sketch
cd ARDUINO-BASED-AUTOMATED-GARBAGE-DISPOSAL-SYSTEM/arduino-code
# Open automated_garbage_disposal.ino in Arduino IDE

# Select board: Tools → Board → Arduino Uno
# Select port: Tools → Port → /dev/cu.usbmodemXXXXX
# Upload: Sketch → Upload (Ctrl+U)
```

## Simulation

[Click here to simulate on Wokwi](https://wokwi.com/projects/new/arduino-uno)

Paste the code from `arduino-code/automated_garbage_disposal.ino` into the Wokwi editor, then wire the components as shown in the circuit diagram above.

## Testing

1. **Power on** — LEDs test (all blink once), servo calibrates, LCD shows ready
2. **Wave hand** over IR sensor — lid opens, LCD shows *"Lid Open"*
3. **Wait 5 seconds** — lid closes automatically
4. **Cover ultrasonic sensor** — fill % increases on LCD, LEDs change color
5. **Fully cover** — red LED + buzzer alert, LCD shows *"! BIN FULL !"*
6. **Open Serial Monitor** (9600 baud) — real-time debug output

## License

MIT License — free to use, modify, and distribute.

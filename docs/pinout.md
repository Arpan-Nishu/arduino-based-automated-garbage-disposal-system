# Pinout Reference

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

## Pin Mapping Table

| Arduino Pin | Type    | Connected To         | Cable Color   |
|-------------|---------|----------------------|---------------|
| D5          | OUTPUT  | Green LED (anode)    | Green         |
| D6          | OUTPUT  | Yellow LED (anode)   | Yellow        |
| D7          | OUTPUT  | Red LED (anode)      | Red           |
| D8          | INPUT   | IR Sensor OUT        | Blue          |
| D9          | OUTPUT  | HC-SR04 TRIG         | Yellow        |
| D10         | INPUT   | HC-SR04 ECHO         | White         |
| D11         | OUTPUT  | Servo Signal         | Orange        |
| D12         | OUTPUT  | Buzzer (+)           | Purple        |
| A4          | I2C/SDA | LCD SDA              | Blue          |
| A5          | I2C/SCL | LCD SCL              | Green         |
| 5V          | POWER   | VCC (all modules)    | Red           |
| GND         | POWER   | GND (all modules)    | Black         |

## LED Resistor Values

| LED    | Resistor | Color Code        |
|--------|----------|-------------------|
| Green  | 220Ω     | Red-Red-Brown     |
| Yellow | 220Ω     | Red-Red-Brown     |
| Red    | 220Ω     | Red-Red-Brown     |

## Power Requirements

| Component      | Voltage | Current (max) |
|----------------|---------|---------------|
| Arduino Uno    | 7-12V   | 500mA         |
| HC-SR04        | 5V      | 15mA          |
| IR Sensor      | 5V      | 10mA          |
| Servo SG90     | 5V      | 250mA         |
| LCD 16x2 I2C   | 5V      | 60mA          |
| Buzzer         | 5V      | 30mA          |
| LEDs ×3        | 2V ea   | 20mA ea       |
| **Total**      |         | **~700mA**    |

> A 5V 2A power adapter is recommended to handle peak servo current draw.

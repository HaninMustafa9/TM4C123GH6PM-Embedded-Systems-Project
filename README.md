# TM4C123GH6PM Multi-Function Project

This repository contains a project for the TM4C123GH6PM microcontroller, implementing three core functionalities:

1. **Measuring Temperature** using the internal temperature sensor.
2. **Controlling LED Intensity** with PWM.
3. **Writing Messages on an LCD** using UART communication.

---

## Features

### 1. Measuring Temperature
- Utilizes the internal temperature sensor of the TM4C123GH6PM.
- Reads the temperature data and converts it into a readable format (Celsius/Fahrenheit) based on button clicking.
- Displays the temperature on the connected LCD screen.

### 2. Controlling LED Intensity with PWM
- Implements Pulse Width Modulation (PWM) to control the brightness of an LED.
- Adjusts brightness through based on an ADC input from a potentiometer.

### 3. Writing Messages on an LCD with UART
- Writing messages to an LCD via UART.
- Supports clear display, deleting Characters and line updates.

---

## Hardware Requirements

1. **TM4C123GH6PM LaunchPad**
2. **LCD Module (20x4)**
3. **LED with Resistor**
4. **Breadboard and Jumper Wires**
5. **Potentiometer**
6. **External Button**

---

## Software Requirements

1. **Keil uVision IDE**
2. **TM4C123GH6PM Drivers and Peripheral Libraries**
3. **USB-to-UART Bridge** (if needed)

---

## Setup and Usage

### 1. Clone the Repository
```bash
git clone https://github.com/HaninMustafa9/TM4C123GH6PM-Embedded-Systems-Project
cd TM4C123GH6PM-Embedded-Systems-Project
```

### 2. Import the Project
- Open the project in Keil uVision IDE.
- Ensure the proper configurations for the TM4C123GH6PM microcontroller.

### 3. Pin Configuration
| **Functionality**      | **TM4C123 Pin** | **Peripheral** |
|------------------------|-----------------|----------------|
| Temperature Sensor     | Internal        | ADC0           |
| LED (PWM)              | PB6 (example)   | PWM1           |
| UART (LCD Interface)   | PA0, PA1        | UART0          |

### 4. Compile and Flash
- Compile the project in Keil uVision IDE.
- Flash the binary file to the TM4C123GH6PM using a debugger/programmer.

### 5. Run the Project
- Power the microcontroller.
- Observe:
  - The measured temperature on the LCD.
  - LED brightness changes controlled by PWM and POT.
  -  Write messages to the LCD.

---



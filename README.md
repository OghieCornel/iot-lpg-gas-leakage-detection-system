# Gas Leak Detection System (LPG & Butane)

## Overview

This project presents the design and implementation of a low-cost, real-time LPG and Butane Gas Leak Detection System using an ESP8266 (Wemos D1 Mini), MQ6 gas sensor, OLED display, and buzzer. The system continuously monitors the air for combustible gases and provides both visual and audible alerts based on calibrated gas concentration thresholds.

The device operates as a standalone safety system suitable for homes, kitchens, and small industrial facilities. It also supports optional Wi-Fi configuration and IoT integration for real-time remote monitoring and notifications.

---

## Features

* Real-time LPG and Butane gas detection
* Gas concentration display in Parts Per Million (PPM)
* 0.96" OLED display for live readings and status
* Multi-level alert system (Clean, Mild Leak, High, Danger)
* Audible buzzer alerts based on gas concentration
* 1-minute automatic sensor warm-up stabilization


---

## System Architecture

<img width="400" height="270" alt="Gas Leak Detection (LPG   Butane Gas) (1)_page-0001" src="https://github.com/user-attachments/assets/7847845c-9b15-4f69-a00f-83b7e1023f20" />

<img width="325" height="250" alt="Screenshot 2025-05-16 193255" src="https://github.com/user-attachments/assets/d8d34021-b9ab-4ec0-97ef-5f939face8a5" />

<img width="325" height="250" alt="Screenshot 2025-05-16 193414" src="https://github.com/user-attachments/assets/95629081-f284-46fb-87c0-1c6512a28850" />

---

## Hardware Components

* ESP8266 (Wemos D1 Mini)
* MQ6 Gas Sensor (LPG/Butane detection)
* 0.96" OLED Display
* Buzzer
* 5V USB Power Source
* 8V 4000MAH battery
* BC547 Transistor
* Resistors 4.7k
* TP4056

---

## Software & Platforms Used

### Back End

* Microcontroller: ESP8266
* Programming Language: C++
* Development Environment: Arduino IDE
* Libraries: Blynk library, Adafruit OLED libraries
* WiFi Configuration: WiFiManager

### Front End

* Platform: Blynk IoT


---

## Device Operation

### 1. Power On

* Power the device using the switch.
* Connect to a 5V USB source if not fully charged.

OLED displays:

* "Gas Leak Detector"
* "LPG/Butane Detector"
* "Warming up…"

### Sensor Warm-Up (1 Minute)

<img width="444" height="773" alt="reallpgiot4" src="https://github.com/user-attachments/assets/f95a55e9-976d-4e60-8a8b-b9bad49f69d1" />


* Sensor stabilizes for accurate readings.
* PPM values are displayed.
* Alerts are suppressed.
* Buzzer remains OFF.
<img width="249" height="540" alt="iot lpg gas detection" src="https://github.com/user-attachments/assets/fd4f4126-2ff8-4096-9534-4fe0aef072e0" />

---

## Gas Detection Levels

| PPM Range | Status             | Buzzer     | OLED Message        |
| --------- | ------------------ | ---------- | ------------------- |
| 0–600     | Clean Air          | OFF        | "Air is clean."     |
| 601–999   | Mild Leak          | Beep Once  | "Gas Detected!"     |
| 1000–1999 | High Concentration | Beep Once  | "Warning! Gas High" |
| ≥2000     | Danger             | Continuous | "DANGER! Gas Leak!" |


---

## IoT Monitoring

### Blynk IoT Platform

* Displays real-time PPM readings
* Shows sensor status
* Provides remote monitoring via mobile Blynk app dashboard


---

## Testing

### Butane Simulation Test (Optional)

* Release a small amount of lighter gas near the MQ6 sensor.
* Observe increase in PPM readings.
* Verify OLED status update.
* Confirm buzzer behavior matches alert level.

---

## Maintenance & Safety

* Keep MQ6 sensor free from dust.
* Avoid direct liquid exposure.
* Calibrate periodically in clean air.
* Ensure stable 5V power supply.
* If buzzer sounds continuously, confirm no actual gas leak before troubleshooting.

---

## Troubleshooting

| Issue                 | Possible Cause                       | Solution                                        |
| --------------------- | ------------------------------------ | ----------------------------------------------- |
| OLED not displaying   | Loose I2C wires or incorrect address | Verify SDA/SCL connections and address 0x3C     |
| Always shows high PPM | Contaminated sensor                  | Allow sensor to air out; avoid alcohol exposure |
| Buzzer not working    | Wrong pin or faulty buzzer           | Check D5 connection or replace buzzer           |

---

## Institution

Department of Computer Engineering
Faculty of Engineering
Edo University Iyamho

---

## Supervisor

Engr. Dr. Chukwuemeka .C. Obasi

# Smart Air Quality and Noise Monitoring System

## Overview

This project presents a portable IoT-based environmental monitoring system designed to measure indoor air quality and environmental noise levels in real time.

The system uses an **ESP32** microcontroller as the main processing unit, an **MQ135 gas sensor** to estimate CO₂ concentration and evaluate air quality, and an **MKE-S06 sound sensor** to monitor ambient noise levels. The collected data are processed by the ESP32 and transmitted to the **Blynk IoT platform** via Wi-Fi, allowing users to remotely monitor environmental conditions using a smartphone.

---

## Features

* Real-time CO₂ concentration monitoring.
* Real-time environmental noise monitoring.
* Wireless data transmission via Wi-Fi.
* Remote visualization using the Blynk mobile application.
* Threshold-based warning system for high CO₂ concentration and excessive noise levels.
* Portable and low-cost environmental monitoring solution.

---

## Hardware Components

| Component  | Description                         |
| ---------- | ----------------------------------- |
| ESP32      | Main microcontroller                |
| MQ135      | Air quality / CO₂ estimation sensor |
| MKE-S06    | Sound sensor for noise monitoring   |
| Wi-Fi      | Wireless communication              |
| Smartphone | Remote monitoring through Blynk     |

---

## Software Requirements

* PlatformIO / Arduino IDE
* Visual Studio Code
* Blynk IoT Platform
* ESP32 Board Package


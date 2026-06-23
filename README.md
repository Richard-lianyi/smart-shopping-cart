# Smart Shopping Cart 智能购物车

A Smart Shopping Cart prototype project focused on BLE-based device detection and future hardware-software integration.

本项目是一个智能购物车原型项目，当前重点是通过 BLE Bluetooth Low Energy 实现设备扫描与信号强度检测，并为后续的购物车状态检测、传感器集成和智能结算功能打下基础。

---

## Project Overview 项目简介

The Smart Shopping Cart project aims to build a shopping cart system that can collect status data, communicate with nearby devices, and support future functions such as item tracking, automatic checkout, and smart shopping assistance.

智能购物车项目的目标是开发一个能够获取购物车状态、与附近设备通信，并支持商品追踪、自动结算和购物辅助等功能的原型系统。

---

## Current Progress 当前进展

The current version includes an Arduino BLE scanner firmware.

目前版本包含一个 Arduino BLE 扫描程序，用于检测指定 BLE UUID 的目标设备，并读取其 RSSI 信号强度。

Current firmware function:

- Scans for BLE devices with a target UUID
- Filters devices by service UUID
- Finds the strongest RSSI value in each scan cycle
- Prints the target device MAC address and RSSI value to the Serial Monitor

---

## Repository Structure 项目结构

```text
smart-shopping-cart/
├── README.md
├── LICENSE
├── .gitignore
└── firmware/
    └── ble_scanner/
        └── ble_scanner.ino
```

---

## Firmware 固件说明

The Arduino firmware is located in:

```text
firmware/ble_scanner/ble_scanner.ino
```

### BLE Scanner

The BLE scanner searches for devices using the following target UUID:

```text
0000fdaa-0000-1000-8000-00805f9b34fb
```

During each scan cycle, the firmware records the target device with the strongest RSSI value.

Example Serial Monitor output:

```text
===== SmartCart Strongest RSSI Scanner Start =====
Best target | MAC: 40:1b:7c:4f:64:92 | RSSI: -36
Target not found
```

RSSI values closer to 0 usually indicate a stronger signal.

For example:

```text
-36 dBm = stronger signal
-93 dBm = weaker signal
```

---

## Technologies 技术方向

- Arduino development
- ESP32 BLE communication
- Embedded systems
- Bluetooth Low Energy scanning
- Sensor-based cart status detection
- Hardware-software integration

---

## Planned Features 计划功能

- BLE-based cart or device detection
- RSSI-based distance/signal estimation
- Sensor-based cart status monitoring
- Item tracking
- Hardware prototype testing
- Future web or mobile interface
- Future automatic checkout support

---

## Future Work 后续计划

- Organize additional Arduino firmware tests
- Add BLE testing records
- Add hardware wiring diagrams
- Add sensor testing code
- Add system architecture documentation
- Add prototype photos or demo video
- Integrate item detection or cart status sensors

---

## Author

Richard Lianyi Chen

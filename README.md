# ⚡ Brushless Motor Monitor — Qt GUI with Custom Analog Gauges

> Real-time desktop dashboard for monitoring and controlling a brushless DC motor with Hall effect sensors. Features custom-painted analog Qt widgets (QGauge + QMeter), bidirectional serial communication at 115200 baud, and packaged as a self-contained Linux AppImage via linuxdeployqt.

[![Language: C++](https://img.shields.io/badge/Language-C%2B%2B-blue?logo=c%2B%2B)](https://en.wikipedia.org/wiki/C%2B%2B)
[![Framework: Qt](https://img.shields.io/badge/Framework-Qt-41CD52?logo=qt&logoColor=white)](https://www.qt.io/)
[![UI: QML](https://img.shields.io/badge/UI-QML%20%2B%20Widgets-41CD52?logo=qt&logoColor=white)](https://doc.qt.io/qt-6/qmlapplications.html)
[![Serial: 115200 baud](https://img.shields.io/badge/Serial-115200%20baud-orange)](https://doc.qt.io/qt-6/qserialport.html)
[![Deploy: AppImage](https://img.shields.io/badge/Deploy-AppImage-brightgreen)](https://appimage.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

---

## 📖 Table of Contents

- [About](#about)
- [Screenshots](#screenshots)
- [Features](#features)
- [Architecture](#architecture)
- [Project Structure](#project-structure)
- [Technologies Used](#technologies-used)
- [Download & Run (AppImage)](#download--run-appimage)
- [Building from Source](#building-from-source)
- [Serial Protocol](#serial-protocol)
- [Usage](#usage)

---

## About

This Qt desktop application provides a real-time GUI dashboard for a **brushless DC motor** equipped with **Hall effect sensors**. The app connects to a microcontroller over a serial port (115200 baud, 8N1), receives live motor speed data, and sends speed setpoints back to the MCU via a simple framed protocol.

The interface features two fully custom analog widgets — a **speedometer-style gauge** (`QGauge`) displaying motor speed, and an **analog meter** (`QMeter`) displaying power/load — both implemented as hand-painted C++ Qt widgets using `QPainter`, with their draw routines separated into dedicated `*drawfunctions.cpp` modules.

The application is distributed as a **self-contained Linux AppImage** built with **linuxdeployqt**, which bundles all Qt shared libraries so the app runs on any modern Linux system without requiring Qt to be installed.

---

## Screenshots


![Brushless Motor Monitor](assets/screenshot.png)

---

## Features

- **Real-time speed display** via custom analog `QGauge` widget (speedometer-style needle dial)
- **Power/load display** via custom analog `QMeter` widget
- **Serial port auto-discovery** — scans and lists all available ports at startup
- **Connect / Disconnect** toggle with live port management
- **Speed setpoint control** via slider → sends framed command `#<value>&` to MCU
- **Speed setpoint dial** → mirrors value on LCD readout
- **Serial data log** panel showing raw incoming data from the MCU
- **Self-contained AppImage** — runs on any Linux x86_64 machine without Qt installed

---

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                  Qt Application (AppImage)                  │
│                                                             │
│  ┌────────────────────────────────────────────────────┐     │
│  │  Dialog  (main window)                             │     │
│  │                                                    │     │
│  │   ┌──────────────┐       ┌────────────────────┐   │     │
│  │   │   QGauge     │       │     QMeter         │   │     │
│  │   │  (speed %)   │       │  (power / load)    │   │     │
│  │   │  custom C++  │       │  custom C++        │   │     │
│  │   │  QPainter    │       │  QPainter          │   │     │
│  │   └──────▲───────┘       └────────▲───────────┘   │     │
│  │          │ setValue()             │ setValue()     │     │
│  │   ┌──────┴────────────────────────┴────────────┐   │     │
│  │   │          QSerialPort  (115200, 8N1)        │   │     │
│  │   │  readyRead() ──▶ parse int ──▶ setValue()  │   │     │
│  │   │  write()     ◀── "#<setpoint>&"            │   │     │
│  │   └───────────────────────┬────────────────────┘   │     │
│  └───────────────────────────┼────────────────────────┘     │
└──────────────────────────────┼──────────────────────────────┘
                               │  USB / UART
┌──────────────────────────────▼──────────────────────────────┐
│                    Microcontroller (MCU)                     │
│         Brushless DC Motor + Hall Effect Sensors             │
│                                                             │
│   Sends    →  speed integer as ASCII  (e.g. "75\n")         │
│   Receives →  "#<speed_setpoint>&"                          │
└─────────────────────────────────────────────────────────────┘
```

---

## Project Structure

```
speedometer_qt/
└── interfaz potencia/
    ├── main.cpp                       # Application entry point
    ├── interfaz.pro                   # QMake project file
    ├── Gauge.qml                      # QML gauge component
    │
    ├── dialog.cpp                     # Main window: serial port, gauges,
    ├── dialog.h                       #   speed slider and setpoint sender
    ├── dialog.ui                      # Qt Designer layout
    │
    ├── mainwindow.cpp                 # Secondary window: serial debug log,
    ├── mainwindow.h                   #   speed dial, LCD readout
    ├── mainwindow.ui                  # Qt Designer layout
    │
    ├── QGauge/
    │   ├── qgauge.h
    │   ├── qgauge.cpp                 # Analog speedometer gauge widget
    │   └── qgaugedrawfunctions.cpp    # QPainter needle & arc draw routines
    │
    └── QMeter/
        ├── qmeter.h
        ├── qmeter.cpp                 # Analog power meter widget
        └── qmeterdrawfunctions.cpp    # QPainter draw routines
```

---

## Technologies Used

| Technology | Role |
|---|---|
| **C++ (88%)** | Application logic, serial handling, custom widget painting |
| **QML (9%)** | Declarative gauge component (`Gauge.qml`) |
| **QMake (2.7%)** | Build system (`interfaz.pro`) |
| **Qt Widgets** | Main window, dialog, dial, LCD display, slider, text panel |
| **Qt SerialPort** | Bidirectional serial communication with MCU at 115200 baud, 8N1 |
| **QPainter** | Custom analog needle gauge rendering in `*drawfunctions.cpp` |
| **linuxdeployqt** | AppImage packaging — bundles all Qt `.so` dependencies |

---

## Download & Run (AppImage)

The pre-built AppImage is distributed via **GitHub Releases** (too large to commit directly to the repository).

**➡ [Download the latest AppImage from the Releases page](../../releases/latest)**

Once downloaded:

```bash
# 1. Make it executable
chmod +x Brushless_Motor_Monitor-x86_64.AppImage

# 2. Run it — no Qt installation required
./Brushless_Motor_Monitor-x86_64.AppImage
```

> **Note:** AppImages run on any Linux x86_64 system with FUSE support. If you get a FUSE error, you can extract and run directly instead:
> ```bash
> ./Brushless_Motor_Monitor-x86_64.AppImage --appimage-extract
> ./squashfs-root/AppRun
> ```

---

## Building from Source

### Prerequisites

- Qt 5.x or Qt 6.x with the following modules:
  - `Qt Widgets`
  - `Qt SerialPort`
  - `Qt Quick` / `Qt QML`
- `g++` or `clang++` with C++11 support
- `qmake` in your `PATH`

### Steps

```bash
# 1. Clone the repository
git clone https://github.com/mtassone1/speedometer_qt.git
cd speedometer_qt/interfaz\ potencia

# 2. Generate the Makefile
qmake interfaz.pro

# 3. Build
make -j$(nproc)

# 4. Run
./interfaz
```

### Re-packaging as AppImage (optional)

```bash
# After building, use linuxdeployqt to bundle Qt dependencies
linuxdeployqt interfaz -appimage
```

---

## Serial Protocol

The app communicates with the MCU using a minimal framed ASCII protocol over serial (115200 baud, 8 data bits, no parity, 1 stop bit):

| Direction | Format | Description |
|---|---|---|
| MCU → App | `<integer>\n` | Motor speed as ASCII integer (0–100) |
| App → MCU | `#<value>&` | Speed setpoint byte, framed with `#` start and `&` end |

**Example:**
```
MCU sends:   "75\n"        → gauge updates to 75%
App sends:   "#50&"        → MCU sets motor speed setpoint to 50
```

The app validates incoming values (`1 < value < 100`) before updating the gauge, discarding noise or malformed frames.

---

## Usage

1. Connect the MCU to the computer via USB/UART.
2. Launch the AppImage (or the built binary).
3. Select the correct serial port from the dropdown menu.
4. Click **Conectar** — the button changes to **Desconectar** on success.
5. The `QGauge` needle will update in real time as the MCU sends speed readings.
6. Use the **speed slider** to send a setpoint to the MCU (`#<value>&`).
7. Click **Desconectar** or close the app to release the serial port.

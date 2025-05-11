# SmartFeed IoT Simulation

An ESP32-based smart bird pet feeder with real-time scheduling, food level monitoring, and remote control via Firebase and WebSocket using mobile application (Flutter).

## Features

- **Automated Feeding:** Schedule feeding times remotely via Firebase.
- **Food Level Monitoring:** Uses an ultrasonic sensor to measure and report food level.
- **Manual Feeding:** Trigger feeding remotely or via schedule.
- **OLED Display:** Shows current food level and feeder status.
- **WebSocket Integration:** Real-time updates for feeding schedule and food level threshold.
- **Configurable Threshold:** Set minimum food level threshold remotely.
- **Threshold Notification:** Notify users (mobile) when feed level drops below or equal to food level threshold.

## Hardware

- ESP32 Dev Board
- Ultrasonic Sensor (HC-SR04)
- Servo Motor
- OLED Display (SSD1306)
- RTC Module (DS1307)

## Directory Structure

```
.
├── include/           # Header files
├── lib/               # Custom libraries (optional)
├── src/               # Source files
├── test/              # Unit tests
├── platformio.ini     # PlatformIO project config
├── wokwi.toml         # Wokwi simulator config
└── README.md          # This file
```

## Getting Started

### Prerequisites

- [PlatformIO](https://platformio.org/) (VSCode recommended)
- ESP32 Dev Board

### Setup

1. **Clone the repository:**
   ```sh
   git clone https://github.com/mfazrinizar/SmartFeed-IoT-Simulation
   cd SmartFeed-IoT-Simulation
   ```

2. **Configure environment:**
   - Copy `src/Config.cpp.example` to `src/Config.cpp` and fill in your WiFi, API, and Firebase credentials.

3. **Install dependencies:**
   - PlatformIO will auto-install libraries listed in your code and `platformio.ini`.

4. **Build and upload:**
   ```sh
   pio run --target upload
   ```

5. **(Optional) Simulate in Wokwi:**
   - Open `diagram.json` and run in [Wokwi Simulator](https://wokwi.com/).

### Project Structure

- `src/main.cpp` - Main application logic
- `src/FeedSensor.cpp` / `include/FeedSensor.h` - Ultrasonic sensor logic
- `src/Feeder.cpp` / `include/Feeder.h` - Servo feeding logic
- `src/Schedule.cpp` / `include/Schedule.h` - Feeding schedule logic
- `src/Display.cpp` / `include/Display.h` - OLED display logic
- `src/Network.cpp` / `include/Network.h` - WiFi, HTTP, WebSocket logic
- `src/Config.cpp` / `include/Config.h` - Configuration

## Usage

- **Feeding Schedule:** Set via Firebase, updates in real-time via WebSocket.
- **Food Level Threshold:** Set via Firebase, device updates status display accordingly.
- **Manual Feeding:** Triggered via backend/WebSocket.

## Backend

- Node.js backend using Firebase and WebSocket (ESP doesn't support gRPC)
- Repository: [SmartFeed-IoT-NodeJS-Express](https://github.com/mfazrinizar/SmartFeed-IoT-NodeJS-Express)

## Mobile

- Flutter mobile framework using Real-time Firebase Cloud Firestore and Firebase Cloud Messaging.
- Repository: [SmartFeed-IoT-Mobile](https://github.com/mfazrinizar/SmartFeed-IoT-Mobile)

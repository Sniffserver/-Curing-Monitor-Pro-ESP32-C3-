# 🌿 Curing Monitor Pro (ESP32-C3)

> **A smart, heavily-robust, offline-first monitor for curing environments.** 
> Built natively for the **ESP32-C3**, driving an **SHT40** Precision Humidity Sensor and an ultra-compact **0.42" SSD1306 (72x40) OLED**.

---

## 🎯 Project Goals

- **Minimalist & Pixel-Perfect:** 100% native driver mapping strictly using the physical 72x40 display boundary. No hacky padding. No clip-offs. 
- **Offline-First:** Functions flawlessly in a glass jar. No Wi-Fi dependence. No apps required.
- **Battle-Tested & Robust:** Features an aggressive internal fail-safe engine to resurrect the I2C bus if static shock knocks down the sensor lines.
- **Low-Power Ready:** Groundwork laid for `<2mA` dormant deep sleep architectures.

---

## 🔌 Core Hardware & Wiring

> [!WARNING]
> **Brown-out protection:** Ensure your `3.3V` line is robust, especially when firing up the OLED pixel matrix and the SHT40 simultaneously. Solder a bypass capacitor (e.g., **`10µF`**) across 3.3V and GND if experiencing OLED UI stutter / visual glitches!

| Component | ESP32-C3 Pin | Purpose |
| :--- | :--- | :--- |
| **SHT40 Sensor** | `GPIO 5` | I2C SDA |
| **SHT40 Sensor** | `GPIO 6` | I2C SCL |
| **SHT40 Power** | `3V3` | Power (+3.3V) |
| **SHT40 Ground** | `GND` | Ground |
| **0.42" OLED** | `GPIO 5` | I2C SDA |
| **0.42" OLED** | `GPIO 6` | I2C SCL |
| **0.42" OLED Power**| `3V3` | Power (+3.3V) |
| **0.42" OLED Ground** | `GND` | Ground |

---

## 🛠 Advanced Features

### 1. Robust I2C Auto-Recovery
Total fault tolerance. If the C++ logic detects `>3` failed read attempts, it doesn't just reboot the sensor—**it completely terminates and restarts the ESP32 TwoWire hardware peripheral** to sever any frozen bus hold. If it fails `6` consecutive times, it switches the UI to `SIMULATING` fallback mode to prevent the display loop from crashing.

### 2. SHT40 Precision
Includes extreme integrity logic. Built-in **CRC-8 Hash Validation** strips out any corrupted data reads before they can hit the display.

### 3. Dynamic "Glanceable" Dashboard UI
Custom built **State-Machine Dashboard**:
- Temperature and Humidity actively spawn **Upward/Downward trend arrows**.
- Bottom zone calculates exact pixel spacing and dynamically anchors your Environment Tag (`OPTIMAL`, `DRY`, `HUMID`) perfectly into the center utilizing standard `6x12` clean fonts.

---

## 🚀 Setup & Flashing

Setup takes roughly 20 seconds using PlatformIO.

1. **Board Configuration:** Launch PlatformIO on the `esp32-c3-devkitm-1` environment.
2. **USB CDC Activation:** We added `ARDUINO_USB_MODE=1` to the `.ini`. This bypasses extra UART chips and links your Type-C cable *directly* to the physical ESP32-C3 core flawlessly. 
3. **Upload:** Unplug any physical serial UART cables. Plug standard USB-C directly into the breakout module and run:
   ```bash
   pio run -t upload
   ```

> [!TIP]
> **Calibration Drift & Care:** The SHT40 is heavily factory-calibrated. However, months of intense 60%+ curing humidity causes minor upward drift. Trust the built-in CRC checking for datalink integrity, but use a secondary hygrometer every 6–12 months to verify chemical baseline accuracy.

---

## 🔮 Future-Proofing (Next Steps)

The architecture inside `main.cpp` was fundamentally polished for the incoming phases:

* **Hardware OTA (Over-The-Air):** We explicitly architected the loop using non-blocking state mechanisms. The next milestone implies adding an External Button to wake up `WiFi` + `ElegantOTA` for 60 seconds, so you can wirelessly push patch notes to the jar without breaching the vacuum seal.
* **Deep Sleep Engine:** The structure handles `delay(10)` watchdogs safely, but explicit stubs are laid out detailing where `esp_light_sleep_start()` goes once you fully un-tether from the debugging Serial chord and hit raw battery mode.

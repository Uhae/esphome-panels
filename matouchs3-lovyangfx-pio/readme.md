# MaTouch S3 Parallel (3.5\" ILI9488) – LovyanGFX + PlatformIO Baseline

This project is a **known-good bring-up baseline** for the Makerfabs / MaTouch **ESP32-S3 Parallel 3.5\" TFT with Touch**
using **LovyanGFX** and **PlatformIO**.

✅ Status of this baseline:
- Display works (ILI9488, parallel 16-bit i8080)
- Touch works (I2C @ `0x38`, FT5x06/FT6236 family)
- Rotation: **Landscape via `setRotation(3)`** (confirmed on hardware)
- Touch mapping stays correct in landscape
- Simple test UI prints text and draws red dots where you touch

Tag / reference:
- Git tag: `matouch-lgfx-baseline-v1`

---

## Hardware

Board: ESP32-S3 (tested as `esp32-s3-devkitc-1`)  
Display: ILI9488, 320×480, 16-bit parallel (i8080 style)  
Touch: FT5x06/FT6236 compatible, I2C address `0x38`  
Backlight: PWM on **GPIO45** (confirmed working)

---

## Pin Mapping (as implemented)

### Parallel bus (16-bit)
- WR: GPIO35
- RD: GPIO48
- RS/DC: GPIO36
- CS: GPIO37

Data pins:
- D0  GPIO47
- D1  GPIO21
- D2  GPIO14
- D3  GPIO13
- D4  GPIO12
- D5  GPIO11
- D6  GPIO10
- D7  GPIO9
- D8  GPIO3
- D9  GPIO8
- D10 GPIO16
- D11 GPIO15
- D12 GPIO7
- D13 GPIO6
- D14 GPIO5
- D15 GPIO4

### Touch (I2C)
- SDA: GPIO38
- SCL: GPIO39
- INT: GPIO40
- Address: `0x38`

### Backlight (PWM)
- BL: GPIO45 (LovyanGFX `Light_PWM`)

---

## Software Layout

- `platformio.ini` – PlatformIO environment definition
- `include/LGFX_MaTouchS3_Parallel_ILI9488.hpp` – LovyanGFX device class + pinmap + touch detect
- `src/main.cpp` – Bring-up test: text + touch dots + heartbeat logs

---

## Build / Flash / Monitor

From the project folder:

```powershell
cd C:\projects\esphome\panels\matouchs3-lovyangfx-pio
```

### List serial devices (find COM port)
```powershell
pio device list
```

### Build
```powershell
pio run
```

### Upload (adjust COM port)
```powershell
pio run -t upload --upload-port COM6
```

### Serial monitor (adjust COM port)
```powershell
pio device monitor --port COM6 --baud 115200
```

---

## Rotation

This baseline uses:

```cpp
lcd.setRotation(3);  // landscape (confirmed correct on hardware)
```

If you ever need alternatives:
- `0` portrait
- `1` landscape (90°)
- `2` portrait (180°)
- `3` landscape (270°)

---

## What you should see

On boot:
- Text on black background:
  - "MaTouch S3 Parallel"
  - "LovyanGFX bring-up"
  - "Touch: tap the screen"

On touch:
- Serial log:
  - `TOUCH: x=... y=...`
- A red dot appears under your finger

---

## Troubleshooting

### Display is white / unstable / flickers
Lower the parallel write frequency in `LGFX_MaTouchS3_Parallel_ILI9488.hpp`:

```cpp
cfg.freq_write = 20000000; // 20 MHz
// or
cfg.freq_write = 10000000; // 10 MHz
```

Then rebuild + upload.

### Touch not detected
- Confirm I2C address `0x38` is present
- Check SDA/SCL/INT pins (GPIO38/39/40)

### Backlight not working
This board’s backlight is confirmed on **GPIO45** (not GPIO48).

---

## Next Steps

Good next steps after this baseline:
1. Replace the bring-up screen with a simple button grid (touch toggles buttons).
2. Add a lightweight UI loop or integrate LVGL (LovyanGFX as display backend).
3. Integrate with Home Assistant (MQTT / HTTP / custom API) depending on your architecture.

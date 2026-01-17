\# JC3248W535 Room Controller (ESPHome + LVGL)



Hardware:

\- Guition JC3248W535EN / ESP32-S3

\- Display: axs15231 (QSPI)

\- Touch: axs15231 (I2C)

\- Resolution: 480x320 (landscape), rotation 90



Features:

\- Top bar: Time + indoor temperature + humidity

\- EVCC button: mode toggle + charge power (kW + bar 0..12kW)

\- Kitchen cleaning button: input\_button.press

\- 2 generic HA switches (LVGL buttons 3 and 4)



Secrets:

Create `secrets.yaml` based on `secrets.example.yaml`.



Build:

python -m esphome compile ali-panel-01.yaml




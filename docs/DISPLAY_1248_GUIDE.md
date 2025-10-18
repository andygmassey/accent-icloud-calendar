# 12.48" Waveshare display guide

- Model: GDEY1248Z51 (1304×984)
- Driver: DESPI‑C1248

Key points
- Page rendering required; buffer per page fits ESP32 RAM
- Typical page height: 123px (HEIGHT/8)
- BUSY/DC/RST pins must match wiring; see example pin map in client/include/Display.h
- Hibernation: some adapters require skipping hibernate to avoid BUSY lock; note power impact

Known-good combo
- GxEPD2 pinned to a stable version tested with this panel
- PlatformIO environment provided in client/platformio.ini

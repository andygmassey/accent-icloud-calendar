# ESP32 client setup (12.48" e‑paper)

Hardware
- Waveshare DESPI‑C1248 + GDEY1248Z51 (1304×984 tri‑color)
- ESP32‑S3 DevKit (tested) or ESP32 variant

Configure
- Edit `client/include/Config.h`:
  - BASE_URL (e.g., https://your-host.example.com)
  - DEVICE_KEY (string you’ll pass to /epd and /next)
- Optional: adjust pins if your wiring differs (see DISPLAY_1248_GUIDE.md)

Build and flash
```bash
cd client
python3 -m platformio run --target upload
```

Troubleshooting
- If display shows “Error”, open serial monitor and confirm BASE_URL resolves and returns HTTP 200 for /next and /epd.
- Memory: rendering uses page mode; default settings are tuned for 12.48".

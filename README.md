# accent-icloud-calendar

[![server](https://github.com/andygmassey/accent-icloud-calendar/actions/workflows/server.yml/badge.svg)](https://github.com/andygmassey/accent-icloud-calendar/actions/workflows/server.yml)
[![client](https://github.com/andygmassey/accent-icloud-calendar/actions/workflows/client.yml/badge.svg)](https://github.com/andygmassey/accent-icloud-calendar/actions/workflows/client.yml)

A clean, reusable fork of the Accent e‑paper display concept with:
- iCloud Calendar via CalDAV (no Google OAuth)
- ESP32 client for 12.48" Waveshare e‑paper (GDEY1248Z51 + DESPI‑C1248)
- Portable server (run locally, on Raspberry Pi, in Docker, or in the cloud)

Quick start
- Server: see docs/SETUP_SERVER.md
- Client (ESP32): see docs/SETUP_CLIENT.md

Where to run the server
- Local (Python): easiest for development; set env vars and run Flask/gunicorn
- Docker (any host): consistent runtime; compose file provided
- Raspberry Pi: systemd service; minimal footprint
- Cloud (Cloud Run/App Engine/VPS): public URL, managed TLS/uptime
Notes: Only iCloud CalDAV username and app‑specific password are required. No Google dependencies.

Wiring summary (12.48" with DESPI‑C1248)
- SPI: SCK=13, MISO=12, MOSI=14, CS=15
- DESPI‑C1248 lines (to ESP32):
  - CS M1=23, CS S1=22, CS M2=16, CS S2=19
  - DC1=25, DC2=17, RST1=33, RST2=5
  - BUSY M1=32, BUSY S1=26, BUSY M2=18, BUSY S2=4
See docs/DISPLAY_1248_GUIDE.md for details.

Features
- iCloud CalDAV calendar discovery and event rendering
- E‑paper friendly images for 12.48" display
- /health endpoint and optional watchdog integration
- Zero secrets in repo; .env.example provided

License and attribution
- Based on Max Braun’s Accent (https://github.com/maxbbraun/accent)
- This repo re‑implements calendar using iCloud CalDAV and documents 12.48" display specifics

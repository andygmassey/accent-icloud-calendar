# accent-icloud-calendar

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

Features
- iCloud CalDAV calendar discovery and event rendering
- E‑paper friendly images for 12.48" display
- /health endpoint and optional watchdog integration
- Zero secrets in repo; .env.example provided

License and attribution
- Based on Max Braun’s Accent (https://github.com/maxbbraun/accent)
- This repo re‑implements calendar using iCloud CalDAV and documents 12.48" display specifics

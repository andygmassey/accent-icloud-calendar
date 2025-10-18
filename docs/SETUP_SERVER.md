# Server setup (iCloud CalDAV)

Requirements
- Python 3.10+
- iCloud account (username) and app-specific password

Environment variables (.env.example)
- ICLOUD_USERNAME=your_icloud_username
- ICLOUD_APP_PASSWORD=your_app_password
- ACCENT_DEVICE_KEY=example-device-key
- ACCENT_DEFAULT_WIDTH=1304
- ACCENT_DEFAULT_HEIGHT=984

Run locally (dev)
```bash
python3 -m venv .venv && source .venv/bin/activate
pip install -r server/requirements.txt
cp .env.example .env  # fill values
python server/main.py
```

Docker
```bash
docker compose up --build
```

Raspberry Pi (systemd)
- Copy repo to the Pi
- Create a venv and install requirements
- Create a user systemd unit launching `python server/main.py`

Cloud (e.g., Cloud Run)
- Build a container from Dockerfile
- Set env vars in the service
- Expose HTTPS endpoint

Endpoints
- GET /health → 200 OK {"status":"ok","version":"v0"}
- GET /epd?key=DEVICE_KEY&width=1304&height=984&variant=bwr → binary image
- GET /next?key=DEVICE_KEY → milliseconds until next refresh

Notes
- No Google dependencies required
- Credentials are read from env; do not commit secrets

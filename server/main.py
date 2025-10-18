from flask import Flask, request, jsonify, Response
import os
from io import BytesIO
from datetime import datetime
from calendar import Calendar, SUNDAY
from collections import Counter
from PIL import Image, ImageDraw, ImageFont

# Lazy import of CalDAV client (optional real integration)
try:
    from icloud_caldav import ICloudCalDAV
except Exception:
    ICloudCalDAV = None

app = Flask(__name__)

VERSION = "v0.1.0"

DEVICE_KEY = os.environ.get("ACCENT_DEVICE_KEY", "demo-key")
DEFAULT_WIDTH = int(os.environ.get("ACCENT_DEFAULT_WIDTH", 1304))
DEFAULT_HEIGHT = int(os.environ.get("ACCENT_DEFAULT_HEIGHT", 984))
ICLOUD_USERNAME = os.environ.get("ICLOUD_USERNAME")
ICLOUD_APP_PASSWORD = os.environ.get("ICLOUD_APP_PASSWORD")

_cal = None
if ICLOUD_USERNAME and ICLOUD_APP_PASSWORD and ICloudCalDAV:
    _cal = ICloudCalDAV(ICLOUD_USERNAME, ICLOUD_APP_PASSWORD)
    if not _cal.connect():
        _cal = None

@app.get("/health")
def health():
    return jsonify({"status": "ok", "version": VERSION})

@app.get("/next")
def next_ms():
    if request.args.get("key") != DEVICE_KEY:
        return Response("unauthorized", status=401)
    # Simple default: 10 minutes
    return str(10 * 60 * 1000)

@app.get("/epd")
def epd():
    if request.args.get("key") != DEVICE_KEY:
        return Response("unauthorized", status=401)
    width = int(request.args.get("width", DEFAULT_WIDTH))
    height = int(request.args.get("height", DEFAULT_HEIGHT))

    # Render a simple monthly calendar with dots for event counts
    now = datetime.now()
    cal = Calendar(firstweekday=SUNDAY)
    weeks = cal.monthdayscalendar(now.year, now.month)

    # Get event counts per day from iCloud if configured, else synthetic
    counts: Counter
    if _cal:
        # discover names and count events
        names = list(_cal.discover_calendars().keys())
        counts = _cal.get_event_counts_by_day(names, now.year, now.month)
    else:
        counts = Counter({d: (d % 3) for d in range(1, 29)})

    img = Image.new("RGB", (width, height), (255, 255, 255))
    draw = ImageDraw.Draw(img)
    try:
        font = ImageFont.load_default()
    except Exception:
        font = None

    x_stride = width // (7 + 1)
    y_stride = height // (len(weeks) + 1)

    # Title
    title = f"{now.strftime('%B %Y')}"
    draw.text((10, 5), title, fill=(0, 0, 0), font=font)

    for wy, week in enumerate(weeks):
        for dx, day in enumerate(week):
            if day == 0:
                continue
            x = (dx + 1) * x_stride
            y = (wy + 1) * y_stride
            # day number
            draw.text((x, y), str(day), fill=(0, 0, 0), font=font, anchor="mm")
            # dots for events
            n = counts.get(day, 0)
            for i in range(min(3, n)):
                draw.ellipse((x - 6 + i*6, y + 10, x - 2 + i*6, y + 14), fill=(0, 0, 0))

    bio = BytesIO()
    img.save(bio, format="PNG")
    bio.seek(0)
    return Response(bio.getvalue(), mimetype="application/octet-stream")

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8080, debug=False)

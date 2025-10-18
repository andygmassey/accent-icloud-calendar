from flask import Flask, request, jsonify, Response
import os
from io import BytesIO
from PIL import Image

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

    # Placeholder: render simple calendar/title until CalDAV configured
    img = Image.new("RGB", (width, height), (255, 255, 255))
    # Minimal black band as proof of life
    for y in range(0, min(40, height)):
        for x in range(width):
            img.putpixel((x, y), (0, 0, 0))

    bio = BytesIO()
    img.save(bio, format="PNG")
    bio.seek(0)
    return Response(bio.getvalue(), mimetype="application/octet-stream")

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8080, debug=False)

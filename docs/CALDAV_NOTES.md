# iCloud CalDAV notes

- Create an app-specific password in your Apple ID (Security → App Passwords)
- Use ICLOUD_USERNAME (your Apple ID email) and ICLOUD_APP_PASSWORD as env vars
- Server will connect to https://caldav.icloud.com/ and auto-discover calendars
- Troubleshooting:
  - If /epd renders demo dots, credentials are missing or invalid
  - Check logs: server should report calendars discovered and event counts
  - Timezones: this minimal implementation counts by DTSTART day (all-day events supported)

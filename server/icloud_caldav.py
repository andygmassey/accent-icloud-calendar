import os
from collections import Counter
from datetime import datetime, timedelta
from typing import Dict, List

try:
    import caldav
except Exception:  # pragma: no cover
    caldav = None

class ICloudCalDAV:
    """Minimal CalDAV client for iCloud to fetch monthly event counts."""

    def __init__(self, username: str, password: str):
        self.username = username
        self.password = password
        self._client = None
        self._principal = None

    def connect(self) -> bool:
        if not caldav:
            return False
        try:
            self._client = caldav.DAVClient(url="https://caldav.icloud.com/", username=self.username, password=self.password)
            self._principal = self._client.principal()
            # touch calendars to verify access
            _ = self._principal.calendars()
            return True
        except Exception:
            return False

    def discover_calendars(self) -> Dict[str, object]:
        """Return mapping of calendar displayname -> calendar object."""
        result = {}
        if not self._principal:
            return result
        for cal in self._principal.calendars():
            try:
                name = cal.get_properties([caldav.dav.DisplayName()])[caldav.dav.DisplayName.tag]
            except Exception:
                # fallback
                name = getattr(cal, 'url', None) or 'Calendar'
            result[str(name)] = cal
        return result

    def get_event_counts_by_day(self, calendar_names: List[str], year: int, month: int) -> Counter:
        counts: Counter = Counter()
        if not self._principal:
            return counts
        start = datetime(year, month, 1)
        # compute first day of next month
        if month == 12:
            end = datetime(year + 1, 1, 1)
        else:
            end = datetime(year, month + 1, 1)
        cals = self.discover_calendars()
        selected = [cals[n] for n in calendar_names if n in cals]
        if not selected:
            selected = list(cals.values())
        for cal in selected:
            try:
                events = cal.date_search(start, end)
                for ev in events:
                    try:
                        v = ev.vobject_instance
                        dtstart = None
                        # prefer DTSTART
                        if hasattr(v, 'vevent') and hasattr(v.vevent, 'dtstart'):
                            ds = v.vevent.dtstart.value
                            if isinstance(ds, datetime):
                                dtstart = ds
                            else:  # date
                                dtstart = datetime(ds.year, ds.month, ds.day)
                        if not dtstart:
                            continue
                        if dtstart.year == year and dtstart.month == month:
                            counts[dtstart.day] += 1
                    except Exception:
                        continue
            except Exception:
                continue
        return counts

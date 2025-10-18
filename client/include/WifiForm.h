#ifndef WIFI_FORM_H
#define WIFI_FORM_H

#include <Arduino.h>

const String kWifiForm =
    "<!DOCTYPE html>\n"
    "<html>\n"
    "<head><title>Accent Wifi Setup</title></head>\n"
    "<body>\n"
    "<form method=\"post\" action=\"/save\">\n"
    "SSID: <input type=\"text\" name=\"ssid\" size=\"20\"><br/>\n"
    "Password: <input type=\"password\" name=\"password\" size=\"20\"><br/>\n"
    "<input type=\"submit\" value=\"Save\" />\n"
    "</form>\n"
    "</body>\n"
    "</html>\n";

#endif  // WIFI_FORM_H
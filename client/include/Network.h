#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include "Display.h"
#include "Power.h"

class Network {
 public:
  Network(uint32_t serial_speed)
      : display_(serial_speed), power_(), wifi_setup_server_(nullptr) {}
  ~Network() {
    if (wifi_setup_server_ != nullptr) {
      delete wifi_setup_server_;
    }
  }

  bool ConnectWifi();
  bool HttpGet(HTTPClient* http, const String& url);
  bool HttpGet(HTTPClient* http, const String& base_url,
               const std::vector<String>& parameters);
  void ResetWifi();
  bool StartWifiSetupServer();
  bool HandleWifiSetupServer();

 private:
  void ShowWifiForm();
  void SaveWifiForm();
  void SendNotFound();
  void AddAuthHeader(HTTPClient* http);

  Display display_;
  Power power_;
  WebServer* wifi_setup_server_;
};

#endif  // NETWORK_H

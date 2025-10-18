#include "Network.h"

#include <Preferences.h>
#include <WiFi.h>
#include <base64.h>
#include "WifiForm.h"

const char* kRootCertificate =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDdTCCAl2gAwIBAgILBAAAAAABFUtaw5QwDQYJKoZIhvcNAQEFBQAwVzELMAkG\n"
    "A1UEBhMCQkUxGTAXBgNVBAoTEEdsb2JhbFNpZ24gbnYtc2ExEDAOBgNVBAsTB1Jv\n"
    "b3QgQ0ExGzAZBgNVBAMTEkdsb2JhbFNpZ24gUm9vdCBDQTAeFw05ODA5MDExMjAw\n"
    "MDBaFw0yODAxMjgxMjAwMDBaMFcxCzAJBgNVBAYTAkJFMRkwFwYDVQQKExBHbG9i\n"
    "YWxTaWduIG52LXNhMRAwDgYDVQQLEwdSb290IENBMRswGQYDVQQDExJHbG9iYWxT\n"
    "aWduIFJvb3QgQ0EwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDaDuaZ\n"
    "jc6j40+Kfvvxi4Mla+pIH/EqsLmVEQS98GPR4mdmzxzdzxtIK+6NiY6arymAZavp\n"
    "xy0Sy6scTHAHoT0KMM0VjU/43dSMUBUc71DuxC73/OlS8pF94G3VNTCOXkNz8kHp\n"
    "1Wrjsok6Vjk4bwY8iGlbKk3Fp1S4bInMm/k8yuX9ifUSPJJ4ltbcdG6TRGHRjcdG\n"
    "snUOhugZitVtbNV4FpWi6cgKOOvyJBNPc1STE4U6G7weNLWLBYy5d4ux2x8gkasJ\n"
    "U26Qzns3dLlwR5EiUWMWea6xrkEmCMgZK9FGqkjWZCrXgzT/LCrBbBlDSgeF59N8\n"
    "9iFo7+ryUp9/k5DPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNVHRMBAf8E\n"
    "BTADAQH/MB0GA1UdDgQWBBRge2YaRQ2XyolQL30EzTSo//z9SzANBgkqhkiG9w0B\n"
    "AQUFAAOCAQEA1nPnfE920I2/7LqivjTFKDK1fPxsnCwrvQmeU79rXqoRSLblCKOz\n"
    "yj1hTdNGCbM+w6DjY1Ub8rrvrTnhQ7k4o+YviiY776BQVvnGCv04zcQLcFGUl5gE\n"
    "38NflNUVyRRBnMRddWQVDf9VMOyGj/8N7yy5Y0b2qvzfvGn9LhJIZJrglfCm7ymP\n"
    "AbEVtQwdpf5pLGkkeB6zpxxxYu7KyJesF12KwvhHhm4qxFYxldBniYUr+WymXUad\n"
    "DKqC5JlR3XC321Y9YeRq4VzW9v493kHMB65jUr9TU/Qr6cf9tveCX4XSQRjbgbME\n"
    "HMUfpIBvFSDJ3gyICh3WZlXi/EjJKSZp4A==\n"
    "-----END CERTIFICATE-----\n";

const char* kWifiPreferences = "wifi";
const char* kWifiSsidKey = "ssid";
const char* kWifiPasswordKey = "password";
const char* kSetupSsid = "AccentSetup";
const IPAddress kSetupIp(1, 2, 3, 4);
const IPAddress kSetupSubnet(255, 255, 255, 0);
const String kShowWifiFormUrl = "/go";
const String kSaveWifiFormUrl = "/save";
const uint32_t kConnectTimeoutMs = 30 * 1000;
const uint32_t kConnectTimeoutStepMs = 500;
const uint16_t kReadTimeoutMs = 30 * 1000;

bool Network::ConnectWifi() {
  if (WiFi.isConnected()) {
    Serial.println("Already connected");
    return true;
  }
  Preferences preferences;
  preferences.begin(kWifiPreferences, true);
  String ssid = preferences.getString(kWifiSsidKey, "");
  if (ssid.length() == 0) {
    Serial.println("No Wifi credentials");
    return false;
  }
  String password = preferences.getString(kWifiPasswordKey, "");
  preferences.end();
  Serial.printf("Connecting to \"%s\" .", ssid.c_str());
  WiFi.mode(WIFI_STA);
  if (password.length() > 0) WiFi.begin(ssid.c_str(), password.c_str());
  else WiFi.begin(ssid.c_str());
  for (int i = 0; !WiFi.isConnected(); ++i) {
    if (i > kConnectTimeoutMs / kConnectTimeoutStepMs) {
      Serial.println("\nTimed out connecting");
      display_.ShowError();
      power_.Restart();
    }
    delay(kConnectTimeoutStepMs);
    Serial.print(".");
  }
  Serial.printf("\nConnected to %s as %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
  return true;
}

bool Network::HttpGet(HTTPClient* http, const String& url) { return HttpGet(http, url, {}); }

bool Network::HttpGet(HTTPClient* http, const String& base_url, const std::vector<String>& parameters) {
  if (parameters.size() % 2 != 0) {
    Serial.printf("Incomplete pairs of keys and values for URL: %s\n", base_url.c_str());
    return false;
  }
  String url = base_url;
  for (int i = 0; i < parameters.size(); i += 2) {
    String delimiter = (i == 0 ? "?" : "&");
    url += delimiter + parameters[i] + "=" + parameters[i + 1];
  }
  Serial.printf("Requesting URL: %s\n", url.c_str());
  if (!http->begin(url, kRootCertificate)) {
    Serial.printf("Failed to connect to server: %s\n", url.c_str());
    return false;
  }
  http->setTimeout(kReadTimeoutMs);
  AddAuthHeader(http);
  int status = http->GET();
  if (status <= 0) {
    Serial.printf("Request failed: %s\n", http->errorToString(status).c_str());
    http->end();
    return false;
  }
  Serial.printf("Status code: %d\n", status);
  if (status != HTTP_CODE_OK) {
    http->end();
    return false;
  }
  return true;
}

void Network::ResetWifi() {
  Serial.println("Resetting Wifi credentials");
  Preferences preferences;
  preferences.begin(kWifiPreferences, false);
  preferences.putString(kWifiSsidKey, "");
  preferences.putString(kWifiPasswordKey, "");
  preferences.end();
}

bool Network::StartWifiSetupServer() {
  Serial.println("Starting Wifi setup");
  WiFi.mode(WIFI_AP);
  if (!WiFi.softAP(kSetupSsid)) {
    Serial.println("Failed to start access point");
    wifi_setup_server_ = nullptr;
    return false;
  }
  delay(100);
  if (!WiFi.softAPConfig(kSetupIp, kSetupIp, kSetupSubnet)) {
    Serial.println("Failed to apply access point config");
    wifi_setup_server_ = nullptr;
    return false;
  }
  IPAddress ip = WiFi.softAPIP();
  Serial.printf("Access point \"%s\" started at %s\n", kSetupSsid, ip.toString().c_str());
  wifi_setup_server_ = new WebServer(80);
  wifi_setup_server_->on(kShowWifiFormUrl, std::bind(&Network::ShowWifiForm, this));
  wifi_setup_server_->on(kSaveWifiFormUrl, std::bind(&Network::SaveWifiForm, this));
  wifi_setup_server_->onNotFound(std::bind(&Network::SendNotFound, this));
  wifi_setup_server_->begin();
  return true;
}

bool Network::HandleWifiSetupServer() {
  if (wifi_setup_server_ == nullptr) return false;
  wifi_setup_server_->handleClient();
  return true;
}

void Network::ShowWifiForm() { wifi_setup_server_->send(200, "text/html", kWifiForm); }

void Network::SaveWifiForm() {
  String ssid = wifi_setup_server_->arg("ssid");
  String password = wifi_setup_server_->arg("password");
  Serial.printf("Saving credentials: %s %s\n", ssid.c_str(), (password.length() ? "******" : ""));
  Preferences preferences;
  preferences.begin(kWifiPreferences, false);
  preferences.putString(kWifiSsidKey, ssid);
  preferences.putString(kWifiPasswordKey, password);
  preferences.end();
  wifi_setup_server_->sendHeader("Location", kShowWifiFormUrl);
  wifi_setup_server_->send(302, "text/plain", "Saved");
}

void Network::SendNotFound() {
  wifi_setup_server_->send(404, "text/plain", "Not Found");
}

void Network::AddAuthHeader(HTTPClient* http) {
  // No auth by default; you can add Basic auth here if you enable it server-side
}

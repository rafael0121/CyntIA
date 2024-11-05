#ifndef WIFI_HELPER_H
#define WIFI_HELPER_H

#include <WiFi.h>

class WiFiHelper {
public:
    void connect(const String& ssid, const String& password);
};

#endif // WIFI_HELPER_H

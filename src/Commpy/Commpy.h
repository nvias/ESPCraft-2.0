// Commpy - communication provider
// Created by nvias - Jiří Švihla, 2021
// Licensed under MIT License

#ifndef Commpy_h
#define Commpy_h

#define DEFAULT_WIFI_AP_SSID    "espblock"      // Name of the block's WiFi in AP mode
#define DEFAULT_WIFI_AP_PASS    ""              // Password of the block's WiFi in AP mode (nothing = open network)

#define WIFI_SSID   "espcraft"                  // Default Wifi SSID to connect
#define WIFI_PASS   "nvias"                     // Default WiFi PASS to connect

#define MQTT_SERVER "broker.hivemq.com"         // Default MQTT server ip
#define MQTT_PORT   1883                        // Default MQTT server port

#include <Arduino.h>
#include <Preferences.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <ESPmDNS.h>

class Commpy
{
    public:
        Commpy(void);
        void coldBoot();
        void init();
        void reconnect();
        void send(int channel, String message);
    private:
    
};

#endif
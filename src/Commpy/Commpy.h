// Commpy - communication provider
// Created by nvias - Jiří Švihla, 2021
// Licensed under MIT License

#ifndef Commpy_h
#define Commpy_h

#define BOARD_TAG   "Bol"                       // Board UUID (for now)

#define DEFAULT_WIFI_AP_SSID    "espblock"      // Name of the block's WiFi in AP mode
#define DEFAULT_WIFI_AP_PASS    ""              // Password of the block's WiFi in AP mode (nothing = open network)

#define WIFI_SSID   "espcraft"                  // Default Wifi SSID to connect
#define WIFI_PASS   "nvias-esp"                 // Default WiFi PASS to connect

#define MQTT_SERVER "broker.hivemq.com"         // Default MQTT server ip
#define MQTT_PORT   1883                        // Default MQTT server port

//===== BOOT CODES =============================================================================
#define COMMPY_OK             0xFFFFFFE7        // ##---OO---##
#define COMMPY_FAIL           0xFFFFFF81        // ##-OOOOOO-##
//==============================================================================================

#include <Arduino.h>
#include <Preferences.h>
#include <PubSubClient.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <ESPmDNS.h>

class Commpy
{
    public:
        Commpy(void);
        void coldBoot();
        int init();
        void reconnect();
        void send(int channel, int data);
        void startPolling();
        int get(int channel);

    private:
    
};

#endif
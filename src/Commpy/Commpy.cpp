#include "Commpy.h"

String ssid = WIFI_SSID;
String password = WIFI_PASS;
String mqtt_server = MQTT_SERVER;
const int  mqtt_port = MQTT_PORT;

WiFiClient espClient;
PubSubClient client(espClient);
Preferences commpy_settings;
//=================================================================================================================================
Commpy::Commpy(void)
{

}
//=================================================================================================================================
void Commpy::coldBoot(void)
{
    commpy_settings.begin("commpy-pref", false);
    commpy_settings.putString("ssid", ssid);
    commpy_settings.putString("pass", password);
    commpy_settings.putString("mqtt-server", mqtt_server);
    commpy_settings.putInt("mqtt-port", mqtt_port);
    commpy_settings.end();
}
//=================================================================================================================================
void Commpy::init(void)
{
    commpy_settings.begin("commpy-pref", true);
    ssid = commpy_settings.getString("ssid", ssid);
    password = commpy_settings.getString("pass", password);
    WiFi.begin(ssid.c_str(), password.c_str());
    
}
//=================================================================================================================================
void Commpy::reconnect(void)
{

}

//=================  CALLBACK FROM MQTT SERVER  =================
void callback(char *topic, byte *payload, unsigned int length)
{

}
//===============================================================
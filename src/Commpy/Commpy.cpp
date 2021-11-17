#include "Commpy.h"

WiFiClient espClient;
PubSubClient client(espClient);
Preferences commpy_settings;
QueueArray<String> queue;

//=================  CALLBACK FROM MQTT SERVER  =================
void callback(char *topic, byte *payload, unsigned int length)
{
    queue.push(String(*topic) + " " + String(*payload));
}
//===============================================================

//=================================================================================================================================
Commpy::Commpy(void)
{
    _mqtt_server = MQTT_SERVER;
    _mqtt_port = MQTT_PORT;
}
//=================================================================================================================================
void Commpy::coldBoot(void)
{
    commpy_settings.begin("commpy-pref", false);
    commpy_settings.putString("ssid", WIFI_SSID);
    commpy_settings.putString("pass", WIFI_PASS);
    commpy_settings.putString("mqtt-server", MQTT_SERVER);
    commpy_settings.putUInt("mqtt-port", MQTT_PORT);
    commpy_settings.putString("board-name", BOARD_TAG);
    commpy_settings.end();
}
//=================================================================================================================================
void Commpy::reconnect(String name)
{
    if (!client.connected())
    {
        Serial.println("[MQTT Client] Attempting connection to server...");
        String clientId = name + "-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (client.connect(clientId.c_str()))
        {
            Serial.println("[MQTT Client] Connected as " + clientId);
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
        }
    }
}
//=================================================================================================================================

int Commpy::init(void)
{
    WiFi.mode(WIFI_STA);
    String ssid = "";
    String password = "";
    commpy_settings.begin("commpy-pref", true);
    ssid = commpy_settings.getString("ssid", WIFI_SSID);
    password = commpy_settings.getString("pass", WIFI_PASS);
    _mqtt_server = commpy_settings.getString("mqtt-server", MQTT_SERVER);
    _mqtt_port = commpy_settings.getUInt("mqtt-port", MQTT_PORT);
    String uuid = commpy_settings.getString("board-name", "espblock");

    if (WiFi.status() != WL_CONNECTED)
    {
        WiFi.begin(ssid.c_str(), password.c_str());
    }
    WiFi.waitForConnectResult();

    client.setServer(MQTT_SERVER, MQTT_PORT);   // (char*) &_mqtt_server doesnt work
    client.setCallback(callback);
    reconnect(uuid);

    if ((WiFi.status() == WL_CONNECTED) && (MDNS.begin("espcraft")))
    {
        return COMMPY_OK;
    }
    else
    {
        WiFi.softAP(DEFAULT_WIFI_AP_SSID, DEFAULT_WIFI_AP_PASS);
        return COMMPY_FAIL;
    }
}
//=================================================================================================================================

void Commpy::sendMessage(String channel, String data)
{
}
//=================================================================================================================================

String Commpy::getMessage()
{
    return queue.pop();
}

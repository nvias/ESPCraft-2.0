#include "Commpy.h"

WiFiClient espClient;
PubSubClient client(espClient);
Preferences commpy_settings;

typedef struct commObj
{
    String uuid;
    String bdin;
    String mcin;
    String ch01;
    String ch02;
    String ch03;
    String ch04;
    String ch05;
    String ch06;
    String ch07;
}   commObj;
commObj comm;

//=================================================================================================================================
Commpy::Commpy(void)
{

}
//=================================================================================================================================
void Commpy::coldBoot(void)
{
    commpy_settings.begin("commpy-pref", false);
    commpy_settings.putString("ssid", WIFI_SSID);
    commpy_settings.putString("pass", WIFI_PASS);
    commpy_settings.putString("mqtt-server", MQTT_SERVER);
    commpy_settings.putInt("mqtt-port", MQTT_PORT);
    commpy_settings.putString("board-name",BOARD_TAG);
    commpy_settings.end();
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
    comm.uuid = commpy_settings.getString("board-name", "espblock");

    if(WiFi.status() != WL_CONNECTED)
    {
        WiFi.begin(ssid.c_str(), password.c_str());
    }
    WiFi.waitForConnectResult();
    if(WiFi.status() == WL_CONNECTED)
    {
        return COMMPY_OK;
    }
    else
    {
        return COMMPY_FAIL;
    }
}
//=================================================================================================================================
void Commpy::reconnect(void)
{

}

void Commpy::write(int channel, String data)
{

}

void Commpy::read(int channel, String data)
{

}

void Commpy::send()
{

}

void Commpy::get()
{
    HTTPClient http;

        Serial.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://www.howsmyssl.com/a/check", ca); //HTTPS
        String addr = "http://iot.e-iot.cz/mc/mcout.php?devID=board&uuid=";
        addr += comm.uuid;
        http.addHeader("Connection", "keep-alive");
        http.addHeader("Keep-Alive", "timeout=5, max=1000");
        http.addHeader("Content-Type", "text/html");
        http.begin(addr.c_str()); //HTTP
        // start connection and send HTTP header
        for (int b = 0; b < 50; b++)
        {
            int httpCode = http.GET();

            String payload;
            // httpCode will be negative on error
            if(httpCode > 0) {
                // HTTP header has been send and Server response header has been handled
                Serial.printf("[HTTP] GET... code: %d\n", httpCode);

                // file found at server
                if(httpCode == HTTP_CODE_OK) {
                    payload = http.getString();
                }
            } else {
                Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
            }
            for(unsigned int i = payload.length(); i > 0; i--)
            {
                if(payload.charAt(i) == '}')
                {
                    for (unsigned int x = i; x > 0; x--)
                    {
                        if(payload.charAt(x) == '{')
                        {
                            payload = payload.substring(x,i+1);
                            break;
                        }
                    }
                    break;
                }
            }
            Serial.println(payload);
            delay(500);
        }
        

        http.end();
}

//=================  CALLBACK FROM MQTT SERVER  =================
void callback(char *topic, byte *payload, unsigned int length)
{

}
//===============================================================


#include "Commpy.h"

WiFiClient espClient;
PubSubClient client(espClient);
Preferences commpy_settings;

typedef struct commObj
{
    String uuid;
    String bdin;
    String mcin;
    String ch00;
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

void Commpy::send(int channel, int data)
{
    switch (channel)
    {
    case 0:
        comm.ch00 = String(data);
        break;
    case 1:
        comm.ch01 = String(data);
        break;
    case 2:
        comm.ch02 = String(data);
        break;
    case 3:
        comm.ch03 = String(data);
        break;
    case 4:
        comm.ch04 = String(data);
        break;
    case 5:
        comm.ch05 = String(data);
        break;
    case 6:
        comm.ch06 = String(data);
        break;
    case 7:
        comm.ch07 = String(data);
        break;
    
    default:
        break;
    }
    HTTPClient http;
    String addr = "http://iot.e-iot.cz/mc/mcin.php?devID=board&uuid=";
    addr = addr + comm.uuid + "&ch00=" + comm.ch00 + "&ch01=" + comm.ch01;
    addr = addr + "&MC=" + comm.mcin + "&ch02=" + comm.ch02 + "&ch03=" + comm.ch03;
    addr = addr + "&BD=" + comm.bdin + "&ch04=" + comm.ch04 + "&ch05=" + comm.ch05;
    addr = addr + "&ch06=" + comm.ch06 + "&ch07=" + comm.ch07;
    
    http.addHeader("Content-Type", "text/html");
    http.addHeader("Host", "iot.e-iot.cz");
    http.addHeader("Upgrade-Insecure-Requests", "1");
    http.addHeader("Cache-Control", "max-age=0");
    Serial.print("[HTTP] GET ");
    Serial.println(addr);
    http.begin(addr.c_str()); //HTTP
    int httpCode = http.GET();
            if(httpCode > 0) {
                // HTTP header has been send and Server response header has been handled
                Serial.printf("[HTTP] GET SEND... code: %d\n", httpCode);
            } else {
                Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
            }
    http.end();
}

void server_relay(void *parameter);    // function prototype
void Commpy::startPolling()
{
    xTaskCreate(
    &server_relay,      // Function that should be called
    "Server polling",   // Name of the task (for debugging)
    16384,              // Stack size (bytes)
    NULL,               // Parameter to pass
    1,                  // Task priority
    NULL                // Task handle
  );
}

void server_relay(void *parameter)
{
    StaticJsonDocument<200> msg;
    while(1)
    {
        HTTPClient http;
        Serial.print("[HTTP] begin...\n");
        // configure traged server and url
        String addr = "http://iot.e-iot.cz/mc/mcout.php?devID=board&uuid=";
        addr += comm.uuid;
        http.addHeader("Connection", "keep-alive");
        http.addHeader("Keep-Alive", "timeout=5, max=1000");
        http.addHeader("Content-Type", "text/html");
        http.begin(addr.c_str()); //HTTP
        // start connection and send HTTP header
        for (int b = 0; b < 1000; b++)
        {
            int httpCode = http.GET();

            String payload;
            // httpCode will be negative on error
            if(httpCode > 0) {
                // HTTP header has been send and Server response header has been handled
                Serial.printf("[HTTP] GET RECEIVE... code: %d\n", httpCode);

                // file found at server
                if(httpCode == HTTP_CODE_OK) {
                    payload = http.getString();
                }
            } else {
                Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
                break;
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
            deserializeJson(msg, payload);
            JsonObject obj = msg.as<JsonObject>();
            comm.bdin = obj["BD"].as<String>();
            comm.mcin = obj["MC"].as<String>();
            comm.ch00 = obj["ch00"].as<String>();
            comm.ch01 = obj["ch01"].as<String>();
            comm.ch02 = obj["ch02"].as<String>();
            comm.ch03 = obj["ch03"].as<String>();
            comm.ch04 = obj["ch04"].as<String>();
            comm.ch05 = obj["ch05"].as<String>();
            comm.ch06 = obj["ch06"].as<String>();
            comm.ch07 = obj["ch07"].as<String>();
            vTaskDelay(500 / portTICK_PERIOD_MS);

        }
        

        http.end();
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

int Commpy::get(int channel)
{
    switch (channel)
    {
    case 0:
        return comm.ch00.toInt();
        break;
    case 1:
        return comm.ch01.toInt();
        break;
    case 2:
        return comm.ch02.toInt();
        break;
    case 3:
        return comm.ch03.toInt();
        break;
    case 4:
        return comm.ch04.toInt();
        break;
    case 5:
        return comm.ch05.toInt();
        break;
    case 6:
        return comm.ch06.toInt();
        break;
    case 7:
        return comm.ch07.toInt();
        break;
    
    default:
        return 0;
        break;
    }
}


//=================  CALLBACK FROM MQTT SERVER  =================
void callback(char *topic, byte *payload, unsigned int length)
{

}
//===============================================================


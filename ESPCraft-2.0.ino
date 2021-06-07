#include <WiFi.h>
#include <ESPmDNS.h>
#include <PubSubClient.h>
#include <FastLED.h>
#include <Wire.h>
#include <SparkFun_STUSB4500.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <SPIFFS.h>
#include <HTTPClient.h>
#include "settings.h"
#include "version.h"
#include "flash_alloc.h"

const char *ssid = "";
const char *password = "";
const char *mqtt_server = MQTT_SERVER;

WiFiClient espClient;
PubSubClient client(espClient);
AsyncWebServer server(80);
String msg = "               ";
bool lastLedState = 0;
char uuid[15] = "";

const char wifiSet[] PROGMEM =
    "<!DOCTYPE html>"
    "\n<html>"
    "\n<title>ESP Craft OS 2.0 fail-safe GUI</title>"
    "\n<meta charset=\"UTF-8\">"
    "\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
    "\n"
    "\n<body>"
    "\n  <br>"
    "\n  <h1> ESP Craft OS 2.0 WiFi fail-safe setting</h1>"
    "\n  <br>"
    "\n  <form class=\"form-inline\" action=\"/wifi\" method=\"POST\">"
    "\n    <label for=\"email\">SSID:  </label> <input type=\"text\" placeholder=\"Your WiFi name\" name=\"ssid\">"
    "\n    <label for=\"pwd\">Password: </label> <input type=\"text\" placeholder=\"Your WiFi password\" name=\"pass\">"
    "\n    <button type=\"submit\">Submit</button>"
    "\n  </form>"
    "\n  <p>Warning: This board works with 2.4 GHz networks only!</p>"
    "\n</body>"
    "\n"
    "\n</html>";

//----------------------------------------------------------------------------------------------------------

STUSB4500 usb;

//----------------------------------------------------------------------------------------------------------

CRGB leds[NUM_LEDS];

//----------------------------------------------------------------------------------------------------------
int oldPushValue[3] = {0, 0, 0};
int oldPotValue = 0;

void readExternalPeriphreal(int side, int device)
{
  const char topic[3][6] = {{"left"}, {"right"}, {"up"}};
  if (device == 0)
  {
    int newValue = !digitalRead(gpio[side * 2]) * 15;
    if (newValue != oldPushValue[side])
    {
      oldPushValue[side] = newValue;
      client.publish((mainTopic + "/" + String(uuid) + "/" + topic[side]).c_str(), String(newValue).c_str());
      Serial.println("[MQTT Client] Sent: /" + String(topic[side]) + "/ [" + String(newValue) + "]");
    }
  }
  else if (device == 1)
  {
    int newValue = map(analogRead(gpio[side * 2]), 0, 4096, 0, 34);
    if (newValue != oldPotValue)
    {
      oldPotValue = newValue;
      client.publish((mainTopic + "/" + String(uuid) + "/" + topic[side]).c_str(), String(newValue).c_str());
      Serial.println("[MQTT Client] Sent: /" + String(topic[side]) + "/ [" + String(newValue) + "]");
    }
  }
  else
  {
  }
}
//----------------------------------------------------------------------------------------------------------
void writeExternalPeriphreal(int value)
{
  ledcWrite(1, value * 17);
  ledcWrite(2, 255 - value * 17);
}
//----------------------------------------------------------------------------------------------------------
void prepareExternalPeriphreal(int side, uint8_t type)
{
  switch (type)
  {
  case 0: // Pushbutton
    pinMode(gpio[side * 2], INPUT_PULLUP);
    break;
  case 1: // Potentiometer
    pinMode(gpio[side * 2], INPUT);
    pinMode(gpio[side * 2 + 1], INPUT_PULLUP);
    //Serial.println(side*2+1);
    //digitalWrite(gpio[side * 2 + 1], HIGH);
    break;
  case 2: // Leds
    ledcSetup(1, 1000, 8);
    ledcSetup(2, 1000, 8);
    ledcAttachPin(gpio[side * 2], 1);
    ledcAttachPin(gpio[side * 2 + 1], 2);
    const char topic[3][6] = {{"left"}, {"right"}, {"up"}};
    client.subscribe((mainTopic + "/" + String(uuid) + "/" + topic[side]).c_str());
    break;
  }
}
//----------------------------------------------------------------------------------------------------------
int lastLightState = 0;
long lastLightTime = 0;

void readLightIntensity(int sens_pin)
{
  int newValue = map(analogRead(sens_pin), 0, 4096, 0, 15);
  if ((newValue != lastLightState) || (millis() > lastLightTime + 5000))
  {
    client.publish((mainTopic + "/" + String(uuid) + "/" + "light").c_str(), String(newValue).c_str());
    lastLightState = newValue;
    lastLightTime = millis();
    Serial.println("[MQTT Client] Sent: /light/ [" + String(newValue) + "]");
  }
}
//----------------------------------------------------------------------------------------------------------
OneWire oneWire(TEMP_PIN);
DallasTemperature sensor(&oneWire);
float lastTemperature = 0.00;
long lastTempTime = 0;

void readDallasTemperature(int sens_pin)
{
  sensor.requestTemperatures();
  int newValue = map(sensor.getTempCByIndex(0), 20, 36, 0, 16);
  if ((newValue != lastTemperature) || (millis() > lastTempTime + 5000))
  {
    client.publish((mainTopic + "/" + String(uuid) + "/" + "temp").c_str(), String(newValue).c_str());
    lastTemperature = newValue;
    lastTempTime = millis();
    Serial.println("[MQTT Client] Sent: /temp/ [" + String(newValue) + "]");
  }
}
//----------------------------------------------------------------------------------------------------------
uint8_t octave = OCTAVE;
double ledcWriteNote(uint8_t chan, int note, uint8_t Octave)
{
  const uint16_t noteFrequencyBase[12] = {
      //   C        C#       D        Eb       E        F       F#        G       G#        A       Bb        B
      4186, 4435, 4699, 4978, 5274, 5588, 5920, 6272, 6645, 7040, 7459, 7902};

  if (Octave > 8 || note > 11)
  {
    return 0;
  }
  double noteFreq = (double)noteFrequencyBase[note] / (double)(1 << (8 - Octave));
  return ledcWriteTone(chan, noteFreq);
}
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
void reconnect()
{
  // Loop until we're reconnected
  if (!client.connected())
  {
    Serial.println("[MQTT Client] Attempting connection to server...");
    String clientId = name;
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str()))
    {
      Serial.println("[MQTT Client] Connected to subtopic /" + String(uuid) + "/");
      Serial.println("[MQTT Client] Ready.");
      //====================================================
      client.subscribe((mainTopic + "/" + String(uuid) + "/" + "tone").c_str());
      client.subscribe((mainTopic + "/" + String(uuid) + "/" + "octave").c_str());
      client.subscribe((mainTopic + "/" + String(uuid) + "/" + "map").c_str());

      //====================================================
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds.");
      delay(5000);
    }
  }
}
//----------------------------------------------------------------------------------------------------------
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("[MQTT Client] Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if (topic[19] == 'm')
  {
    int value = 0;
    for (int i = 0; i < length; i++)
    {
      value = value * 10 + payload[i] - 48;
    }
    display(value); // run simulation frame
  }
  if (topic[19] == 't')
  {
    int value = 0;
    for (int i = 0; i < length; i++)
    {
      value = value * 10 + payload[i] - 48;
    }
    if (value == 0)
    {
      ledcWriteTone(0, 0);
    }
    else if (value > 11)
    {
      ledcWriteNote(0, 11, octave);
    }
    else
    {
      ledcWriteNote(0, value - 1, octave);
    }
  }
  if (topic[19] == 'o')
  {
    int value = 0;
    for (int i = 0; i < length; i++)
    {
      value = value * 10 + payload[i] - 48;
    }
    octave = value;
  }
  const char sideNumbers[3] = {'l', 'r', 'u'};
  for (int p = 0; p < 3; p++)
  {
    if (topic[19] == sideNumbers[p])
    {
      int value = 0;
      for (int i = 0; i < length; i++)
      {
        value = value * 10 + payload[i] - 48;
      }
      writeExternalPeriphreal(value);
    }
  }
}
//----------------------------------------------------------------------------------------------------------
void setup()
{
  EEPROM.begin(512);
  Serial.begin(115200);
  sensor.begin();
  ledcSetup(0, 500, 8);
  ledcAttachPin(BUZZER_PIN, 0);
  Serial.println("[SYSTEM] Booting");
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(USER_BTN, INPUT_PULLUP);
  WiFi.mode(WIFI_STA);
  //WiFi.begin("nvias.org", "22448132");
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  //WiFi.waitForConnectResult();
  if (!SPIFFS.begin(true))
  {
    Serial.println("[SYSTEM] An Error has occurred while mounting SPIFFS!");
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    return;
  }
  String PASS = "";
  String SSID = "";
  for (int l = SSID_ADDR; l < SSID_ADDR + 127; ++l)
  {
    char data = EEPROM.read(l);
    if (data != '$')
    {
      SSID += data;
    }
    else
    {
      break;
    }
  }
  ssid = SSID.c_str();
  for (int l = PASS_ADDR; l < PASS_ADDR + 127; ++l)
  {
    char data = EEPROM.read(l);
    if (data != '$')
    {
      PASS += data;
    }
    else
    {
      break;
    }
  }
  password = PASS.c_str();
  Serial.print("[SYSTEM] Connecting to ");
  Serial.println(ssid);
  if(WiFi.status() != WL_CONNECTED)
  {
    WiFi.begin(ssid, password);
  }
  WiFi.waitForConnectResult();
  if (!MDNS.begin("espcraft"))
  {
    Serial.println("[SYSTEM] Error starting mDNS");
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    Serial.println("[SYSTEM] mDNS service running...");
  }
  if (!SPIFFS.exists("/index.html"))
  {
    Serial.println("[SYSTEM] WiFi WEBGUI not present, rewriting to fail-safe...");
    File file = SPIFFS.open("/index.html", "w");
    file.print(wifiSet);
    file.close();
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("[SYSTEM] Connection to WiFI failed, setting up Acces Point.");
    WiFi.softAP("ESP Block", "espcraft");
    Serial.print("[SYSTEM] IP address: ");
    Serial.println(WiFi.softAPIP());
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    Serial.print("[SYSTEM] IP address: ");
    Serial.println(WiFi.localIP());
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
  }

  String readUuid = "ESPblock(";
  for (int l = SET_ADDR + 3; l < SET_ADDR + 7; l++)
  {
    char data = EEPROM.read(l);
    if (data != '$')
    {
      readUuid += data;
    }
    else
    {
      break;
    }
  }
  readUuid += ")";
  readUuid.toCharArray(uuid, 15);

  //--------------------------------------------------------------------------------------------------------------------------------------------
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", String(), false);
  });
  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request) {
    SPIFFS.remove("/index.html");
    File file = SPIFFS.open("/index.html", "w");
    file.print(wifiSet);
    file.close();
    for (int j = 0; j < 4; j++)
    {
      EEPROM.write(SET_ADDR + 3 + j, '0');
    }
    EEPROM.write(SET_ADDR + 7, '$');
    EEPROM.commit();
    Serial.println("[SYSTEM] Performed reset!");
    Serial.println("[SYSTEM] Wifi set WEBGUI rewritten to fail-safe version.");
    Serial.println("[SYSTEM] UUID set to 0000");
    request->send(SPIFFS, "/index.html", String(), false);
  });
  server.on("/perip", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/perip.html", String(), false);
  });
  server.on("/uuid", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/uuid.html", String(), false);
  });

  server.on("/wifi", HTTP_POST, [](AsyncWebServerRequest *request) {
    int params = request->params();
    Serial.printf("[HTTP CLIENT] %d params sent in\n", params);
    for (int i = 0; i < params; i++)
    {
      AsyncWebParameter *p = request->getParam(i);
      if (p->isPost())
      {
        String data = p->value().c_str();
        if (i == 0)
        {
          data = data + +"$";
          for (int i = SSID_ADDR; i < PASS_ADDR; i++)
          {
            EEPROM.write(i, 0);
          }
          for (int j = 0; j < data.length() - 1; j++)
          {
            EEPROM.write(SSID_ADDR + j, data[j]);
          }
          EEPROM.commit();
          Serial.println("[SYSTEM] SSID set to " + data);
        }
        if (i == 1)
        {
          data = data + +"$";
          for (int i = PASS_ADDR; i < PASS_ADDR + 128; i++)
          {
            EEPROM.write(i, 0);
          }
          for (int j = 0; j < data.length() - 1; j++)
          {
            EEPROM.write(PASS_ADDR + j, data[j]);
          }
          EEPROM.commit();
          Serial.println("[SYSTEM] PASS set to " + data);
        }
      }
    }
    request->send(SPIFFS, "/index.html", String(), false);
  });

  server.on("/perip", HTTP_POST, [](AsyncWebServerRequest *request) {
    int params = request->params();
    Serial.printf("[HTTP CLIENT] %d params sent in\n", params);
    String data = "";
    for (int i = 0; i < params; i++)
    {
      AsyncWebParameter *p = request->getParam(i);
      if (p->isPost())
      {
        data += p->value().c_str();
        data += " ";
      }
    }
    char command[32] = "";
    data.toCharArray(command, 32);
    for (int i = 0; i < 3; i++) //---------------- (push -> pot -> leds)
    {
      bool comp = 1;
      for (int j = 0; j < 3; j++)       //-------------- (p->u->s)
      {                                 //                |
        if (command[j] != perips[i][j]) //-------- (p!=p OR u!=u OR s!=s ? 0:1)
        {
          comp = 0;
        }
      }
      if (comp) //-------------------------------- (true(above) ?)
      {
        for (int x = 0; x < 3; x++)
        {
          if (command[4] == ports[0][x] || command[5] == ports[0][x])
          {
            char response[] = "";
            sprintf(response, "[SYSTEM] Periphreal board \"%s\" is now attached to the %s port.", perips[i], ports[x + 1]);
            Serial.println(response);
            EEPROM.write(SET_ADDR + x, byte(i));
            EEPROM.commit();
          }
        }
      }
    }

    request->send(SPIFFS, "/perip.html", String(), false);
  });

  server.on("/uuid", HTTP_POST, [](AsyncWebServerRequest *request) {
    int params = request->params();
    Serial.printf("[HTTP CLIENT] %d params sent in\n", params);
    for (int i = 0; i < params; i++)
    {
      AsyncWebParameter *p = request->getParam(i);
      if (p->isPost())
      {
        String data = p->value().c_str();
        data = data + +"$";
        for (int j = 0; j < data.length() - 1; j++)
        {
          EEPROM.write(SET_ADDR + 3 + j, data[j]);
        }
        EEPROM.commit();
        Serial.println("[SYSTEM] UUID set to " + data);
      }
    }
    request->send(SPIFFS, "/uuid.html", String(), false);
  });

  AsyncElegantOTA.begin(&server);
  server.begin();
  Serial.println("[HTTP CLIENT] HTTP server started");
  //---------------------------------------------------------------------------------------------------------------------------------------------

  Serial.println("[SYSTEM] Reset the board and hold user button when blue LED is light up to for settings...");
  Serial.println("[SYSTEM] ESPCraftOS " + String(OS_VERSION) + " ready. Starting MQTT client...\n");

  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  //----------------------------------------------------------------------------------------------------------
  if (!digitalRead(USER_BTN))
  {
    delay(500);
    Serial.println("-----------------------------------------------------------------------");
    Serial.println("[SYSTEM] Setting mode acive!");
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("[SYSTEM] Performing SPIFFS file-check...");
    File root = SPIFFS.open("/");
    File file = root.openNextFile();

    while (file)
    {

      Serial.print("[SPIFFS] FILE: ");
      Serial.print(file.name());
      Serial.print(", size: ");
      Serial.print(file.size());
      Serial.println(" bytes.");

      file = root.openNextFile();
    }
    file.close();

    delay(500);
    if (!digitalRead(USER_BTN))
    {
      digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("[SYSTEM] Performing SPIFFS NETGUI update, please do not turn off the board!");
      server.end();
      Serial.println("[SPIFFS] Deleting old files...");
      SPIFFS.remove("/index.html");
      SPIFFS.remove("/perip.html");
      SPIFFS.remove("/uuid.html");
      Serial.println("[SPIFFS] Downloading new files...");
      File file = SPIFFS.open("/index.html", "w");
      HTTPClient http;
      http.begin("http://iot.e-iot.cz/index.html"); //Specify the URL and certificate
      int httpCode = http.GET();                    //Make the request
      if (httpCode == HTTP_CODE_OK)
      { //Check for the returning code

        http.writeToStream(&file);
      }
      file.close();
      Serial.println("[SPIFFS] Downloaded file perip.html");
      http.end(); //Free the resources
      file = SPIFFS.open("/perip.html", "w");
      http.begin("http://iot.e-iot.cz/perip.html"); //Specify the URL and certificate
      httpCode = http.GET();                        //Make the request
      if (httpCode == HTTP_CODE_OK)
      { //Check for the returning code

        http.writeToStream(&file);
      }
      file.close();
      Serial.println("[SPIFFS] Downloaded file perip.html");
      http.end(); //Free the resources
      file = SPIFFS.open("/uuid.html", "w");
      http.begin("http://iot.e-iot.cz/uuid.html"); //Specify the URL and certificate
      httpCode = http.GET();                       //Make the request
      if (httpCode == HTTP_CODE_OK)
      { //Check for the returning code

        http.writeToStream(&file);
      }
      file.close();
      Serial.println("[SPIFFS] Downloaded file uuid.html");
      http.end(); //Free the resources
    }

    Serial.println("[SYSTEM] Set WiFi SSID/PASS with -s Your_WiFi_SSID or -p Your_WiFi_PASS");
    Serial.println("[SYSTEM] Or show help with -h");
    String data = "";
    Serial.setTimeout(100);
    while (1)
    {
      if (Serial.available())
      {
        msg = Serial.readString();
        data = msg.substring(3);
        if (msg[1] == 'a')
        {
          char command[32] = "";
          data.toCharArray(command, 32);
          for (int i = 0; i < 3; i++) //---------------- (push -> pot -> leds)
          {
            bool comp = 1;
            for (int j = 0; j < 3; j++)       //-------------- (p->u->s)
            {                                 //                |
              if (command[j] != perips[i][j]) //-------- (p!=p OR u!=u OR s!=s ? 0:1)
              {
                comp = 0;
              }
            }
            if (comp) //-------------------------------- (true(above) ?)
            {
              for (int x = 0; x < 3; x++)
              {
                if (command[4] == ports[0][x] || command[5] == ports[0][x])
                {
                  char response[] = "";
                  sprintf(response, "[SYSTEM] Periphreal board \"%s\" is now attached to the %s port.", perips[i], ports[x + 1]);
                  Serial.println(response);
                  EEPROM.write(SET_ADDR + x, byte(i));
                  EEPROM.commit();
                }
              }
            }
          }
        }
        if (msg[1] == 'd')
        {
          String wifi_ssid = "default$";
          for (int j = 0; j < wifi_ssid.length(); j++)
          {
            EEPROM.write(SSID_ADDR + j, wifi_ssid[j]);
          }
          String wifi_pass = "default$";
          for (int j = 0; j < wifi_pass.length(); j++)
          {
            EEPROM.write(PASS_ADDR + j, wifi_pass[j]);
          }
          EEPROM.commit();
          Serial.println("[SYSTEM] Defaults assumed!");
        }
        if (msg[1] == 'h')
        {
          Serial.println("[SYSTEM] Help:");
          Serial.println("\t -d  Load defaults\n");
          Serial.println("\t -s  Set new WiFi SSID");
          Serial.println("\t -p  Set new WiFi PASS\n");
          Serial.println("\t -a  Attach periphreals to ports");
          Serial.println("\t -h  Show this help");
        }
        if (msg[1] == 's')
        {
          data = data + +"$";
          for (int i = SSID_ADDR; i < PASS_ADDR; i++)
          {
            EEPROM.write(i, 0);
          }
          for (int j = 0; j < data.length() - 1; j++)
          {
            EEPROM.write(SSID_ADDR + j, data[j]);
          }
          EEPROM.commit();
          Serial.println("[SYSTEM] SSID set to " + data);
        }
        if (msg[1] == 'p')
        {
          data = data + +"$";
          for (int i = PASS_ADDR; i < PASS_ADDR + 128; i++)
          {
            EEPROM.write(i, 0);
          }
          for (int j = 0; j < data.length() - 1; j++)
          {
            EEPROM.write(PASS_ADDR + j, data[j]);
          }
          EEPROM.commit();
          Serial.println("[SYSTEM] PASS set to " + data);
        }
        /*
        if(msg[1] == ){
          
        }
        */
      }
    }
  }
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
  //----------------------------------------------------------------------------------------------------------
  LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}
//----------------------------------------------------------------------------------------------------------

void loop()
{
  AsyncElegantOTA.loop();
  if (!client.connected())
  {
    reconnect();
    uint8_t device = ' ';
    for (int i = 0; i < 3; ++i)
    {
      device = EEPROM.read(i + SET_ADDR);
      prepareExternalPeriphreal(i, device);
    }
  }
  readLightIntensity(LIGHT_SNS);
  readDallasTemperature(TEMP_PIN);

  int device = 0;
  for (int i = 0; i < 3; ++i)
  {
    device = EEPROM.read(i + SET_ADDR);
    readExternalPeriphreal(i, device);
  }

  
  

  client.loop();
}


void display(int color)
{ 
  byte bitmap[64] ={0,0,0,1,1,0,0,0,
                    0,0,0,1,1,0,0,0,
                    0,0,0,1,1,0,0,0,
                    1,1,1,1,1,1,1,1,
                    1,1,1,1,1,1,1,1,
                    0,0,0,1,1,0,0,0,
                    0,0,0,1,1,0,0,0,
                    0,0,0,1,1,0,0,0};
  
  byte col[16][3] = {{0,0,0},{0,0,255},{0,255,0},{0,255,255}, {255,0,0},{255,0,255},{255,255,0},{255,255,255},
                     {0,0,0},{0,0,127},{0,127,0},{0,127,127}, {127,0,0},{127,0,127},{127,127,0},{127,127,127}};

	for(int i = 0; i < 64; i++){
    leds[i] = CRGB(col[color][0]*bitmap[i],col[color][1]*bitmap[i],col[color][2]*bitmap[i]);
    FastLED.show(); 
    delay(1);
	}
}

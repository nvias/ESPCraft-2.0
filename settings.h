
#define LED_BUILTIN 17                                  // Vestavěná LED
#define BUZZER_PIN  33                                  // Vestavěný bzučák
#define OCTAVE      6                                   // Pracovní tónová oktáva
#define TEMP_PIN    32                                  // Vestavěné čidlo teploty (DS18B20)
#define LIGHT_SNS   36                                  // Vestavěný senzor světla
#define USER_BTN    0                                   // Tlačítko na desce (na nových verzích GPIO 0, na starších GPIO 16)

/*
const char perips[3][5] = {{"push"},                    // Jména základních externích periferií
                           {"pot"},                     // (ID) tlačítko - 0, potenciometr - 1, ledky - 2
                           {"leds"}};
const char ports[4][6] = {{"LRU"},                      // Identifikátoy portů
                          {"left"},{"right"},{"upper"}};// Jména portů
const int gpio[6] = {26,25,19,18,35,34};                // Čísla GPIO pinů v portech (řazeno SDA, SCK)

const char name[] = "ESPCraft";                         // Jméno desky
//const char uuid[] = "ESPblock(001D)";                   // Unikátní ID desky (M pro mentory - poslední M002), poslední 001D
String mainTopic = "PI1";                               // Topic pro propojení se serverem

#define NUM_LEDS    64                                  // Počet LED diod na displeji v kitu
#define DATA_PIN    27                                  // Pin na kterém jsou připojeny
#define BRIGHTNESS  32                                  // Jas LED diod

#define PD_ADDR     0x2A                                // Adresa Power Delivery controlleru na interní  I2C sběrnici

#define MQTT_SERVER "147.228.121.4"                     // Adresa MQTT serveru na který se deska připojuje
#define MQTT_PORT   80                                  // Port pro připojení k MQTT serveru
*/
/*
#define LED_BUILTIN 17                                  // Vestavěná LED
#define BUZZER_PIN  33                                  // Vestavěný bzučák
#define OCTAVE      6                                   // Pracovní tónová oktáva
#define TEMP_PIN    32                                  // Vestavěné čidlo teploty (DS18B20)
#define LIGHT_SNS   36                                  // Vestavěný senzor světla
#define USER_BTN    16                                  // Tlačítko na desce (na nových verzích GPIO 0, na starších GPIO 16)
#define SSID_ADDR   0                                   // 128 bajtů pro název sítě (SSID)
#define PASS_ADDR   128                                 // 128 bajtů pro heslo sítě (PASS)
#define SET_ADDR    256                                 // Adresa uloženého nastavení desky v EEPROM

const char perips[3][5] = {{"push"},                    // Jména základních externích periferií
                           {"pot"},                     // (ID) tlačítko - 0, potenciometr - 1, ledky - 2
                           {"leds"}};
const char ports[4][6] = {{"LRU"},                      // Identifikátoy portů
                          {"left"},{"right"},{"upper"}};// Jména portů
const int gpio[6] = {26,25,19,18,14,13};                // Čísla GPIO pinů v portech (řazeno SDA, SCK)

const char name[] = "ESPCraft";                         // Jméno desky
const char uuid[] = "ESPblock(0008)";                   // Unikátní ID desky
String mainTopic = "PI1";                               // Topic pro propojení se serverem

#define NUM_LEDS    64                                  // Počet LED diod na displeji v kitu
#define DATA_PIN    27                                  // Pin na kterém jsou připojeny

#define PD_ADDR     0x2A                                // Adresa Power Delivery controlleru na interní  I2C sběrnici

#define MQTT_SERVER "147.228.121.4"                     // Adresa MQTT serveru na který se deska připojuje
#define MQTT_PORT   80                                  // Port pro připojení k MQTT serveru
*/

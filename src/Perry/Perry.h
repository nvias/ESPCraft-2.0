// Perry - periphreal software
// Created by nvias - Jiří Švihla, 2021
// Licensed under MIT License

#ifndef Perry_h
#define Perry_h

#include <Arduino.h>
#include <Preferences.h> 

//============================ DEFAULTS ========================================================
#define LED_BUILTIN 17      // Vestavěná LED
#define BUZZER_PIN  33      // Vestavěný bzučák
#define OCTAVE      6       // Pracovní tónová oktáva
#define TEMP_PIN    32      // Vestavěné čidlo teploty (DS18B20)
#define LIGHT_SNS   36      // Vestavěný senzor světla
#define USER_BTN    0       // Tlačítko na desce (na nových verzích GPIO 0, na starších GPIO 16)

#define NUM_PIXELS  64      // Počet LED diod na displeji v kitu
#define PIXEL_PIN   27      // Pin na kterém jsou připojeny
#define BRIGHTNESS  32      // Jas LED diod
//==============================================================================================

#define CODE_OK             0x00000001
#define CODE_FAIL           0xFFFFFFFF
#define CODE_WIFI_FAIL      0xF0F0F0F0
#define CODE_SENSOR_FAIL    0x0000FFFF
#define CODE_SPIFFS_FAIL    0xFFFF00FF
#define CODE_ERROR          0xFFFFFFFF


class Perry
{
    public:
        Perry(int stack_size, int priority);
        void init(void);
        void coldBoot(void);
        void readTemp(void);
        void readLight(void);
        void buzzTone(void);
        void blinkLed(int blinkCode);
    private:
        int _stack_size;
        int _priority;
};

#endif
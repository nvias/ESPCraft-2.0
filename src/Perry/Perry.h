// Perry - periphreal software
// Created by nvias - Jiří Švihla, 2021
// Licensed under MIT License

#ifndef Perry_h
#define Perry_h

#include <Arduino.h>
#include <Preferences.h>
#include <FastLED.h> 

//===== DEFAULTS ===============================================================================
#define LED_BUILTIN 17                  // Vestavěná LED
#define BUZZER_PIN  33                  // Vestavěný bzučák
#define OCTAVE      6                   // Pracovní tónová oktáva
#define TEMP_PIN    32                  // Vestavěné čidlo teploty (DS18B20)
#define LIGHT_SNS   36                  // Vestavěný senzor světla
#define USER_BTN    0                   // Tlačítko na desce (na nových verzích GPIO 0, na starších GPIO 16)

#define NUM_PIXELS  64                  // Počet LED diod na displeji v kitu
#define PIXEL_PIN   27                  // Pin na kterém jsou připojeny
#define BRIGHTNESS  255                 // Jas LED diod
//==============================================================================================

//===== BOOT CODES =============================================================================
#define PERRY_OK             0xFFE7FFFF     // ##---OO---##
#define PERRY_FAIL           0xFF81FFFF     // ##-OOOOOO-##
//==============================================================================================

class Perry
{
    public:
        Perry(int stack_size, int priority);
        int init(void);
        void coldBoot(void);
        int readTemp(int pin);
        int readLight(int pin);
        void buzzTone(int pin);
        void blinkLed(int blinkCode);
        void show(uint8_t hue, uint8_t saturation, uint8_t value);
    private:
        int _stack_size;
        int _priority;
};

#endif
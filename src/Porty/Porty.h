// Porty - Port service
// Created by nvias - Jiří Švihla, 2021
// Licensed under MIT License

#ifndef Porty_h
#define Porty_h

#include <Arduino.h>
#include <Preferences.h>
#include <FastLED.h>

//===== BOOT CODES =============================================================================
#define PORTY_OK             0xE7FFFFFF        // ##---OO---##
#define PORTY_FAIL           0x81FFFFFF        // ##-OOOOOO-##
//==============================================================================================

class Porty
{
    public:
        Porty(void);
        void coldBoot();
        int init();

    private:
    
};

#endif
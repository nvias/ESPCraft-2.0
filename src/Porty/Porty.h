// Porty - Port service
// Created by nvias - Jiří Švihla, 2021
// Licensed under MIT License

#ifndef Porty_h
#define Porty_h

#include <Arduino.h>
#include <Preferences.h>
#include <FastLED.h>

//===== BOOT CODES =============================================================================
#define PORTY_OK            0xE7FFFFFF      // ##---OO---##
#define PORTY_FAIL          0x81FFFFFF      // ##-OOOOOO-##
//==============================================================================================

//===== DEFAULTS ===============================================================================
#define PORTY_DATA          0               // Primary data pin of port
#define PORTY_CLK           1               // Secondary data (or clk) pin of port

#define PORTY_LEFT_1        26              // 
#define PORTY_LEFT_2        25              // 
#define PORTY_UP_1          35              // 
#define PORTY_UP_2          34              // 
#define PORTY_RIGHT_1       19              // 
#define PORTY_RIGHT_2       18              // 
#define PORTY_MATRIX        27              // 
#define PORTY_MATRIX_PIXELS 5               // Počet LED diod na displeji v kitu
#define PORTY_MATRIX_BR     255             // 
//==============================================================================================

class Porty
{
    public:
        Porty(void);
        void coldBoot();
        int init();
        int readButton();

    private:
    
};

#endif
// Porty - Port service
// Created by nvias - Jiří Švihla, 2021
// Licensed under MIT License

#ifndef Porty_h
#define Porty_h

#include <Arduino.h>
#include <Preferences.h>
#include <FastLED.h>


class Porty
{
    public:
        Porty(void);
        void coldBoot();
        void init();

    private:
    
};

#endif
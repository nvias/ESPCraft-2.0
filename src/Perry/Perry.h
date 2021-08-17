// Perry - periphreal software
// Created by nvias - Jiří Švihla, 2021
// Licensed under MIT License

#ifndef Perry_h
#define Perry_h

#include <Arduino.h>

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
        void blinkLed(void);
    private:
        int _stack_size;
        int _priority;
};

#endif
// Netty - net services provider
// Created by nvias - Jiří Švihla, 2021
// Licensed under MIT License

#ifndef Netty_h
#define Netty_h

#include <Arduino.h>
#include <Preferences.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
//#include <AsyncElegantOTA.h>
#include <SPIFFS.h>
#include <HTTPClient.h>

//===== BOOT CODES =============================================================================
#define NETTY_OK             0xFFFFE7FF        // ##---OO---##
#define NETTY_FAIL           0xFFFF81FF        // ##-OOOOOO-##
//==============================================================================================


class Netty
{
    public:
        Netty(void);
        void coldBoot();
        int init();

    private:
    
};

#endif
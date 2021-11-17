#include "settings.h"                                           // Settings file
#include "version.h"                                            // Version history file

#include <Preferences.h>                                        // NVS flash interface
#include <nvs_flash.h>                                          // NVS flash additional fonctionality lib.
Preferences settings;

#define FORCE_COLD_BOOT 0                                       // Force cold boot

//#include "src/3rdparty/QueueArray/QueueArray.h"               // CommandcoreQueue library
//QueueArray <String>coreQueue;

#include "src/Commpy/Commpy.h"                                  // Commpy (Communication provider)
Commpy commpy;

#include "src/Netty/Netty.h"                                    // Netty (Internet services interface)
Netty netty;

#include "src/Perry/Perry.h"                                    // Perry (Periphreals interface)
Perry perry(2048, 1);

#include "src/Porty/Porty.h"                                    // Porty (IO port interface)
Porty porty;

typedef struct Message 
{
    int pid;        // PID - Program ID (01-Commpy, 02-Netty, 03-Perry, 04-Porty) 
    int status;     // Program status (00-Stopped, 01-Idle, 02-Working, 03-Error, 04-Crash)
    char body[32];  // The message for core
}   Message;
Message msg;
static QueueHandle_t coreQueue;

void setup()
{
    delay(1000);
    settings.begin("os-pref", true);
    Serial.begin(115200);

//=================  BOOT SELFCHECK =================
    if(!settings.getUChar("boot", 0) || FORCE_COLD_BOOT){
        // COLD BOOT HANDLER
        // UPGRADE PROCEDURE NOT IMPLEMENTED YET
        Serial.println("Cold boot");
        settings.end();
        nvs_flash_erase();
        nvs_flash_init();
        settings.begin("os-pref", false);
        settings.putUChar("boot", 1);
        settings.putUShort("version", OS_VERSION);
        settings.putUShort("board-ver", BOARD_VER);
        settings.end();

        commpy.coldBoot();
        netty.coldBoot();
        perry.coldBoot();
        porty.coldBoot();
    }
    coreQueue = xQueueCreate(16, sizeof(Message));

    int status = commpy.init();
    status = status & netty.init();
    status = status & perry.init();
    status = status & porty.init();

    perry.blinkLed(status);
    Serial.print("Boot code: ");
    Serial.println(status, HEX);
    
//===================================================

}

void loop()
{
    // NETTY - Server update

    // PERRY - Periphreals reading

    // PORTY - External periphreals reading

    // COMMPY - Commection problems


    vTaskDelay(portTICK_PERIOD_MS);
}
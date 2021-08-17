#include "settings.h"                                           // Settings file
#include "version.h"                                            // Version history file

#include <Preferences.h>                                        // NVS flash interface
#include <nvs_flash.h>                                          // NVS flash additional fonctionality lib.
Preferences settings;

//#include "src/3rdparty/QueueArray/QueueArray.h"                 // Command queue library
//QueueArray <String> queue;

#include "src/Commpy/Commpy.h"                                // Commpy (Communication provider)
Commpy commpy;

#include "src/Netty/Netty.h"                                  // Netty (Internet services interface)
Netty netty;

#include "src/Perry/Perry.h"                                    // Perry (Periphreals interface)
Perry perry(2048, 1);

#include "src/Porty/Porty.h"                                  // Porty (IO port interface)
Porty porty;

typedef struct Message 
{
    int pid;        // PID - Program ID (01-Commpy, 02-Netty, 03-Perry, 04-Porty)
    int status;     // Program status (00-Stopped, 01-Idle, 02-Working, 03-Error, 04-Crash)
    char body[32];  // The message for core
}   Message;
Message msg;
static QueueHandle_t queue;

void setup()
{
    delay(1000);
    settings.begin("os-pref", false);

//=================  BOOT SELFCHECK =================
    if(settings.getUChar("boot", 0) == 0){
        // COLD BOOT HANDLER
        // UPGRADE PROCEDURE NOT IMPLEMENTED YET
        settings.end();
        nvs_flash_erase();
        nvs_flash_init();
        settings.begin("os-pref", false);
        settings.putUShort("version", OS_VERSION);
        settings.putUChar("boot", 0x01);
        settings.end();

        commpy.coldBoot();
        netty.coldBoot();
        perry.coldBoot();
        porty.coldBoot();
    }
    queue = xQueueCreate(16, sizeof(Message));

    commpy.init();
    netty.init();
    perry.init();
    porty.init();
    
//===================================================

}

void loop()
{
    if (xQueueReceive(queue, (void *)&msg, 0) == pdTRUE) {

    }
}
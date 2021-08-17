
#include "Perry.h"

Perry::Perry(int stack_size, int priority)  // Specify stack size (in Bytes) of the process & its priority.
{                                           // For activation call routine Perry.init();
    _stack_size = stack_size;
    _priority = priority;
}
//=================================================================================================================================

void Perry::init(void)
{
    
}
//=================================================================================================================================
void Perry::coldBoot(void)
{
    /*
    xTaskCreate(
    Perry.buzzTone(),   // Function that should be called
    "Buzzer IO",        // Name of the task (for debugging)
    1023,               // Stack size (bytes)
    NULL,               // Parameter to pass
    1,                  // Task priority
    NULL                // Task handle
  );
  */
    
}
//=================================================================================================================================

//=================================================================================================================================
void Perry::blinkLed(void)
{

}
//=================================================================================================================================

static int buzzCode = 0x00000000;                       // Communication variable
void Perry::buzzTone()
{
    int buzzPin = 33;
    for(;;)
    {
        if(buzzCode != 0x00)
        {
            
        }
        else
        {
            vTaskDelay(500 / portTICK_PERIOD_MS);
        }
    }
}
//=================================================================================================================================
void Perry::readLight(void)
{

}
//=================================================================================================================================
void Perry::readTemp(void)
{

}
//=================================================================================================================================
static int blinkCode = 0x50000000;                      // Communication variable
void statusLed(void * parameter)
{
    int ledPin = 17;
    for(;;)
    {
        if(blinkCode != 0)
        {
            for(int i = 0; i < 32; i++)
            {
                digitalWrite(ledPin, (bool) (blinkCode & (1<<i)));
                vTaskDelay(100 / portTICK_PERIOD_MS);
            }
            if(blinkCode < 0x80000000)
            {
                blinkCode = 0;
            }
        }
        else
        {
            vTaskDelay(500 / portTICK_PERIOD_MS);
        }
    }
}
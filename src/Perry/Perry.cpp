#include "Perry.h"

#define QUEUE_SIZE  4       // Data array for communication between Perry tasks
int queueArray[QUEUE_SIZE]; // 0 - USR_LED, 1-BUZZER, 2-TEMP, 3-LIGHT_SNS

CRGBArray<NUM_PIXELS> leds;
Preferences perrySettings;

typedef struct perryPinout
{
    int usrLed;
    int buzzer;
    int tempSns;
    int lightSns;
    int usrBtn;
    int pixelMatrix;
}   perryPinout;
perryPinout perip;

Perry::Perry(int stack_size, int priority)  // Specify stack size (in Bytes) of the process & its priority.
{                                           // For activation call routine Perry.init();
    _stack_size = stack_size;
    _priority = priority;
}
//=================================================================================================================================

int Perry::init(void)
{
    perrySettings.begin("perry", true);
    perip.usrLed = perrySettings.getUChar("usrLed", 0xFF);
    perip.buzzer = perrySettings.getUChar("buzzer", 0xFF);
    perip.tempSns = perrySettings.getUChar("tempSns", 0xFF);
    perip.lightSns = perrySettings.getUChar("lightSns", 0xFF);
    perip.usrBtn = perrySettings.getUChar("usrBtn", 0xFF);
    perip.pixelMatrix = perrySettings.getUChar("pixelMatrix", 0xFF);
    perrySettings.end();

    pinMode(perip.usrLed, OUTPUT);
    pinMode(perip.buzzer, OUTPUT);
    pinMode(perip.lightSns, INPUT);
    pinMode(perip.usrBtn, INPUT_PULLUP);
    digitalWrite(perip.usrLed, HIGH);
    int mtx_pin = (int) perip.pixelMatrix;
    FastLED.addLeds<WS2812B,PIXEL_PIN>(leds, NUM_PIXELS);
    FastLED.setBrightness(BRIGHTNESS);

    if(perip.buzzer == 0xFF||perip.lightSns == 0xFF||perip.pixelMatrix == 0xFF||perip.tempSns == 0xFF||perip.usrBtn == 0xFF||perip.usrLed == 0xFF)
    {
        return PERRY_FAIL;
    }
    else if((readTemp(perip.tempSns) < -100) || (readLight(perip.lightSns) == 0))
    {
        return PERRY_FAIL;
    }
    else
    {
        return PERRY_OK;
    }
}
//=================================================================================================================================
void Perry::coldBoot(void)
{
    perrySettings.begin("perry", false);
    perrySettings.putUChar("usrLed", LED_BUILTIN);
    perrySettings.putUChar("buzzer", BUZZER_PIN);
    perrySettings.putUChar("tempSns", TEMP_PIN);
    perrySettings.putUChar("lightSns", LIGHT_SNS);
    perrySettings.putUChar("usrBtn", USER_BTN);
    perrySettings.putUChar("pixelMatrix", PIXEL_PIN);
    perrySettings.putUChar("pixelBrightness", BRIGHTNESS);
    perrySettings.putUChar("pixelNum", NUM_PIXELS);
    perrySettings.end();
}
//=================================================================================================================================

//=================================================================================================================================
void statusLed(void *parameter);    // function prototype
void Perry::blinkLed(int blinkCode)
{
    queueArray[0] = blinkCode;
    xTaskCreate(
    &statusLed,         // Function that should be called
    "Status Led IO",    // Name of the task (for debugging)
    1023,               // Stack size (bytes)
    NULL,               // Parameter to pass
    1,                  // Task priority
    NULL                // Task handle
  );
}
//=================================================================================================================================

static int buzzCode = 0x00000000;                       // Communication variable
void Perry::buzzTone(int pin)
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
int Perry::readLight(int pin)
{
    int value = analogRead(pin);
    if(value == 0)
    {
        return 0;
    }
    else
    {
        return map(value, 0, 4096, 0, 16);
    }
}
//=================================================================================================================================
int Perry::readTemp(int pin)
{
    return 20;
}
//=================================================================================================================================
void statusLed(void *parameter)
{
    if(queueArray[0] != 0)
    {
        for(int i = 0; i < 32; i++)
        {
            digitalWrite(perip.usrLed, (bool) (queueArray[0] & (1<<i)));
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        queueArray[0] = 0;
    }
    digitalWrite(perip.usrLed, HIGH);
    vTaskDelete(NULL);
}

void Perry::show(uint8_t hue, uint8_t saturation, uint8_t value)
{
    for(int i = 0; i < NUM_PIXELS; i++) {   
        leds[i] = CHSV(hue,saturation,value);
    }
    FastLED.show();
}
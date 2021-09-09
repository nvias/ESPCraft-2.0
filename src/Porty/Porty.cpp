#include "Porty.h"

Preferences portySettings;

typedef struct portyPinout
{
    int left[2];
    int up[2];
    int right[2];
    int matrix_pin;
    int matrix_brightness;
    int matrix_pixels;
}   portyPinout;
portyPinout port;

Porty::Porty(void)
{

}
//=================================================================================================================================
void Porty::coldBoot(void)
{
    portySettings.begin("porty", false);
    portySettings.putUChar("left-data", PORTY_LEFT_1);
    portySettings.putUChar("left-clk", PORTY_LEFT_2);
    portySettings.putUChar("right-data", PORTY_RIGHT_1);
    portySettings.putUChar("right-clk", PORTY_RIGHT_2);
    portySettings.putUChar("up-data", PORTY_UP_1);
    portySettings.putUChar("up-clk", PORTY_UP_2);
    portySettings.putUChar("matrix-pin", PORTY_MATRIX);
    portySettings.putUChar("matrix-br", PORTY_MATRIX_BR);
    portySettings.putUChar("matrix-pixels", PORTY_MATRIX_PIXELS);
    portySettings.end();
}
//=================================================================================================================================
int Porty::init(void)
{
    portySettings.begin("porty", true);
    port.left[PORTY_DATA] = portySettings.getUChar("left-data", 0xFF);
    port.left[PORTY_CLK] = portySettings.getUChar("left-clk", 0xFF);
    port.right[PORTY_DATA] = portySettings.getUChar("right-data", 0xFF);
    port.right[PORTY_CLK] = portySettings.getUChar("right-clk", 0xFF);
    port.up[PORTY_DATA] = portySettings.getUChar("up-data", 0xFF);
    port.up[PORTY_CLK] = portySettings.getUChar("up-clk", 0xFF);
    port.matrix_pin = portySettings.getUChar("matrix-pin", 0xFF);
    port.matrix_brightness = portySettings.getUChar("matrix-br", 0xFF);
    port.matrix_pixels = portySettings.getUChar("matrix-pixels", 0xFF);
    
    portySettings.end();
    
    pinMode(port.up[PORTY_DATA], INPUT);
    return PORTY_OK;
}

int Porty::readButton()
{
    return !digitalRead(port.up[PORTY_DATA]) * 15;
}


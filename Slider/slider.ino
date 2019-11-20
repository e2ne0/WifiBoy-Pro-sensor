#include <Arduino.h>
#include <wifiboypro.h>
#include "dht_sprite.h"

int sensorPin = 25;
int value;

void blit_num256(uint16_t num, uint16_t x, uint16_t y, uint8_t color_mode)
{
    uint16_t d[5];

    d[0] = num / 10000;
    d[1] = (num - d[0] * 10000) / 1000;
    d[2] = (num - d[0] * 10000 - d[1] * 1000) / 100;
    d[3] = (num - d[0] * 10000 - d[1] * 1000 - d[2] * 100) / 10;
    d[4] = num - d[0] * 10000 - d[1] * 1000 - d[2] * 100 - d[3] * 10;

    for (int i = 0; i < 5; i++)
    {
        wbpro_blitBuf8(d[i] * 8 + 120, color_mode * 8, 240, x + i * 8, y, 8, 8, (uint8_t *)sprites); //將d[0]~d[4]逐個顯示並排列
    }
}

void setup()
{
    Serial.begin(115200);
    wbpro_init();
    wbpro_initBuf8();
    for (int i = 0; i < 256; i++)
        wbpro_setPal8(i, wbpro_color565(sprite_pal[i][0], sprite_pal[i][1], sprite_pal[i][2]));
	analogInputToDigitalPin(sensorPin);
}
void loop()
{
    wbpro_clearBuf8();
    value = analogRead(sensorPin);
    // wbpro_drawString((char *)value, 100, 300, 1, 2);
    Serial.println(value, DEC);
    blit_num256(value,100,120,1);
    wbpro_blit8();
}
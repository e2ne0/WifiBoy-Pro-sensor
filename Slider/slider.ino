#include <Arduino.h>
#include <wifiboypro.h>
#include "slider_sprite.h" //字型與線條

int sensorPin = 26;
int value[240];

void blit_num256(uint16_t num, uint16_t x, uint16_t y)
{
    uint16_t d[3]; //宣告一個型別為uint16_t的變數陣列d因為是5位數所以陣列大小為5

    d[0] = num / 100;                    //算出num的百位放入d[0] uint16_t只會取整數值
    d[1] = (num - d[0] * 100) / 10;      //算出num的十位放入d[1]
    d[2] = num - d[0] * 100 - d[1] * 10; //算出num的個位放入d[2]
    for (int i = 0; i < 3; i++)
    {
        wbpro_blitBuf8(d[i] * 22 + 9, 6, 240, x + i * 22, y, 22, 34, (uint8_t *)sprites);
        //wb32_blitBuf8(起始位置x,起始位置y,原始圖片寬度,螢幕位置x,螢幕位置y,圖像寬度,圖像高度,uint8_t型別的原始圖片)
    } //將d[0]~d[2]逐個顯示並排列
}

void setup()
{
    Serial.begin(115200);
    wbpro_init(); //初始化wifiboy
    wbpro_initBuf8(); //初始化wifiboy buffer
    wbpro_fillRect(0, 0, 240, 320, wbBLACK); //背景設為黑色
    for (int i = 0; i < 256; i++)
        wbpro_setPal8(i, wbpro_color565(sprite_pal[i][0], sprite_pal[i][1], sprite_pal[i][2]));
    //設定256色
    analogSetWidth(10); //將類比的訊號設為2^10
}

void drawLine()
{
    for (int i = 0; i < 240; i++)
    {
        if (i != 239)
        {
            value[i] = value[i + 1]; 
            wbpro_blitBuf8(0, 0, 240, 239 - i, 320 - value[i], 1, value[i], (uint8_t *)sprites);
        }
        if (i == 239)
        {
            value[i] = analogRead(sensorPin) / 4; 
            wbpro_blitBuf8(0, 0, 240, 239 - i, 320 - value[i], 1, value[i], (uint8_t *)sprites);
        }
    }
}

void loop()
{
    wbpro_clearBuf8(); //清除buffer
    Serial.println(analogRead(sensorPin));
    drawLine();
    blit_num256(value[239],76,0);
    wbpro_blit8(); //顯示buffer
}


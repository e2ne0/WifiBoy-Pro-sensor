#include <Arduino.h>
#include <wifiboypro.h>
#include "dht_sprite.h"

int sensorPin = 25;
int value[240];
unsigned long currentTime;
unsigned long CD;

void setup()
{
    Serial.begin(115200);
    wbpro_init();
    for (int i = 0; i < 256; i++)
        wbpro_setPal8(i, wbpro_color565(sprite_pal[i][0], sprite_pal[i][1], sprite_pal[i][2]));
	analogInputToDigitalPin(sensorPin);
    analogSetWidth(10);
}

void loop()
{
    currentTime = millis();
    // wbpro_drawString((char *)value, 100, 300, 1, 2);
    drowLine();

}

void drowLine()
{
	if (currentTime >= CD)
	{

		for (int i = 0; i < 240; i++)
		{

			if (i != 239)
			{
				wbpro_drawLine(239 - i, 319 - value[i], 239 - i, 319 - value[i], wbBLACK, 1);
				value[i] = value[i + 1];
				wbpro_drawLine(239 - i, 140 - value[i], 239 - i, 140 - value[i], wbWHITE, 1);
			}
			if (i == 239)
			{
				wbpro_drawLine(239 - i, 319 - value[i], 239 - i, 319 - value[i], wbBLACK, 1);
				value[i] = analogRead(sensorPin)/4;
				wbpro_drawLine(239 - i, 140 - value[i], 239 - i, 140 - value[i], wbWHITE, 1);
			}
		}

		wbpro_fillRect(100, 0, 84, 32, wbBLACK);
		wbpro_drawString((char *)value[239], 100, 0, 1, 2);
		CD += 1152;
	}
}
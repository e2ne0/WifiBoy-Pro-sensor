#include <DHTesp.h>
#include <wifiboypro.h>
#include "slider_sprite.h"

int dhtPin = 23;
DHTesp dht;
int state = 5;
unsigned long currentTime;
unsigned long CD;
int temperature;
int humidity;
void blit_num256(uint16_t num, uint16_t x, uint16_t y)
{
	uint16_t d[3];

	d[0] = num / 100;
	d[1] = (num - d[0] * 100) / 10;
	d[2] = num - d[0] * 100 - d[1] * 10;
	for (int i = 0; i < 3; i++)
	{
		wbpro_blitBuf8(d[i] * 22 + 9, 6, 240, x + i * 22, y, 22, 34, (uint8_t *)sprites);
	}
}

void setup()
{
	currentTime = millis();
	pinMode(dhtPin, INPUT);
	dht.setup(dhtPin, DHTesp::DHT11);
	wbpro_init();
	wbpro_initBuf8();
	for (int i = 0; i < 256; i++)
		wbpro_setPal8(i, wbpro_color565(sprite_pal[i][0], sprite_pal[i][1], sprite_pal[i][2]));
	
	pinMode(state, INPUT);
}
void loop()
{
	currentTime = millis();
	wbpro_clearBuf8();
	if (digitalRead(state) == 0)
	{
		for (int i = 0; i < 120; i++)
		{
			for (int k = 0; k < 240; k++)
			{
				wbpro_setBuf8(i * 240 + k, 60);
			}
		}
	}
	else
	{
		for (int i = 0; i < 120; i++)
		{
			for (int k = 0; k < 240; k++)
			{
				wbpro_setBuf8(i * 240 + k, 224);
			}
		}
	}
	if (currentTime >= CD)
	{
		temperature = (int)dht.getTemperature();
		humidity = (int)dht.getHumidity();
		CD += 1152;
	}
	blit_num256(temperature, 27, 223);
	blit_num256(humidity, 147, 223);
	wbpro_blit8();
}

#include <DHTesp.h>
#include <wifiboypro.h>

int dhtPin = 23;
DHTesp dht;

unsigned long currentTime;
unsigned long CD;
uint16_t temperature[240];
uint16_t humidity[240];

String readDHTTemperature()
{
	float t = dht.getTemperature();

	if (isnan(t))
	{
		return "--";
	}
	else
	{
		Serial.println(t);
		return String(t);
	}
}

String readDHTHumidity()
{
	float h = dht.getHumidity();
	if (isnan(h))
	{
		return "--";
	}
	else
	{
		Serial.println(h);
		return String(h);
	}
}

void setup()
{
	currentTime = millis();
	dht.setup(dhtPin, DHTesp::DHT11);
	for (int i = 0; i < 240; i++)
	{
		temperature[i] = 0;
		humidity[i] = 0;
	}
    wbpro_init();
	wbpro_setTextColor(wbWHITE, wbWHITE);
	pinMode(dhtPin, INPUT);
}
void loop()
{
	currentTime = millis();
	if (currentTime >= CD)
	{

		for (int i = 0; i < 240; i++)
		{

			if (i != 239)
			{
				wbpro_drawLine(239 - i, 140 - temperature[i], 239 - i, 140 - temperature[i], wbBLACK, 1);
				temperature[i] = temperature[i + 1];
				wbpro_drawLine(239 - i, 140 - temperature[i], 239 - i, 140 - temperature[i], wbWHITE, 1);
				Serial.println(temperature[i]);
				wbpro_drawLine(239 - i, 290 - humidity[i], 239 - i, 290 - humidity[i], wbBLACK, 1);
				humidity[i] = humidity[i + 1];
				wbpro_drawLine(239 - i, 290 - humidity[i], 239 - i, 290 - humidity[i], wbWHITE, 1);
			}
			if (i == 239)
			{
				wbpro_drawLine(239 - i, 140 - temperature[i], 239 - i, 140 - temperature[i], wbBLACK, 1);
				wbpro_drawLine(239 - i, 290 - humidity[i], 239 - i, 290 - humidity[i], wbBLACK, 1);
				temperature[i] = (uint16_t)dht.getTemperature();
				humidity[i] = (uint16_t)dht.getHumidity();
				wbpro_drawLine(239 - i, 140 - temperature[i], 239 - i, 140 - temperature[i], wbWHITE, 1);
				wbpro_drawLine(239 - i, 290 - humidity[i], 239 - i, 290 - humidity[i], wbWHITE, 1);
			}
		}

		wbpro_fillRect(100, 150, 42, 16, wbBLACK);
		wbpro_drawString(readDHTTemperature().c_str(), 100, 150, 1, 2);
		wbpro_fillRect(100, 300, 42, 16, wbBLACK);
		wbpro_drawString(readDHTHumidity().c_str(), 100, 300, 1, 2);
		CD += 1152;
	}
}

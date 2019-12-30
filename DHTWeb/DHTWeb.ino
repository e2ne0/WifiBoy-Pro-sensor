#include <ArduinoJson.h>

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHTesp.h>
#include <wifiboypro.h>

// Replace with your network credentials
const char *ssid = "3314";
const char *password = "805801805801";

int dhtPin = 23;
DHTesp dht;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readDHTTemperature()
{
	// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
	// Read temperature as Celsius (the default)
	float t = dht.getTemperature();
	// Read temperature as Fahrenheit (isFahrenheit = true)
	//float t = dht.readTemperature(true);
	// Check if any reads failed and exit early (to try again).
	if (isnan(t))
	{
		Serial.println("Failed to read from DHT sensor!");
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
	// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
	float h = dht.getHumidity();
	if (isnan(h))
	{
		Serial.println("Failed to read from DHT sensor!");
		return "--";
	}
	else
	{
		Serial.println(h);
		return String(h);
	}
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    body {
     font-family: Arial;
     display: flex;
	 flex-direction: column;
	 justify-content: 'center';
     align-items: 'center;'
	 width: 100%;
	 height: 768px;
     text-align: centerå;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP32 DHT Server</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">%</sup>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ;
</script>
</html>)rawliteral";

// Replaces placeholder with DHT values
String processor(const String &var)
{
	//Serial.println(var);
	if (var == "TEMPERATURE")
	{
		return readDHTTemperature();
	}
	else if (var == "HUMIDITY")
	{
		return readDHTHumidity();
	}
	return String();
}
unsigned long currentTime;
unsigned long CD;
uint16_t temperature[240];
uint16_t humidity[240];
void setup()
{
	currentTime = millis();
	// Serial port for debugging purposes
	Serial.begin(115200);

	dht.setup(dhtPin, DHTesp::DHT11);

	// Connect to Wi-Fi
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(1000);
		Serial.println("Connecting to WiFi..");
	}
	for (int i = 0; i < 240; i++)
	{
		temperature[i] = 0;
		humidity[i] = 0;
	}
	// Print ESP32 Local IP Address
	Serial.println(WiFi.localIP());


	// Route for root / web page
	// server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
	// 	request->send_P(200, "application/json", index_html, processor);
	// });



	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
		char responseMessage[20];
		strcpy (responseMessage,readDHTTemperature().c_str());
  		strcat (responseMessage,", ");
  		strcat (responseMessage,readDHTHumidity().c_str());
		strcat (responseMessage,"\n");
		puts(responseMessage);

		request->send_P(200, "text/plain", responseMessage);
	});

	server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send_P(200, "text/plain", readDHTTemperature().c_str());
	});
	server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send_P(200, "text/plain", readDHTHumidity().c_str());
	});

	// Start server
	server.begin();
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
				wbpro_drawLine(239 - i, 140, 239 - i, 140 - temperature[i], wbBLACK, 1);
				temperature[i] = temperature[i + 1];
				wbpro_drawLine(239 - i, 140, 239 - i, 140 - temperature[i], wbWHITE, 1);
				Serial.println(temperature[i]);
				wbpro_drawLine(239 - i, 290, 239 - i, 290 - humidity[i], wbBLACK, 1);
				humidity[i] = humidity[i + 1];
				wbpro_drawLine(239 - i, 290, 239 - i, 290 - humidity[i], wbWHITE, 1);
			}
			if (i == 239)
			{
				wbpro_drawLine(239 - i, 140, 239 - i, 140 - temperature[i], wbBLACK, 1);
				wbpro_drawLine(239 - i, 290, 239 - i, 290 - humidity[i], wbBLACK, 1);
				temperature[i] = (uint16_t)dht.getTemperature();
				humidity[i] = (uint16_t)dht.getHumidity();
				wbpro_drawLine(239 - i, 140, 239 - i, 140 - temperature[i], wbWHITE, 1);
				wbpro_drawLine(239 - i, 290, 239 - i, 290 - humidity[i], wbWHITE, 1);
			}
		}

		wbpro_fillRect(100, 150, 42, 16, wbBLACK);
		wbpro_drawString(readDHTTemperature().c_str(), 100, 150, 1, 2);
		wbpro_fillRect(100, 300, 42, 16, wbBLACK);
		wbpro_drawString(readDHTHumidity().c_str(), 100, 300, 1, 2);
		CD += 1152;
	}
}

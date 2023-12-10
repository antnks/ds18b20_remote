#include <ESP8266WiFi.h>
#include <DallasTemperature.h>
#include <OneWire.h>

// lolin wemos d1 mini clone
#define ONE_WIRE_BUS 14
#define ONBOARD_LED  D4

const char ssid[] = "mynetwork";
const char password[] = "mypassword";
const char servername[] = "www.myserver.domain";

// poll temperature every 60 sec
int refresh = 60;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void blink_led()
{
    // on d1 mini HIGH and LOW are swapped for some reason
    pinMode(ONBOARD_LED, OUTPUT);  
    delay(500);
    digitalWrite(ONBOARD_LED,LOW);
    delay(100);
    digitalWrite(ONBOARD_LED,HIGH);
}

void setup()
{
  Serial.begin(115200);
  delay(10);

  WiFi.softAPdisconnect (true);
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    blink_led();
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  Serial.println(WiFi.localIP());
  sensors.begin();
}

void loop()
{
  sensors.requestTemperatures();
  double celsius = sensors.getTempCByIndex(0);
  Serial.print(celsius);

  WiFiClient httpclient;
  if (httpclient.connect(servername, 80))
  {
      blink_led();

      Serial.println("connected to http");
      httpclient.print("GET /receive.php?sensor=ds&temp=");
      httpclient.print(celsius);
      httpclient.println(" HTTP/1.1");
      httpclient.println("Host: www.myserver.domain");
      httpclient.println("User-Agent: arduino-robot/1.0");
      httpclient.println();
  }
    
  delay(refresh * 1000);
}

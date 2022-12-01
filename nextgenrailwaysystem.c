#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Servo.h>
#define servoPin 2
#define sens1 16
#define sens2 5
#define buzzer 13
#define WLAN_SSID "ABCDE"
#define WLAN_PASS "11111118"
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME "Group9"
#define AIO_KEY "aio_DIah63TMAZmbOECeWW4ro8Evtls0"
Servo servo1;
void beep()
{
for(int a=0;a<5;a++)
{
digitalWrite(buzzer, HIGH);
delay(100);
  digitalWrite(buzzer, LOW);
delay(100);
}
}
WiFiClient client; // Create an ESP8266 WiFiClient class to connect to the
MQTT server.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT,
AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish STATUS1 = Adafruit_MQTT_Publish(&mqtt,
AIO_USERNAME "/feeds/STATUS1");

void setup()
{
Serial.begin(115200);
delay(10);
servo1.attach(servoPin);
// mqtt.subscribe(&STATUS1);
pinMode(sens1, INPUT);
pinMode(sens2, INPUT);
pinMode(4, OUTPUT); // led
pinMode(0, OUTPUT); // led
pinMode(13, OUTPUT); // buzzer
servo1.write(0);
  Serial.println(); Serial.println(); // Connect to WiFi access point.
Serial.print("Connecting to ");
Serial.println(WLAN_SSID);
WiFi.begin(WLAN_SSID, WLAN_PASS);
while (WiFi.status() != WL_CONNECTED)
{
delay(500);
Serial.print(".");
}
Serial.println();
Serial.println("WiFi connected");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
//connect(); // connect to adafruit io
}
void MQTT_connect() // connect to adafruit via mqtt
{
Serial.print(F("connecting to Adafruit io..."));
int8_t tra;
while ((tra = mqtt.connect()) !=0)
{
switch (tra)
{
case 1: Serial.println(F("Wrong protocol")); break;
case 2: Serial.println(F("ID rejected")); break;
case 3: Serial.println(F("Server unavailable")); break;
case 4: Serial.println(F("Bad user/pass")); break;
case 5: Serial.println(F("Not authorized")); break;
case 6: Serial.println(F("Failed to Subscribe")); break;
default: Serial.println(F("Connection failed")); break;
}
if(tra >= 0)
mqtt.disconnect();
Serial.println(F("Retrying connection..."));
delay(10000);
}
Serial.println(F("Adafruit io connected"));
}
void loop()
{
if(! mqtt.ping(3))
{
//reconnect to adafruit io
if(! mqtt.connected())
MQTT_connect();
}
int i,val1,val2;
  val1 = digitalRead(sens1);
val2 = digitalRead(sens2);
digitalWrite(4, HIGH);
if (val1 == 0 || val2 == 0)
{
if (val1 == 0)
{
Serial.print("...");
if (! STATUS1.publish("Train arrived"))
{
Serial.println(F("Failed"));
}
else
{
Serial.println(F("OK!"));
}
digitalWrite(0, HIGH);
digitalWrite(4, LOW);
beep();
for (i = 0; i < 180; i += 10)
{
servo1.write(i);
delay(10);
}
while (digitalRead(sens2) == 1)
{
servo1.write(180);
}
if (digitalRead(sens2)==0)
{
  Serial.print("...");
if (! STATUS1.publish("Train departed"))
{
Serial.println(F("Failed"));
}
else
{
Serial.println(F("OK!"));
}
  digitalWrite(0, LOW);
digitalWrite(4, HIGH);
for (i = 180; i >= 0; i -= 5)
{
servo1.write(i);
delay(50);
}
while (digitalRead(sens2) == 0)
{
servo1.write(0);
}
}
}
else if (val2 == 0)
{
Serial.print("...");
if (! STATUS1.publish("Train arrived"))
{
Serial.println(F("Failed"));
}
else
{
Serial.println(F("OK!"));
}
digitalWrite(0, HIGH);
digitalWrite(4, LOW);
beep();
for (i = 0; i < 180; i += 10
)
{
servo1.write(i);
delay(10);
}
while (digitalRead(sens1) == 1)
{
servo1.write(180);
}
  if (digitalRead(sens1)==0)
{
Serial.print("...");
if (! STATUS1.publish("Train departed"))
{
Serial.println(F("Failed"));
}
else
{
Serial.println(F("OK!"));
}
digitalWrite(0, LOW);
digitalWrite(4, HIGH);
for (i = 180; i >= 0; i -= 5)
{
servo1.write(i);
delay(50);
}
while (digitalRead(sens1) == 0)
{
servo1.write(0);
}
}
}
}
}

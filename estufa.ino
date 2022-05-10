#define BLYNK_TEMPLATE_ID           "TMPLlpcex8cP"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "3iwLP4lediNiU_H4VmgkB-VlG6JNa_2h"
#define pho_PIN 2


#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHTesp.h"

const int DHT_PIN = 15;

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Wokwi-GUEST";
char pass[] = "";


DHTesp dhtSensor;
BlynkTimer timer;


BLYNK_WRITE(V0)
{

  int value = param.asInt();

  // Update state
  digitalWrite(14, value);
}


BLYNK_CONNECTED()
{

  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}


void sendSensor()
{
  TempAndHumidity  data = dhtSensor.getTempAndHumidity();
  Serial.println(data.humidity);
  if (digitalRead(pho_PIN) == LOW) {
    Blynk.virtualWrite(V7, 0);
  } else {Blynk.virtualWrite(V7, 1);}

  Blynk.virtualWrite(V5, data.humidity);
  Blynk.virtualWrite(V6, data.temperature);
}

void setup()
{
  pinMode(14, OUTPUT);
  pinMode(pho_PIN, INPUT);
  // Debug console
  Serial.begin(115200);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  Blynk.begin(auth, ssid, pass);


  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}

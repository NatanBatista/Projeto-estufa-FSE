#define BLYNK_TEMPLATE_ID           "TMPLlpcex8cP"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "3iwLP4lediNiU_H4VmgkB-VlG6JNa_2h"



#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>




char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Wokwi-GUEST";
char pass[] = "";


BlynkTimer timer;


BLYNK_WRITE(V0)
{

  int value = param.asInt();
  //Liga e Desliga led pelo Blynk
  // Update state
  digitalWrite(14, value);
}


BLYNK_CONNECTED()
{

  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}



void setup()
{
  pinMode(14, OUTPUT);
  // Debug console
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

}

void loop()
{
  Blynk.run();
  timer.run();
}

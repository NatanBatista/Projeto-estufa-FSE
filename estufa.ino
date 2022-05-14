#define BLYNK_TEMPLATE_ID           "TMPLlpcex8cP"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "3iwLP4lediNiU_H4VmgkB-VlG6JNa_2h"
#define pho_PIN 2
#define led_PIN 14
#define DHT_PIN 15
#define SERVO_PIN 32
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>     //Bliblioteca do blynk
#include "DHTesp.h"               //Bliblioteca do sensor de Temperatura e umidade
#include <ESP32Servo.h>           //Bliblioteca do servo motor


bool manual = false;
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Wokwi-GUEST";
char pass[] = "";


DHTesp dhtSensor;    //Criando os objetos
BlynkTimer timer;
Servo sv; 



BLYNK_WRITE(V0)   //Receber valores o blynk
{

  int value = param.asInt();

  // Update state
  digitalWrite(led_PIN, value);
  if (value == 0) {
    manual = false;
  } else {
    manual = true;
  }
}

BLYNK_WRITE(V8)   //Receber valores o blynk
{

  int value = param.asInt();

  // Update state
  sv.write(value);
}




void sendSensor()  //Enviar os valores dos sensores para a dashboard
{
  TempAndHumidity  data = dhtSensor.getTempAndHumidity();
  if (digitalRead(pho_PIN) == LOW) {
    Blynk.virtualWrite(V7, 0);
  } else {Blynk.virtualWrite(V7, 1);}

  segurancaTemp(data.temperature, manual);

  Blynk.virtualWrite(V5, data.humidity);
  Blynk.virtualWrite(V6, data.temperature);
  Serial.println(manual);
}

void segurancaTemp(int temp, bool manual) 
{
  if (temp > 24 && manual == false) {
    Blynk.virtualWrite(V8, 90);
    sv.write(50);
  } else if (7 < temp < 24 && manual == false) {
    Blynk.virtualWrite(V8, 0);
    sv.write(0);
  }
}


void setup()
{
  Serial.begin(115200);
  pinMode(led_PIN, OUTPUT);
  pinMode(pho_PIN, INPUT);
  
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  Blynk.begin(auth, ssid, pass);


  sv.attach(SERVO_PIN);
  sv.write(0);

  timer.setInterval(1000L, sendSensor);  //Intervalo que a função sendSensor vai inviar os valores para o dashboard
}

void loop()
{
  Blynk.run();
  timer.run();
}
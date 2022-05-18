#define BLYNK_TEMPLATE_ID           "TMPLlpcex8cP"     //Template da dashboard
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "3iwLP4lediNiU_H4VmgkB-VlG6JNa_2h"
#define pho_PIN 2  //Pino do sensor fotorresistor
#define led_PIN 14     //Pino do led
#define DHT_PIN 15     //Pino do sensor de temperatura e umidade
#define SERVO_TEMP_PIN 32   //Pino do servo motor que move a lona
#define LED_TEMP_PIN 4
#define LED_UMI_PIN 33      //Pino do led que indica que o umidificador de ar está ativo
#define LED_UMI1_PIN 26     //Pino de led que indica que o Desumidificador está ativo
#define SERVO_photo_PIN 25    //Pino que abre a valvula de água
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>     //Bliblioteca do blynk
#include "DHTesp.h"               //Bliblioteca do sensor de Temperatura e umidade
#include <ESP32Servo.h>           //Bliblioteca do servo motor


bool manual = false;
int temp_maxi = 24;
int temp_min = 7;
int umidade_min = 60;
int umidade_maxi = 80;
int aux = 0;
int aux1 = 0;
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Wokwi-GUEST";
char pass[] = "";


DHTesp dhtSensor;    //Criando os objetos
BlynkTimer timer;
Servo sv1;
Servo sv2;



BLYNK_WRITE(V0)   //Receber valores o blynk
{

  int value = param.asInt();

  digitalWrite(led_PIN, value);
  if (value == 0) {
    manual = false;
  } else {
    manual = true;
  }
}

BLYNK_WRITE(V1)   //Receber valores o blynk
{

  int value = param.asInt();

  digitalWrite(LED_TEMP_PIN, value);
}

BLYNK_WRITE(V8)   //Receber valores o blynk
{

  int value = param.asInt();

  sv1.write(value); //Abre a lona conforme o slide da dashboard
}

BLYNK_WRITE(V9)   //Receber valores o blynk
{

  int value = param.asInt();

  temp_maxi = value;  //Define a temperatura maxima conforme o slide da dashboard
}

BLYNK_WRITE(V10)   //Receber valores o blynk
{

  int value = param.asInt();

  temp_min = value;  //Define a temperatura mínima conforme o slide da dashboard
}

BLYNK_WRITE(V11)   //Receber valores o blynk
{

  int value = param.asInt();

  umidade_min = value;  //Define a umidade minima
}

BLYNK_WRITE(V12)   //Receber valores o blynk
{

  int value = param.asInt();

  umidade_maxi = value;  //Define a umidade minima
}

BLYNK_WRITE(V13)   //Receber valores o blynk
{

  int value = param.asInt();

  digitalWrite(LED_UMI_PIN, value);
}

BLYNK_WRITE(V14)   //Receber valores o blynk
{

  int value = param.asInt();

  digitalWrite(LED_UMI1_PIN, value);
}

void photoSensor()
{
  if (digitalRead(pho_PIN) == HIGH && aux == aux1) {
    sv2.write(180);
    Blynk.virtualWrite(V7, 1);
    aux = 1;
    delay(5000);
    sv2.write(0);
  } else if (digitalRead(pho_PIN) == LOW && aux != aux1) {
    Blynk.virtualWrite(V7, 0);
    aux = 0;
    }
}

void sendSensor()  //Enviar os valores dos sensores para a dashboard
{
  TempAndHumidity  data = dhtSensor.getTempAndHumidity();


  if (manual == 0) {
    segurancaTemp(data.temperature, temp_maxi);
    segurancaUmi(data.humidity, umidade_min);
  }


  Blynk.virtualWrite(V5, data.humidity);
  Blynk.virtualWrite(V6, data.temperature);
  Serial.println(manual);
}

void segurancaTemp(int temp, int temp_maxi) //Caso a temperatura esteja no maximo definido, abre a lona
{
  if (temp > temp_maxi * 1.1) {
    Blynk.virtualWrite(V1, 0);
    Blynk.virtualWrite(V8, 180);
    digitalWrite(LED_TEMP_PIN, LOW);
    sv1.write(180);
  } else if (temp > temp_maxi) {
    Blynk.virtualWrite(V1, 0);
    Blynk.virtualWrite(V8, 90);
    digitalWrite(LED_TEMP_PIN, LOW);
    sv1.write(90);
  } else if (temp < temp_min) {
    Blynk.virtualWrite(V1, 1);
    Blynk.virtualWrite(V8, 0);
    digitalWrite(LED_TEMP_PIN, HIGH);
    sv1.write(0);
  } else {
    Blynk.virtualWrite(V1, 0);
    Blynk.virtualWrite(V8, 0);
    digitalWrite(LED_TEMP_PIN, LOW);
    sv1.write(0);
  }
}

void segurancaUmi(int umi, int umidade_min) //Caso a umidade esteja muito abaixo da definida, ligar o umidificador
{
  if (umi < umidade_min) {
    digitalWrite(LED_UMI_PIN, HIGH);
    digitalWrite(LED_UMI1_PIN, LOW);
    Blynk.virtualWrite(V13, 1);
    Blynk.virtualWrite(V14, 0);
  } else if (umi > umidade_maxi) {
    digitalWrite(LED_UMI1_PIN, HIGH);
    digitalWrite(LED_UMI_PIN, LOW);
    Blynk.virtualWrite(V13, 0);
    Blynk.virtualWrite(V14, 1);
  } else {
    digitalWrite(LED_UMI_PIN, LOW);
    digitalWrite(LED_UMI1_PIN, LOW);
    Blynk.virtualWrite(V13, 0);
    Blynk.virtualWrite(V14, 0);
  }
}


void setup()
{
  Serial.begin(115200);
  pinMode(led_PIN, OUTPUT);
  pinMode(pho_PIN, INPUT);
  pinMode(LED_TEMP_PIN, OUTPUT);
  pinMode(LED_UMI_PIN, OUTPUT);
  pinMode(LED_UMI1_PIN, OUTPUT);

  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  Blynk.begin(auth, ssid, pass);


  sv1.attach(SERVO_TEMP_PIN);
  sv2.attach(SERVO_photo_PIN);
  sv1.write(0);
  sv2.write(0);

  timer.setInterval(1000L, sendSensor);  //Intervalo que a função sendSensor vai inviar os valores para o dashboard
  timer.setInterval(1000L, photoSensor);

}

void loop()
{
  Blynk.run();
  timer.run();
}

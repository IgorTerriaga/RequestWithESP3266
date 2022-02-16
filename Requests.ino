#include <Ultrasonic.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <string.h>

#define INTERVALO 1000

#define LED_CARRO1 21
#define TRIGGER_CARRO1 23
#define ECHO_CARRO1 22

#define LED_CARRO2 4
#define TRIGGER_CARRO2 2
#define ECHO_CARRO2 15

#define LED_CARRO3 5
#define TRIGGER_CARRO3 19
#define ECHO_CARRO3 18

Ultrasonic ultrasonicCarro1(TRIGGER_CARRO1, ECHO_CARRO1);
Ultrasonic ultrasonicCarro2(TRIGGER_CARRO2, ECHO_CARRO2);
Ultrasonic ultrasonicCarro3(TRIGGER_CARRO3, ECHO_CARRO3);

unsigned int distanciaCarro1 = 0;
unsigned int distanciaCarro2 = 0;
unsigned int distanciaCarro3 = 0;

unsigned int mudancaCarro1 = 0;
unsigned int mudancaCarro2 = 0;
unsigned int mudancaCarro3 = 0;

char idVaga1[] = "1a82c1fc-b2c7-45d9-8dd4-707f176476c9"; // carrinho Amarelo - Led Amarelo
char idVaga2[] = "7fc6db75-58f1-4299-ae95-c28c85bbed3d"; // carrinho Verde - Led Verde
char idVaga3[] = "a756f3f9-ec9a-44d6-81df-816971280e9e"; // carrinho Azul - Led Azul

const char* ssid = "IGOR_JAQUELINE";
const char* password = "#vitoria1q2w";

char serverNameOcupar[] = "http://192.168.31.154:5000/informar_ocupacao_vaga/";
char serverNameDesocupar[] = "http://192.168.31.154:5000/informar_desocupacao_vaga/";

void configuration() {
  pinMode(LED_CARRO1, OUTPUT);
  pinMode(LED_CARRO2, OUTPUT);
  pinMode(LED_CARRO3, OUTPUT);
}

void connectWifi() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

}

void setup() {
  Serial.begin(115200);
  configuration();
  connectWifi();
}

int getDistance(Ultrasonic ultrasonic) {
  int distanciaCM;
  long microsec = ultrasonic.timing();
  distanciaCM = ultrasonic.convert(microsec, Ultrasonic::CM);
  return distanciaCM;
}

void postApi(char server[], char idVaga[]) {
  char buffer[250];
  strcat(strcpy(buffer, server), idVaga);

  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    http.begin(client, buffer);

    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST("");

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}

void loop() {
  distanciaCarro1 = getDistance(ultrasonicCarro1);

  Serial.print("Carro 1: ");
  Serial.print(distanciaCarro1);
  Serial.print("\n");

  if (distanciaCarro1 <= 3) {
    if (mudancaCarro1 == 0) {
      digitalWrite(LED_CARRO1, HIGH);
      postApi(serverNameOcupar, idVaga1);
    }
    mudancaCarro1 = 1;
  }
  else {
    if (mudancaCarro1 == 1) {
      digitalWrite(LED_CARRO1, LOW);
      postApi(serverNameDesocupar, idVaga1);
    }
    mudancaCarro1 = 0;

  }

  delay(INTERVALO);
  distanciaCarro2 = getDistance(ultrasonicCarro2);

  Serial.print("Carro 2: ");
  Serial.print(distanciaCarro2);
  Serial.print("\n");

  if (distanciaCarro2 <= 3) {
    if (mudancaCarro2 == 0) {
      digitalWrite(LED_CARRO2, HIGH);
      postApi(serverNameOcupar, idVaga2);
    }
    mudancaCarro2 = 1;
  }
  else {
    if (mudancaCarro2 == 1) {
      digitalWrite(LED_CARRO2, LOW);
      postApi(serverNameDesocupar, idVaga2);
    }
    mudancaCarro2 = 0;
  }

  delay(INTERVALO);
  distanciaCarro3 = getDistance(ultrasonicCarro3);

  Serial.print("Carro 3: ");
  Serial.print(distanciaCarro3);
  Serial.print("\n");
  if (distanciaCarro3 <= 3) {
    if (mudancaCarro3 == 0) {
      digitalWrite(LED_CARRO3, HIGH);
      postApi(serverNameOcupar, idVaga3);
    }
    mudancaCarro3 = 1;
  }
  else {
    if (mudancaCarro3 == 1) {
      digitalWrite(LED_CARRO3, LOW);
      postApi(serverNameDesocupar, idVaga3);
    }
    mudancaCarro3 = 0;
  }
  delay(INTERVALO);
}

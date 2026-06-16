#include <WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>
#include <ArduinoJson.h>

// =========================
// Wi-Fi
// =========================
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// =========================
// MQTT
// =========================
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_topic = "careplus/healthtracker/esp32_001";

// =========================
// Pinos
// =========================
const int DHT_PIN = 15;
const int POT_PIN = 34;
const int BUTTON_PIN = 27;

// =========================
// Objetos
// =========================
WiFiClient espClient;
PubSubClient client(espClient);
DHTesp dhtSensor;

// =========================
// Variáveis
// =========================
int steps = 0;
bool lastButtonState = HIGH;

unsigned long lastPublish = 0;
const unsigned long publishInterval = 3000;

// =========================
// Conexão Wi-Fi
// =========================
void connectWiFi() {
  Serial.print("Conectando ao WiFi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// =========================
// Conexão MQTT
// =========================
void connectMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT... ");

    String clientId = "careplus-healthtracker-";
    clientId += String(random(1000, 9999));

    if (client.connect(clientId.c_str())) {
      Serial.println("conectado!");
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 2 segundos...");
      delay(2000);
    }
  }
}

// =========================
// Leitura do batimento simulado
// =========================
int readHeartRate() {
  int analogValue = analogRead(POT_PIN);
  int heartRate = map(analogValue, 0, 4095, 60, 180);

  return heartRate;
}

// =========================
// Contador de passos simulado
// =========================
void countSteps() {
  bool currentButtonState = digitalRead(BUTTON_PIN);

  if (lastButtonState == HIGH && currentButtonState == LOW) {
    steps++;

    Serial.print("Passo detectado! Total: ");
    Serial.println(steps);

    delay(200);
  }

  lastButtonState = currentButtonState;
}

// =========================
// Publicação MQTT
// =========================
void publishData(float temperature, float humidity, int heartRate, int stepCount) {
  StaticJsonDocument<256> doc;

  doc["deviceId"] = "healthtracker_001";
  doc["type"] = "HealthTracker";
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["heartRate"] = heartRate;
  doc["steps"] = stepCount;
  doc["timestamp"] = millis();

  char buffer[256];
  serializeJson(doc, buffer);

  Serial.print("Publicando no tópico ");
  Serial.print(mqtt_topic);
  Serial.print(": ");
  Serial.println(buffer);

  client.publish(mqtt_topic, buffer);
}

// =========================
// Setup
// =========================
void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);

  connectWiFi();

  client.setServer(mqtt_server, mqtt_port);

  randomSeed(micros());

  Serial.println("Sistema iniciado.");
}

// =========================
// Loop principal
// =========================
void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  if (!client.connected()) {
    connectMQTT();
  }

  client.loop();

  countSteps();

  if (millis() - lastPublish >= publishInterval) {
    lastPublish = millis();

    TempAndHumidity data = dhtSensor.getTempAndHumidity();

    if (isnan(data.temperature) || isnan(data.humidity)) {
      Serial.println("Erro ao ler o DHT22");
      return;
    }

    float temperature = data.temperature;
    float humidity = data.humidity;
    int heartRate = readHeartRate();

    Serial.println("===== LEITURA ATUAL =====");
    Serial.print("Temperatura: ");
    Serial.println(temperature);
    Serial.print("Umidade: ");
    Serial.println(humidity);
    Serial.print("Batimentos simulados: ");
    Serial.println(heartRate);
    Serial.print("Passos: ");
    Serial.println(steps);
    Serial.println("=========================");

    publishData(temperature, humidity, heartRate, steps);
  }
}
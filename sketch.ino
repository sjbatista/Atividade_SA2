#include <DHTesp.h>
#include <WiFi.h>
#include <ThingSpeak.h>

// Dados da rede Wi-Fi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Dados do ThingSpeak
const unsigned long channelID = 2141743;
const char* apiKey = "KSHQ91CHHLU94K1R";

DHTesp dhtSensor;

void setup() {
  Serial.begin(115200);
  Serial.println("Inicializando Sistema");

  // Conectando à rede Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }

  // Inicializando o sensor DHT
  dhtSensor.setup(13, DHTesp::DHT22);
  pinMode(23, OUTPUT);
  pinMode(18, OUTPUT);
  Serial.println("Finalizando Sistema");
}

void loop() {
  // Verificando a conexão Wi-Fi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Conexão WiFi perdida. Tentando reconectar...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Conectando ao WiFi...");
    }
  }

  float temperatura = dhtSensor.getTemperature();
  float humidade = dhtSensor.getHumidity();

  Serial.println("Temperatura: " + String(temperatura) + "C");
  Serial.println("Humidade: " + String(humidade) + "%");
  Serial.println("--------------------------------------------");

  // Enviando dados para o ThingSpeak
  WiFiClient client;
  ThingSpeak.begin(client);
  ThingSpeak.setField(1, temperatura);
  ThingSpeak.setField(2, humidade);
  int response = ThingSpeak.writeFields(channelID, apiKey);

  if (response == 200) {
    Serial.println("Dados enviados com sucesso para o ThingSpeak!");
  } else {
    Serial.println("Erro ao enviar dados para o ThingSpeak. Código de resposta HTTP: " + String(response));
  }

  delay(3000);

  digitalWrite(23, HIGH);
  digitalWrite(18, HIGH);
  delay(1000);

  digitalWrite(23, LOW);
  digitalWrite(18, LOW);
  delay(1000);
}

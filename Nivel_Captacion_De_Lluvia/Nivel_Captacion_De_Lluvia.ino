#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <NewPing.h>  // Librería para el sensor ultrasónico

#define DHTPIN 15          // Pin donde está conectado el DHT11
#define DHTTYPE DHT11      // Tipo de sensor DHT
#define FLOW_PIN 13        // Pin del sensor de flujo de agua
#define TRIG_PIN 12        // Pin TRIG del sensor HC-SR04
#define ECHO_PIN 14        // Pin ECHO del sensor HC-SR04
#define PUMP_PIN 2         // Pin del transistor para la bomba de agua

// Variables para envio de mensajes
unsigned long lastMsg = 0; // Contador de tiempo mensajes
#define MSG_BUFFER_SIZE  (50) // Buffer para envío de mensajes
char msg[MSG_BUFFER_SIZE]; // Variable para conversión de mensaje

// Manejo de Leds
int statusLedPin = 33;  // Esta variable controla el led de status
int flashLedPin = 4; // Esta variable controla el led flash
bool statusLed = 0;// Bandera que me dice si el led está encendido o apagado

// Variables para el manejo de tiempo
double timeLast, timeNow; // Variables para el control de tiempo no bloqueante
long lastReconnectAttempt = 0; // Variable para el conteo de tiempo entre intentos de reconexión
double WAIT_MSG = 15000;  // Espera de 1 segundo entre mensajes
double RECONNECT_WAIT = 5000; // Espera de 5 segundos entre conexiones

const char* ssid = "TP-Link_4A3C";
const char* password = "01540246";
const char* mqtt_server = "broker.hivemq.com";
// Temas MQTT
const char* mqttMsg = "codigoIoT/esp32/msg";
const char* mqttCallback = "codigoIoT/esp32/callback";
const char* mqttSensor = "codigoIoT/mor/NivelCaptacionLluvia/Sensor";
//-------------------------------------------------------------------------------------------------------------------//
WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);
NewPing sonar(TRIG_PIN, ECHO_PIN, 200);  // Configura el sensor HC-SR04 (200 cm máximo)

volatile int flow_frequency = 0;
unsigned long oldTime = 0;
float calibrationFactor = 4.5; // Factor de calibración para el sensor de flujo de agua
//------------------------------------------------------------------------------------------------------------------//
void IRAM_ATTR flow() {
  flow_frequency++;
}
//------------------------------------------------------------------------------------------------------------------//
// Función para conectarse a WiFi
void setup_wifi() {
  delay(10);
  
  // Mensajes de intento de conexión
  Serial.println();
  Serial.print("Conectando a: ");
  Serial.println(ssid);

  // Funciones de conexión
  WiFi.mode(WIFI_STA); // STA inicia el micro controlador como cliente, para accespoint se usa WIFI_AP
  WiFi.begin(ssid, password); // Proporcionar características de red

  // Secuencia que hace parpadear los leds durante el intento de conexión. Logica Inversa
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite (statusLedPin, HIGH); // Apagar LED
    delay(500); // Dado que es de suma importancia esperar a la conexión, debe usarse espera bloqueante
    digitalWrite (statusLedPin, LOW); // Encender LED
    Serial.print(".");  // Indicador de progreso
    delay (5); // Espera asimetrica para dar la sensación de parpadeo al led
  }

  // Esta función mejora los resultados de las funciones aleatorias y toma como semilla el contador de tiempo
  randomSeed(micros());

  // Mensajes de confirmación
  Serial.println("");
  Serial.println("Conectado a WiFi");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

// Función Callback, Se ejecuta al recibir un mensaje
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido en [");
  Serial.print(topic);
  Serial.print("] ");

  // Esta secuencia imprime y construye el mensaje recibido
  String messageTemp;
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]); // Imprime
    messageTemp += (char)payload[i]; // Construye el mensaje en una variable String
  }
  Serial.println();

  // Se comprueba que el mensaje se haya concatenado correctamente
  Serial.println();
  Serial.print ("Mensaje concatenado en una sola variable: ");
  Serial.println (messageTemp);
  
  // Enciende/apaga el LED Flash al recibir un mensaje.
  if(messageTemp == "true") {
    Serial.println("Led encendido");
    digitalWrite(flashLedPin, HIGH);
  }
  else if(messageTemp == "false") {
    Serial.println("Led apagado");
    digitalWrite(flashLedPin, LOW);
  }
}

// Función de reconexión. Devuelve valor booleano para indicar éxito o falla
boolean reconnect() {
  Serial.print("Intentando conexión MQTT...");
  // Generar un Client ID aleatorio
  String clientId = "ESP32Client-";
  clientId += String(random(0xffff), HEX);
  // Intentar conexión
  if (client.connect(clientId.c_str())) {
    // Una vez conectado, dar retroalimentación
    client.publish(mqttMsg,"Conectado");
    // Función de suscripción
    client.subscribe(mqttCallback);
  } else {
      // En caso de error
      Serial.print("Error rc=");
      Serial.print(client.state());
      Serial.println(" Intentando de nuevo");
      // Esperar 1 segundo
      delay(1000);
    }
  return client.connected();
}
//-------------------------------------------------------------------------------------//
void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(FLOW_PIN, INPUT);
  pinMode(PUMP_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(FLOW_PIN), flow, RISING);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // Iniciar secuencias de tiempo
  timeLast = millis (); // Inicia el control de tiempo
  lastReconnectAttempt = timeLast; // Control de secuencias de tiempo
}
//----------------------------------------------------------------------------------------------------//
void loop() {
  timeNow = millis();

  if (!client.connected()) {
    if (timeNow - lastReconnectAttempt > RECONNECT_WAIT) {
      if (reconnect()) {
        lastReconnectAttempt = timeNow;
      }
    }
  } else {
    client.loop();
  }

  // Declarar la variable flowRate fuera del bloque if para que esté disponible en todo el ámbito
  float flowRate = 0.0;

  if (timeNow - timeLast > WAIT_MSG && client.connected()) {
    timeLast = timeNow;

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    unsigned int waterDistance = sonar.ping_cm();
    int waterLevel = map(waterDistance, 0, 30, 100, 0);

    if ((millis() - oldTime) > 1000) {
      detachInterrupt(digitalPinToInterrupt(FLOW_PIN));
      flowRate = ((1000.0 / (millis() - oldTime)) * flow_frequency) / calibrationFactor;
      oldTime = millis();
      flow_frequency = 0;
      attachInterrupt(digitalPinToInterrupt(FLOW_PIN), flow, RISING);

      if (waterLevel >= 90) {
        digitalWrite(PUMP_PIN, HIGH);
        delay(10000);
      } else {
        digitalWrite(PUMP_PIN, LOW);
      }
    }

    String json = "{\"ID\":\"johanz\",\"temp\":"+String(t)+",\"hum\":"+String(h)+",\"nivel\":"+String(waterLevel)+",\"flujo\":"+String(flowRate)+"}";
    Serial.println(json);
    int str_len = json.length() + 1;
    char char_array[str_len];
    json.toCharArray(char_array, str_len);
    client.publish(mqttSensor, char_array);

    Serial.println("-------------------------");
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println("°C");
    Serial.print("Humedad: ");
    Serial.print(h);
    Serial.println("%");
    Serial.print("Nivel de Agua: ");
    Serial.println(waterLevel);
    Serial.print("Bomba de agua: ");
    Serial.println(digitalRead(PUMP_PIN));
    Serial.print("Flujo de Agua: ");
    Serial.println(flowRate);
    Serial.println("-------------------------");
  }
}

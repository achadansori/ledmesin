/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

#include <WiFi.h>
#include <PubSubClient.h>


const int LEDPIN1 = 22;
const int LEDPIN2 = 23;
const int PushButton = 15;
unsigned long buttonPressTime = 0;
bool led2State = LOW;

// Replace the next variables with your SSID/Password combination
const char* ssid = "Redmi4x";
const char* password = "29292929";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "192.168.43.153";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  Serial.begin(115200);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(LEDPIN1, OUTPUT);
  pinMode(LEDPIN2, OUTPUT);
  pinMode(PushButton, INPUT);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("node2/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  int Push_button_state = digitalRead(PushButton);

  if (Push_button_state == HIGH) {
    // Tombol ditekan
    buttonPressTime = millis(); // Ambil waktu tombol ditekan
    digitalWrite(LEDPIN1, HIGH);
    client.publish("node2/on", "MESIN NYALA");
    client.publish("node2/off", "");
    digitalWrite(LEDPIN2, LOW);
    client.publish("node2/on", "");
  } else {
    // Tombol tidak ditekan
    digitalWrite(LEDPIN1, LOW); // Nyalakan LED pertama
    if (millis() - buttonPressTime > 5000) {
      // Jika tombol telah ditekan lebih dari 5 detik
      led2State = HIGH; // Nyalakan LED kedua
      client.publish("node2/off", "MESIN MATI");
      client.publish("node2/on", "");
    } else {
      led2State = LOW; // Matikan LED kedua
      client.publish("node2/off", "");
    }
    digitalWrite(LEDPIN2, led2State); // Update status LED kedua
  }
}


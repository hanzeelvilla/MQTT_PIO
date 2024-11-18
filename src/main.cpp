#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"

WiFiClient espClient;
PubSubClient client(espClient);

long currenTime, lasTime;

void setupWifi() {
  delay(1000);
  Serial.print("Connecting to: ");
  Serial.println(SSID);

  WiFi.begin(SSID, PSWD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.print("\nConnected to: ");
  Serial.println(SSID);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  if (!client.connected()) {
    while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");

      if (client.connect("ESP32Client")) { // change the client ID to something unique
        Serial.print(" Connected to: ");
        Serial.print(BROKER);
        client.subscribe(RX_TOPIC);
        Serial.print(" Subscribed to: ");
        Serial.println(RX_TOPIC);
      } 
      else {
        Serial.print(" failed, rc= ");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        delay(5000);
      }
    }
  }

  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.print(topic);

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);

  setupWifi();
  client.setServer(BROKER, PORT);
  client.setCallback(callback);
}

void loop() {
  delay(10);
  reconnect();

  currenTime = millis();
  if (currenTime - lasTime > 5000) {
    lasTime = currenTime;

    client.publish(TX_TOPIC, "Hola nena");
    Serial.println("Publishing message...");
  }
}
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClientSecure.h>
#define ARDUINOJSON_DECODE_UNICODE 1
#include <ArduinoJson.h>  // v6.9.1

// WiFi credentials
#define WIFI_SSID  "Salz"
#define WIFI_PASS  "**********"

// Pins that are connected to L293d motor driver, simply 
// swap them if the motor turns in the wrong direction.
#define MOTOR_PIN1 D3
#define MOTOR_PIN2 D4

// Trigger on any payment to this stellar account
#define ACCOUNT_ID    "GCANDY5MQCDKPG6QPS3JAHZL5QO276N53PMKBK4WDGIJ5JDSAP7OYBGY"

// Stellar horizon server settings
#define HORIZON_HOST  "horizon.stellar.org"
#define HORIZON_PORT  443

// Fingerprint of horizon.stellar.org, will expire in august 2019
const char HORIZON_SSL_FINGERPRINT[] PROGMEM = "F5 8A BC A7 32 CA 75 5A ED E9 A3 ED C2 AF B3 86 F6 C1 23 F2";

// Wether to verify SSL fingerprint. Set to false since our security needs for some candy
// are pretty low, especially with respect to the hassle updating the fingerprint again in august.
#define VERIFY_SSL false

// Cursor for horizon endpoint. Starts with "now"
char pagingToken[20] = "now";

ESP8266WiFiMulti WiFiMulti;
WiFiClientSecure client;

void setup() {
    Serial.begin(115200);
    //Serial.setDebugOutput(true);

    WiFiMulti.addAP(WIFI_SSID, WIFI_PASS);

    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.println("Not connected to WiFi...");
        delay(1000);
    }

    Serial.printf("Connected to WiFi: %s\n", WiFi.SSID().c_str());

  if (VERIFY_SSL) {
    client.setFingerprint(HORIZON_SSL_FINGERPRINT);
  } else {
    client.setInsecure();
  }
  
  pinMode(MOTOR_PIN1, OUTPUT);
  pinMode(MOTOR_PIN2, OUTPUT); 
  digitalWrite(MOTOR_PIN1, LOW);
  digitalWrite(MOTOR_PIN2, LOW);
}

void loop() {
  processHorizonStream();
}

void processHorizonStream() {
  
  connectToHorizon();
  
  while (client.available()) {
    String line = client.readStringUntil('\n');
    if (line.startsWith("data: {")) {

      const size_t capacity = 5*JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(15) + 2090;
      DynamicJsonDocument doc(capacity);
      auto error = deserializeJson(doc, line.substring(6));
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
      }

      strcpy(pagingToken, doc["paging_token"]);
      const char* from = doc["from"];
      const char* to = doc["to"];
      const char* amount = doc["amount"];
            
      const char* asset_code;
      const char* asset_issuer;
      if (strcmp(doc["asset_type"], "native") == 0) {
        asset_code = "XLM";
        asset_issuer = "native";
      } else {
        asset_code = doc["asset_code"];
        asset_issuer = doc["asset_issuer"];
      }

      Serial.println( String("Received ") + amount + " " + asset_code + " from " + from );
      if (strcmp(to, ACCOUNT_ID) == 0) {
        dispenseCandy();
      }
    }
  }
}

void connectToHorizon() {
  if (!client.connected()) {
    if (!client.connect(HORIZON_HOST, HORIZON_PORT)) {
      Serial.println( String("Connection to ") + HORIZON_HOST + " failed");
      delay(1000);
      return;
    }
    Serial.println( String("Connected to https://") + HORIZON_HOST + "/accounts/" + ACCOUNT_ID + "/payments?cursor=" + pagingToken );
    client.print(
      String("GET /accounts/") + ACCOUNT_ID + "/payments?cursor=" + pagingToken + " HTTP/1.0\r\n" +
      "Host: " + HORIZON_HOST + "\r\n" +
      "Accept: text/event-stream\r\n\r\n"
    );    
  }  
}

void dispenseCandy() {
  digitalWrite(MOTOR_PIN1, HIGH);
  delay(1200);
  digitalWrite(MOTOR_PIN1, LOW);
}


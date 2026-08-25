/// Hardware used: ESP32_devkit_v1
/// LED connected to GPIO12 and GND

#define ENABLE_USER_AUTH
#define ENABLE_DATABASE
#define ENABLE_ESP_SSLCLIENT

#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseClient.h>
#include <ESP_SSLClient.h>

// Network and Firebase credentials
#define WIFI_SSID "FAHAD"
#define WIFI_PASSWORD "fahad#1998"

#define Web_API_KEY "AIzaSyC7LAYI7CK7THtQU7PSksB3tEfH3WhlSrA"
#define DATABASE_URL "https://led-esp32-b6eec-default-rtdb.asia-southeast1.firebasedatabase.app"
#define USER_EMAIL "fahadchina20231@gmail.com"
#define USER_PASS "fahad123"

#define PROXY_HOST "192.168.2.3"
#define PROXY_PORT 7890

void processData(AsyncResult &aResult);

// Authentication
UserAuth user_auth(Web_API_KEY, USER_EMAIL, USER_PASS);

class ProxyWiFiClient : public WiFiClient {
public:
  int connect(const char *host, uint16_t port, int32_t timeout) override {
    if (!WiFiClient::connect(PROXY_HOST, PROXY_PORT, timeout))
      return 0;

    // Increased timeout from 10ms to 2000ms so proxy response headers aren't missed
    setTimeout(2000); 

    print("CONNECT ");
    print(host);
    print(":");
    print(port);
    print(" HTTP/1.1\r\nHost: ");
    print(host);
    print(":");
    print(port);
    print("\r\n\r\n");

    String statusLine = readStringUntil('\n');
    if (statusLine.indexOf(" 200 ") < 0) {
      stop();
      return 0;
    }

    while (connected()) {
      String header = readStringUntil('\n');
      if (header == "\r" || header.length() == 0)
        break;
    }
    return connected() ? 1 : 0;
  }

  int connect(const char *host, uint16_t port) override {
    return connect(host, port, 10000);
  }
};

ProxyWiFiClient proxy_client, stream_proxy_client;
ESP_SSLClient ssl_client, stream_ssl_client;

FirebaseApp app;
AsyncClientClass aClient(ssl_client), streamClient(stream_ssl_client);
RealtimeDatabase Database;

const int LED_PIN = 12;
const char *listenerPath = "board1/outputs/digital/12";

void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println("\nWiFi Connected! IP: ");
  Serial.println(WiFi.localIP());
}

void setup(){
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  initWiFi();

  ssl_client.setClient(&proxy_client);
  stream_ssl_client.setClient(&stream_proxy_client);
  ssl_client.setInsecure();
  stream_ssl_client.setInsecure();
  ssl_client.setHandshakeTimeout(10);
  stream_ssl_client.setHandshakeTimeout(10);

  // Initialize Firebase
  initializeApp(aClient, app, getAuth(user_auth), processData, "authTask");
  app.getApp<RealtimeDatabase>(Database);
  Database.url(DATABASE_URL);

  streamClient.setSSEFilters("get,put,patch,keep-alive,cancel,auth_revoked");
  Database.get(streamClient, listenerPath, processData, true, "streamTask");
}

void loop(){
  app.loop();
}

void processData(AsyncResult &aResult){
  if (!aResult.isResult())
    return;

  if (aResult.isError()) {
    Firebase.printf("Error task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.error().message().c_str(), aResult.error().code());
  }

  if (aResult.available()) {
    RealtimeDatabaseResult &result = aResult.to<RealtimeDatabaseResult>();
    
    // Convert the database result to a String directly
    String val = result.to<String>();
    val.trim();

    Serial.print("Data event: ");
    Serial.println(val);

    if (val == "1" || val == "true") {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("LED turned ON");
    } else if (val == "0" || val == "false") {
      digitalWrite(LED_PIN, LOW);
      Serial.println("LED turned OFF");
    }
  }
}
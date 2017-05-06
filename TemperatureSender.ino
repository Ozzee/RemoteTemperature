/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>
#include <DallasTemperature.h>
#include <OneWire.h>


#define INTERVAL 1 //seconds

// Set up OneWire and Dallas
#define ONE_WIRE_BUS 2 // PIN2 = D4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

const char* ssid     = "Futurice-Code-Camp";
const char* password = "isitfriday";

const char* host = "192.168.8.143";


void setup() {
  Serial.begin(9600);
  delay(10);

  pinMode(LED_BUILTIN, OUTPUT);

  sensors.begin();

  // We start by connecting to a WiFi network

  Serial.println();
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

int sensorValue = 0;

void loop() {
  delay(INTERVAL*1000);
  digitalWrite(LED_BUILTIN, HIGH);
  
  sensors.requestTemperatures();
  sensorValue = sensors.getTempCByIndex(0);
  Serial.println("Temp: " + String(sensorValue));

  Serial.println("Connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/?temp=" + String(sensorValue);
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
  digitalWrite(LED_BUILTIN, LOW); 
}


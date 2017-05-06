/* 
 * Show the temperature by setting a servo to a certain position
 */

#include <Servo.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid     = "Futurice-Code-Camp";
const char* password = "isitfriday";

ESP8266WebServer server(80);

Servo temperatureServo;

int pos = 0;    // Servo position

void handleRoot() {
  digitalWrite(LED_BUILTIN, HIGH);
  server.send(200, "text/plain", "OK");
  int value = server.arg(0).toInt() * (180/30);
  if(value < 0) {
    value = 0; 
  } else if (value > 180) {
    value = 180;
  }
  
  temperatureServo.write(180-value);
  digitalWrite(LED_BUILTIN, LOW);
}

void handleNotFound(){
  digitalWrite(LED_BUILTIN, HIGH);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(LED_BUILTIN, HIGH);
}


void setup() {
  Serial.begin(9600);
  delay(10);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  temperatureServo.attach(5); // PIN5
  temperatureServo.write(0);

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

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

}


void loop() {
  server.handleClient();
}


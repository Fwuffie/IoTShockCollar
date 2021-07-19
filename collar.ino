#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <EEPROM.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

ESP8266WebServer server(80);
IPAddress apIP(172, 217, 28, 1);
DNSServer dnsServer;

void wifiConnect() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(getAPNAME(), getPass());
  Serial.println("");

  int timeout = millis() + 10000;
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() > timeout){
      Serial.println("Failed To Connect To Wifi");
      Serial.println(getAPNAME());
      Serial.println(getPass());
      
      localAP();
      return;
    }
    delay(500);
    Serial.print(".");
  }

  //Print Details
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(getAPNAME());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
}

void localAP() {
  Serial.println("Switching to AP Mode");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("TransmitterAP");

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(53, "*", apIP);
}

void setup(void) {
  Serial.begin(115200);
  EEPROM.begin(512);

  //Try Connect To Wifi If Details Available
  wifiConnect();

  //Start Server
  server.on("/", handleRoot);
  server.on("/wifi", handleWifi);
  server.on("/wifisubmit", handleWifiSubmit);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  if(WiFi.status() != WL_CONNECTED){
    dnsServer.processNextRequest();
  }
  server.handleClient();
  MDNS.update();
}

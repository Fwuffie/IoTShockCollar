void handleRoot() {
  if (server.args() != 5) {
    server.send(400, "text/plain", "Error");
    return;
  }
  
  server.send(200, "text/plain", "Signal Transmitted");
  Serial.println("Sending Transmission");
  transmitSignal(server.arg(0).toInt(), server.arg(1).toInt(), server.arg(2).toInt(), server.arg(3).toInt(), server.arg(4).toInt());
}

void handleLocalInput() {
  String html = "";
}


void handleWifi() {
  String html = "<form action='/wifisubmit'>SSID: <input name='ssid' type='text'><br>Password: <input name='pass' type='password'><br><input type='submit'></form>";
  server.send(200, "text/html", html);
}

void handleWifiSubmit() {
  writeToEEPROM(server.arg("ssid"), server.arg("pass"));
  String html = "Updated Wifi Data, Reloading Please Wait";
  server.send(200, "text/plain", html);
  wifiConnect();
}

void handleNotFound() {
  server.send(404, "text/plain", "File Not Found\n\n");
}

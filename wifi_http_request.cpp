#include "wifi_http_request.h"

Wifi_http_request::Wifi_http_request(char HOST_NAME[], String PATH_NAME, String queryString, const int blueled_for_Wifi)
{
  HOST_NAME_ = HOST_NAME;
  PATH_NAME_ = PATH_NAME;
  queryString_ = queryString;
  blueled_for_Wifi_ = blueled_for_Wifi;
  ssid = SECRET_SSID;
  pass = SECRET_PASSWORD;
}

void Wifi_http_request::connection()
{
  if (client.connect(HOST_NAME_, 80))
  {
    client_connection = 1;
    digitalWrite(blueled_for_Wifi_, HIGH);
    Serial.println("Connected to server");
  }
  else
  {
    digitalWrite(blueled_for_Wifi_, LOW);
    Serial.println("Connection failed");
  }
}

void Wifi_http_request::begin()
{
  WiFi.begin(ssid, pass);
}

void Wifi_http_request::send_http_request()
{
  client.println("GET " + PATH_NAME_ + queryString_ + " HTTP/1.1");
  client.println("Host: " + String(HOST_NAME_));
  client.println("Connection: close");
  client.println(); // end HTTP header

  while (client.connected()) {
    if (client.available()) 
    {
    // read an incoming byte from the server and print it to serial monitor:
      char c = client.read();
      Serial.print(c);
    }
  }
  Serial.print('\n');
}

void Wifi_http_request::stop()
{
  client.stop();
  client_connection = 0;
  digitalWrite(blueled_for_Wifi_, LOW);
  time_stop = millis();
}

void Wifi_http_request::conditions_to_connect(){
    time_connection = millis();
    if((client_connection == 0) && ((time_connection - time_stop) > delay_time_for_reconnection))
    {
      connection();
    }
}
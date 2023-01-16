#include "wifi_http_request.h"

/*
Wifi_http_request is a class for the handling of the HTTP POST request.
We find some basic function for the connection (begin, connection, stop) 
and the function for sending the HTTP POST request.
*/

Wifi_http_request::Wifi_http_request(char HOST_NAME[], String PATH_NAME, String queryString, const int blueled_for_Wifi)
{
  /*
    Constructor of the class.

    Parameters:
    ============
    HOST_NAME: char*
               Host name.
    
    PATH_NAME: String
               Path name.

    queryString: String
                 Query string.
    
    blueled_for_Wifi: int
                      Integer that repreents the pin at which we have the blue (or white) LED 
                      for the WiFi (if WiFI is on the LED is ON).
  */
  HOST_NAME_ = HOST_NAME;
  PATH_NAME_ = PATH_NAME;
  queryString_ = queryString;
  blueled_for_Wifi_ = blueled_for_Wifi;
  ssid = SECRET_SSID;
  pass = SECRET_PASSWORD;
}

void Wifi_http_request::connection()
{
  /*
    Method for the client connection to the Host name.
    If the connection happens the Blue (or White) LED
    is turned ON and we see a successful message on the 
    Serial monitor.
    If the connection doesn't happen the Blue (or White) LED
    is OFF and we see a error message on the 
    Serial monitor.
  */
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
  /*
    Begin function for the activation of the WiFi.
  */
  WiFi.begin(ssid, pass);
}

void Wifi_http_request::send_http_request()
{
  /*
    Function for the HTTP POST request to Webhooks.
    If everything is fine we see a successfull message
    on the Serial Monitor (and also a phone call should
    arrive to the telephone).
  */
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
  /*
    Stop function for the client.
    The client disactivation means also that the 
    LED for WiFi is turned OFF.
    time_stop is used for the reconnection after the first
    HTTP POST request.
  */
  client.stop();
  client_connection = 0;
  digitalWrite(blueled_for_Wifi_, LOW);
  time_stop = millis();
}

void Wifi_http_request::conditions_to_connect(){
    /*
      Function for checking the right conditions for the
      reconnection after the first use.
      delay_time_for_reconnection is 20 seconds. 
      After we trigger the system Arduino send an HTTP POST request,
      then the client is stopped and we have the disconnection from the 
      server. Then we need to reconnect Arduino for another HTTP POST 
      request, if necessary, but we cannot do it instantaneously, since
      the HTTP POST request cannot be performed immediately. For that reason
      the request has 20 seconds to be executed (the time for the reconnection
      that is delay_time_for_reconnection).
    */
    time_connection = millis();
    if((client_connection == 0) && ((time_connection - time_stop) > delay_time_for_reconnection))
    {
      connection();
    }
}
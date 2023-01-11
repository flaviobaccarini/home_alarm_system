#include <String.h>
#include "arduino_secrets.h"
#include <WiFiNINA.h>

#ifndef WIFIREQUEST_H
#define WIFIREQUEST_H
class Wifi_http_request{
  private:
    WiFiClient client;
    char*   HOST_NAME_;
    String PATH_NAME_;
    String queryString_;
    // variables will change:
    int blueled_for_Wifi_;
    char* ssid;
    char* pass;
    int delay_time_for_reconnection = 20000;
    unsigned long time_stop, time_connection;

  public:
    bool client_connection = 0; // if the server is connected or not
    Wifi_http_request(char* HOST_NAME, String PATH_NAME, String queryString, const int blueled_for_Wifi);
    void connection();
    void send_http_request();
    void begin();
    void stop();
    void conditions_to_connect();
};
#endif
#ifndef HttpClient_h
#define HttpClient_h

#include <Arduino.h>
#include <ESP8266WiFi.h> // https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi/src
#include <WiFiClientSecure.h> // https://links2004.github.io/Arduino/d2/d2f/class_wi_fi_client_secure.html

class HttpClient 
{
public:
    HttpClient(const char* host);
    HttpClient(const char* _host, int _port);
    // Set ssl to on but do not verify server identity with fingerprint
    HttpClient(const char* _host, int _port, int _ssl);
    // Set fingerprint if using SSL, stores the SHA1 fingerprint of the remote site, implicity sets ssl to on
    HttpClient(const char* _host, int _port, const char* _fingerprint);

    // Generic HTTP Request
    int request(const char* method, const char* path, const char* body, String* response);
    // Set a Request Header
    void setHeader(const char*);
    // Set Content-Type Header
    void setContentType(const char*);
    // Set SSL support on(1) or off(0)
    void setSSL(int);

    // GET path and response
    int get(const char*, String*);

    // POST path and body and response
    int post(const char* path, const char* body, String*);

    // PATCH path and body and response
    int patch(const char* path, const char* body, String*);

    // PUT path and body and response
    int put(const char* path, const char* body, String*);

    // DELETE path and response
    int del(const char*, String*);
    // DELETE path and body and response
    int del(const char*, const char*, String*);

private:
    WiFiClient client;
    WiFiClientSecure sslClient;
    const char* host;
    int port;
    int num_headers;
    const char* headers[10];
    const char* contentType;
    const char* fingerprint;
    int ssl;
};

#endif
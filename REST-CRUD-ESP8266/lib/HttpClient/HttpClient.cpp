#include "HttpClient.h"

#define HTTP_DEBUG

#ifdef HTTP_DEBUG
#define HTTP_DEBUG_PRINT(string) (Serial.print(string))
#else
#define HTTP_DEBUG_PRINT(string)
#endif

//******************************************************************************************************************
// Constructors
//******************************************************************************************************************
HttpClient::HttpClient(const char* _host)
{
    host = _host;
    port = 80;
    ssl = 0;
    fingerprint = NULL;
    num_headers = 1;
    headers[0] = "Accept: application/json";
    contentType = "application/json; charset=utf-8";
}

HttpClient::HttpClient(const char* _host, int _port)
{
    host = _host;
    port = _port;
    ssl = 0;
    fingerprint = NULL;
    num_headers = 1;
    headers[0] = "Accept: application/json";
    contentType = "application/json; charset=utf-8";
}

HttpClient::HttpClient(const char* _host, int _port, const char* _fingerprint)
{
    host = _host;
    port = _port;
    ssl = 1;
    fingerprint = _fingerprint;
    num_headers = 1;
    headers[0] = "Accept: application/json";
    contentType = "application/json; charset=utf-8";
}

HttpClient::HttpClient(const char* _host, int _port, int _ssl) 
{
    host = _host;
    port = _port;
    ssl = (_ssl) ? 1 : 0;
    fingerprint = NULL;
    num_headers = 1;
    headers[0] = "Accept: application/json";
    contentType = "application/json; charset=utf-8";
}

//******************************************************************************************************************
// HTTP Methods
//******************************************************************************************************************
//GET path with response
int HttpClient::get(const char* path, String* response)
{
    return request("GET", path, NULL, response);
}

// POST path and body with response
int HttpClient::post(const char* path, const char* body, String* response)
{
    return request("POST", path, body, response);
}

// PATCH path and body with response
int HttpClient::patch(const char* path, const char* body, String* response)
{
    return request("PATCH", path, body, response);
}

// PUT path and body with response
int HttpClient::put(const char* path, const char* body, String* response)
{
    return request("PUT", path, body, response);
}

// DELETE path and response
int HttpClient::del(const char* path, String* response)
{
    return request("DELETE", path, NULL, response);
}

// DELETE path and body with response
int HttpClient::del(const char* path, const char* body, String* response)
{
    return request("DELETE", path, body, response);
}

void HttpClient::setHeader(const char* header)
{
    headers[num_headers] = header;
    num_headers++;
}

void HttpClient::setContentType(const char* contentTypeValue)
{
    contentType = contentTypeValue;
}

void HttpClient::setSSL(int _ssl)
{
    ssl = (_ssl) ? 1 : 0;
}

//******************************************************************************************************************
// Generic request method
//******************************************************************************************************************
int HttpClient::request(const char* method, const char* path, const char* body, String* response)
{
    HTTP_DEBUG_PRINT("HTTP: connecting " + String(host )+ ":" + String(port) + "\n");
    
    if (ssl) 
    {
        if (!sslClient.connect(host, port)) 
        {
            HTTP_DEBUG_PRINT("HTTPS Connection failed\n");
            return 0;
        }
    } 
    else 
    {
        if (!client.connect(host, port))
        {
            HTTP_DEBUG_PRINT("HTTP Connection failed\n");
            return 0;
        }
    }

    HTTP_DEBUG_PRINT("HTTP: connected\n");
    HTTP_DEBUG_PRINT("Request: \n");

    String request = String(method) + " " + String(path) + " HTTP/1.1\r\n";
    for (int i=0; i<num_headers; i++)
    {
        request += String(headers[i]) + "\r\n";
    }
    request += "Host: " + String(host) +  ":" + String(port) + "\r\n";
    request += "Connection: close\r\n";
    if (body != NULL)
    {
        char contentLength[30];
        sprintf(contentLength, "Content-Length: %d\r\n", strlen(body));
        request += String(contentLength);

        request += "Content-Type: " + String(contentType) + "\r\n\r\n";
        request += String(body) + "\r\n";
    }
    request += "\r\n\r\n";

    HTTP_DEBUG_PRINT(request);

    int code = 0;
    bool currentLineIsBlank = true;
    bool httpBody = false;
    bool inStatus = false;
    char statusCode[4];
    int i = 0;
    if (ssl)
    {
        sslClient.print(request.c_str());
        while (sslClient.connected()) 
        {
            HTTP_DEBUG_PRINT("waiting...\n");
            unsigned long timeout = millis();
            while (sslClient.available() == 0) 
            {
                if (millis() - timeout > 5000) 
                {
                    HTTP_DEBUG_PRINT("Client Timeout !\n");
                    sslClient.stop();
                    delay(60000);
                    return 0;
                }
            }
            while (sslClient.available()>0) 
            {
                char ch = static_cast<char>(sslClient.read());

                HTTP_DEBUG_PRINT(ch);

                if (ch == ' ' && !inStatus) inStatus = true;

                if (inStatus && i < 3 && ch != ' ')
                {
                    statusCode[i] = ch;
                    i++;
                }
                if (i == 3)
                {
                    statusCode[i] = '\0';
                    code = atoi(statusCode);
                }

                if (httpBody && response != NULL) response->concat(ch);
                else
                {
                    if (ch == '\n' && currentLineIsBlank) httpBody = true;
                    if (ch == '\n') currentLineIsBlank = true;
                    else if (ch != '\r') currentLineIsBlank = false;
                }
            }
        }
    } 
    else
    {
        client.print(request.c_str());
        while (client.connected()) 
        {
            HTTP_DEBUG_PRINT("waiting...\n");
            unsigned long timeout = millis();
            while (client.available() == 0) 
            {
                if (millis() - timeout > 5000) 
                {
                    HTTP_DEBUG_PRINT("Client Timeout !\n");
                    client.stop();
                    delay(60000);
                    return 0;
                }
            }
            while (client.available()>0) 
            {
                char ch = static_cast<char>(client.read());

                HTTP_DEBUG_PRINT(ch);

                if (ch == ' ' && !inStatus) inStatus = true;

                if (inStatus && i < 3 && ch != ' ')
                {
                    statusCode[i] = ch;
                    i++;
                }
                if (i == 3)
                {
                    statusCode[i] = '\0';
                    code = atoi(statusCode);
                }

                if (httpBody && response != NULL) response->concat(ch);
                else
                {
                    if (ch == '\n' && currentLineIsBlank) httpBody = true;
                    if (ch == '\n') currentLineIsBlank = true;
                    else if (ch != '\r') currentLineIsBlank = false;
                }
            }
        }
    }

    HTTP_DEBUG_PRINT("HTTP: stop client\n");
    num_headers = 0;
    if(ssl) sslClient.stop();
    else client.stop();
    delay(50);
    HTTP_DEBUG_PRINT("HTTP: client stopped\n");

    return code;
}
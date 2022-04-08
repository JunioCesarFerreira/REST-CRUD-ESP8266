#include <Arduino.h>
#include <HttpClient.h>
#include <RequestModel.h>
#include <ResponseModel.h>

#define LED_ON_BOARD 16
#define LED_EXTERNAL 5

// IP do servidor onde a WebApplication está sendo executada
HttpClient httpClient = HttpClient("192.168.0.1", 5000);
ResponseModel responseObject;

void RecoveryJson(String* response)
{
    int length = response->length();
    char chArray[length];
    bool curlyBracket = false;
    strcpy(chArray,response->c_str());
    response->clear();
    for (int i=0; i < length; i++)
    {
        if (chArray[i]=='{')
        {
            curlyBracket = true;
        } 
        if (curlyBracket)
        {
            if (chArray[i] == '\r' || chArray[i] == '\n') break;
            response->concat(chArray[i]);
        }
    }
}

void setup()
{
    pinMode (LED_ON_BOARD, OUTPUT);  
    pinMode (LED_EXTERNAL, OUTPUT);
    digitalWrite(LED_ON_BOARD, HIGH);
    digitalWrite(LED_EXTERNAL, HIGH);
    Serial.begin(9600);
    delay(1000);
    
    WiFi.begin("ssid", "password");
    unsigned short flag=0;
    char wainting[4] = {'\\','|','/','-'};
    unsigned short w=0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print("\rConnecting ");
        Serial.print(wainting[w]);
        w++;
        if (w==4) w=0;
        digitalWrite(LED_ON_BOARD, flag);
        flag = ~flag;
    }
    Serial.print("\n");

    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
    digitalWrite(LED_ON_BOARD, LOW);
    digitalWrite(LED_EXTERNAL, LOW);
}

void Test1()
{
    digitalWrite(LED_ON_BOARD, LOW);
    String response = String();
    int statusCode = httpClient.get("/api/home/test/1", &response);
    Serial.print("Status code from server: ");
    Serial.println(statusCode);
    if (statusCode==200)
    {
    Serial.println("Response:");
    RecoveryJson(&response);
    Serial.println(response.c_str());
    digitalWrite(LED_ON_BOARD, HIGH);
    
    responseObject.deserialize(response);
    Serial.println("\nObject deserialized:");
    Serial.print("\tid = "); 
    Serial.println(responseObject.id);
    Serial.print("\tname = ");
    Serial.println(responseObject.name);
    Serial.print("\tcommand = ");
    Serial.println(responseObject.command);
    Serial.print("\tparameters = {");
    for (int i=0; i<10; i++)
    {
        Serial.print(responseObject.parameters[i]);
        if (i < 9)
        {
            if (responseObject.parameters[i+1][0] == 0) break;
            Serial.print(", ");
        }
    }
    Serial.println("}");
    }
    delay(10000);
}

void Test2()
{
    digitalWrite(LED_ON_BOARD, LOW);
    String response = String();
    int statusCode = httpClient.get("/api/home/test/2", &response);
    Serial.print("Status code from server: ");
    Serial.println(statusCode);
    if (statusCode==200)
    {
    Serial.println("Response:");
    RecoveryJson(&response);
    Serial.println(response.c_str());
    digitalWrite(LED_ON_BOARD, HIGH);
    
    responseObject.deserialize(response);
    Serial.println("\nObject deserialized:");
    Serial.print("\tid = "); 
    Serial.println(responseObject.id);
    Serial.print("\tname = ");
    Serial.println(responseObject.name);
    Serial.print("\tcommand = ");
    Serial.println(responseObject.command);
    Serial.print("\tparameters = {");
    for (int i=0; i<10; i++)
    {
        Serial.print(responseObject.parameters[i]);
        if (i < 9)
        {
            if (responseObject.parameters[i+1][0] == 0) break;
            Serial.print(", ");
        }
    }
    Serial.println("}");
    }
    delay(10000);
}

void loop()
{
    // Começando a brincadeira
    Test1();
    Test2();
}
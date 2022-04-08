
#ifndef ResponseModel_h
#define ResponseModel_h

#include <Arduino.h>
#include <JsonCheckSyntax.h>

class ResponseModel 
{
public:
    int id;
    char name[16];
    char command[16];
    char parameters[10][16];

    ResponseModel();

    ResponseModel(String value);

    ~ResponseModel();

    bool deserialize(String value);

    String serialize();

private:
    char* ptr_json;
    static const int max_text_size = 256;
    static const int tag_id_length = 2;
    const char tag_id[tag_id_length] = {'i','d'};
    static const int tag_name_length = 4;
    const char tag_name[tag_name_length] = {'n','a','m','e'};
    static const int tag_command_length = 7;
    const char tag_command[tag_command_length] = {'c','o','m','m','a','n','d'};
    static const int tag_parameters_length = 10;
    const char tag_parameters[tag_parameters_length] = {'p','a','r','a','m','e','t','e','r','s'};

    bool validateTag(char* ptr, const char* tag, int length);
    int tenPower(int exp);
    int getIntegerValue(char* ptr);
    bool getBooleanValue(char* ptr);
    char* nextComma(char* ptr);
    char* movePtr(char* ptr, int incr);
    int recoveryText(char* ptr, char* destiny, int length);
    int recoveryTextArray(char* ptr, char destiny[10][16], int length);
};

#endif
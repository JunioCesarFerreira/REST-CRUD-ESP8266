#ifndef RequestModel_h
#define RequestModel_h

#include <Arduino.h>
#include <JsonCheckSyntax.h>

class RequestModel 
{
public:
    int id;
    char* name;
    char* description;
    double* data = new double[10];

    RequestModel();

    RequestModel(String value);

    bool deserialize(String value);

    String serialize();

private:
    static const int max_text_size = 256;
    static const int tag_id_length = 2;
    const char tag_id[tag_id_length] = {'i','d'};
    static const int tag_name_length = 4;
    const char tag_name[tag_name_length] = {'n','a','m','e'};
    static const int tag_description_length = 11;
    const char tag_description[tag_description_length] = {'d','e','s','c','r','i','p','t','i','o','n'};
    static const int tag_data_length = 4;
    const char tag_data[tag_data_length] = {'d','a','t','a'};

    bool validateTag(const char* ptr, const char* tag, int length);
    int tenPower(int exp);
    int getIntegerValue(const char* ptr);
    bool getBooleanValue(const char* ptr);
    const char* nextComma(const char* ptr);
    const char* movePtr(const char* ptr, int incr);
    bool recoveryText(const char* ptr, char* destiny, int length);
    bool recoveryDoubleArray(const char* ptr, double* destiny, int length);
};

#endif
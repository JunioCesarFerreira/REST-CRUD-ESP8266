#include "RequestModel.h"

using namespace JsonCheckSyntax;

//#define REQUEST_DEBUG

#ifdef REQUEST_DEBUG
#define REQUEST_DEBUG_PRINT(string) (Serial.print(string))
#else
#define REQUEST_DEBUG_PRINT(string)
#endif

RequestModel::RequestModel(){}

RequestModel::RequestModel(String jsonValue)
{
    deserialize(jsonValue);
}

bool RequestModel::deserialize(String jsonValue)
{
    REQUEST_DEBUG_PRINT("deserialize\n");
	int countOpenBraces = 0;    // Contador de aberturas de chaves
	int countCloseBraces = 0;   // Contador de fechamento de chaves
    const char* ptr_json = jsonValue.c_str(); 
    do
    {
        switch (*ptr_json)
        {
			case '{':
				countOpenBraces++;
				break;
			case '}':
				countCloseBraces++;
				break;
			case ',':
			    // Não faz nada, virgula que separa objetos.
				break;
			case '\"':
			    ptr_json++;
				switch (*ptr_json)
				{
                    case 'i':
                        if (validateTag(ptr_json, tag_id, tag_id_length))
                        {
                            ptr_json = movePtr(ptr_json, tag_id_length);
                            if (*ptr_json == '\"')
                            {
                                ptr_json++;
                                if (*ptr_json == ':')
                                {
                                    ptr_json++;
                                    id = getIntegerValue(ptr_json);
                                }
                                else return false;
                            }
                            else return false;
                        }
                        else return false;
                        break;
                    case 'n':
                        if (validateTag(ptr_json, tag_name, tag_name_length))
                        {
                            if (!recoveryText(ptr_json, name, tag_name_length))
                            {
                                return false;
                            }
                        }
                        else return false;
                        break;
                    case 'd':
                        if (validateTag(ptr_json, tag_description, tag_description_length))
                        {
                            if (!recoveryText(ptr_json, description, tag_description_length))
                            {
                                return false;
                            }
                        }
                        else if (validateTag(ptr_json, tag_data, tag_data_length))
                        {
                            if (!recoveryDoubleArray(ptr_json, data, tag_data_length))
                            {
                                return false;
                            }
                        }
                        else return false;
                        break;
                    default:
                        return false;
                }
				break;
			default:
				return false;
        }

		ptr_json++;
		if (*ptr_json == 0) 
        { 
			if (countOpenBraces == countCloseBraces) break;
			else return false;
		}
    }while(countOpenBraces > countCloseBraces);
    return true;
}

String RequestModel::serialize()
{
    String dataInfo = String();
    if (data == NULL) dataInfo = "null";
    else
    {
        dataInfo = "[";
        dataInfo += String(data[0]);
        for (int i=1; i< 10; i++)
        {
            dataInfo += "," + String(data[i]);
        }
        dataInfo += "]";
    }
    return "{\"id\":" + String(id)
        + ",\"name\":\"" + String(name)
        + "\",\"description\":\"" + String(description)
        + "\",data:" + dataInfo
        + "}";
}

bool RequestModel::recoveryDoubleArray(const char* ptr, double* destiny, int length)
{
    ptr = movePtr(ptr, length);
	if (*ptr == '\"')
	{
		ptr++;
		if (*ptr == ':')
		{
			ptr++;
			if (*ptr == 'n')
			{
				ptr = movePtr(ptr, 3); // null
				destiny = NULL;
			}
			else if (*ptr == '[')
			{
				ptr++;
				for (int i = 0; i < 10; i++)
				{
					if (*ptr == '\"')
					{
                        char* tmpValue = new char[16];
						for (int j = 0; j < 16; j++)
						{
							ptr++;
							if (*ptr == '\"') break;
							tmpValue[j] = *ptr;
						}
                        *destiny = atof(tmpValue);
                        delete tmpValue;
						if (*ptr != '\"') return false;
						ptr++;
						if (*ptr == ']') break;
						if (*ptr == ',') ptr++;
						else return false;
					}
					if (*ptr == ']')
                    {
						ptr++;
						break;
					}
				}
			}
			else return false;
		}
		else return false;
	}
	else return false;

    return true;
}

bool RequestModel::recoveryText(const char* ptr, char* destiny, int length)
{
    ptr = movePtr(ptr, length);
    if (*ptr == '\"')
    {
        ptr++;
        if (*ptr == ':')
        {
            ptr++;
            if (*ptr == '\"')
            {
                for (int i=0; i<max_text_size; i++)
                {
                    destiny[i] = *ptr;
                    ptr++;
                    if (*ptr == '\"')
                    {
                        i++;
                        destiny[i] = '\0';
                        ptr++;
                        break;
                    }
                }
            }
            else if (*ptr == 'n')
            {
                destiny = NULL;
                ptr = movePtr(ptr, 3); // null
            }
            else return false;
        }
        else return false;
    }
    else return false;
    return true;
}

bool RequestModel::validateTag(const char* ptr, const char* tag, int length)
{
    for (int i=0; i < length; i++)
    {
        if (tag[i] != ptr[i]) return false;
    }
    return true;
}

int RequestModel::tenPower(int exp)
{
	int n = 0, R = 1;
	while (n < exp)
	{
		R *= 10;
		n++;
	}
	return R;
}

int RequestModel::getIntegerValue(const char* ptr)
{
	int Count = 0;
	int Aux = 0;
	int Result = 0;
	int Exp = 0;
	while (*(ptr+Count)!=','){
		if (*(ptr+Count)=='}') break;
		Count++;
	}
	do
	{
		Count--;
		Aux = (*(ptr+Count)-48);
		if (0 > Aux || Aux > 9) return -1;
		Result += Aux * tenPower(Exp);  
		Exp++;
	}while (Count>0);
	return Result;	
}

bool RequestModel::getBooleanValue(const char* ptr)
{
	const char TRUE[4] = {'t','r','u','e'};
	int i;
	for (i = 0; i < 4; i++)
	{
		if (*(ptr+i)!=TRUE[i])
		{ 
			return false;
		}
	}
	return true;
}

const char* RequestModel::nextComma(const char* ptr)
{
    while (*(ptr)!=',')
	{
		ptr++;
		if ((*ptr == '}') || (*ptr == ']'))
		{
			ptr--;
			break;	
		}			
	}
	return ptr;
}

const char* RequestModel::movePtr(const char* ptr, int incr)
{
	int n;
	for (n = 0; n < incr; n++) ptr++;
	return ptr;
}
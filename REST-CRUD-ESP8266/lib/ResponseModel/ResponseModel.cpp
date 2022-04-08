#include "ResponseModel.h"

using namespace JsonCheckSyntax;

//#define RESPONSE_DEBUG

#ifdef RESPONSE_DEBUG
#define RESPONSE_DEBUG_PRINT(string) (Serial.print(string))
#else
#define RESPONSE_DEBUG_PRINT(string)
#endif

ResponseModel::ResponseModel(){}

ResponseModel::ResponseModel(String jsonValue)
{
    deserialize(jsonValue);
}

ResponseModel::~ResponseModel()
{
    delete[] ptr_json;
}

bool ResponseModel::deserialize(String jsonValue)
{
    RESPONSE_DEBUG_PRINT("deserialize\n");
	int countOpenBraces = 0;    // Contador de aberturas de chaves
	int countCloseBraces = 0;   // Contador de fechamento de chaves
    // Verifica sintaxe e minimiza json
    char* tmpChrArray = new char[jsonValue.length()];
    const char* ptr_tmp = jsonValue.c_str();
    for (unsigned int i=0; i<jsonValue.length(); i++)
    {
        tmpChrArray[i]= (char)*ptr_tmp;
        ptr_tmp++;
    }
    int minifyLength = CheckSyntax(tmpChrArray, jsonValue.length());
    ptr_json = new char[minifyLength];
    for (int i=0; i<minifyLength; i++)
    {
        ptr_json[i]=tmpChrArray[i];
    } 
    delete[] tmpChrArray;
    // Deserialização
    do {
        RESPONSE_DEBUG_PRINT("do:");
        RESPONSE_DEBUG_PRINT(*ptr_json);
        RESPONSE_DEBUG_PRINT("\n");
        // {""i,,""n""c
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
                RESPONSE_DEBUG_PRINT("tag:");
                RESPONSE_DEBUG_PRINT(*ptr_json);
                RESPONSE_DEBUG_PRINT("\n");
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
                                else
                                {
                                    return false;
                                }
                            }
                            else
                            {
                                return false;
                            }
                        }
                        else
                        {
                            return false;
                        }
                        break;
                    case 'n':
                        if (validateTag(ptr_json, tag_name, tag_name_length))
                        {
                            int jump = recoveryText(ptr_json, name, tag_name_length);
                            if (jump > -1)
                            {
                                ptr_json = movePtr(ptr_json, jump);
                            }
                            else
                            {
                                return false;
                            }
                        }
                        else
                        {
                            return false;
                        }
                        break;
                    case 'c':
                        if (validateTag(ptr_json, tag_command, tag_command_length))
                        {
                            int jump = recoveryText(ptr_json, command, tag_command_length);
                            if (jump > -1)
                            {
                                ptr_json = movePtr(ptr_json, jump);
                            }
                            else
                            {
                                return false;
                            }
                        }
                        else
                        {
                            return false;
                        }
                        break;
                    case 'p':
                        if (validateTag(ptr_json, tag_parameters, tag_parameters_length))
                        {
                            int jump = recoveryTextArray(ptr_json, parameters, tag_parameters_length);
                            if (jump > -1)
                            {
                                ptr_json = movePtr(ptr_json, jump);
                            }
                            else
                            {
                                return false;
                            }
                        }
                        break;
                    default:
                        return false;
                }
                RESPONSE_DEBUG_PRINT("end tag:");
                RESPONSE_DEBUG_PRINT(*ptr_json);
                RESPONSE_DEBUG_PRINT("\n");
				break;
			default:
				return false;
        }

		ptr_json++;
		if (*ptr_json == 0) 
        { 
			if (countOpenBraces == countCloseBraces) break;
			else
            {
                return false;
            }
		}
    } while(countOpenBraces > countCloseBraces);
    return true;
}

String ResponseModel::serialize()
{
    String dataInfo = String();
    if (parameters == NULL) dataInfo = "null";
    else
    {
        dataInfo = "[";
        dataInfo += String(parameters[0]);
        for (int i=1; i< 10; i++)
        {
            dataInfo += "," + String(parameters[i]);
        }
        dataInfo += "]";
    }
    return "{\"id\":" + String(id)
        + ",\"name\":\"" + String(name)
        + "\",\"command\":\"" + String(command)
        + "\",parameters:" + dataInfo
        + "}";
}

int ResponseModel::recoveryTextArray(char* ptr, char destiny[10][16], int length)
{
    ptr = movePtr(ptr, length);
    int count = 0;
	if (*ptr == '\"')
	{
		ptr++;
        length++;
		if (*ptr == ':')
		{
			ptr++;
            length++;
			if (*ptr == 'n')
			{
				ptr = movePtr(ptr, 3); // null
				destiny = NULL;
			}
			else if (*ptr == '[')
			{
				ptr++;
                length++;
				for (int i = 0; i < 10; i++)
				{
					if (*ptr == '\"')
					{
						for (int j = 0; j < 16; j++)
						{
							ptr++;
                            length++;
							if (*ptr == '\"')
                            {
                                destiny[count][j]=0;
                                 break;
                            }
							destiny[count][j] = *ptr;
						}
                        count++;
						if (*ptr != '\"') return false;
						ptr++;
                        length++;
						if (*ptr == ']') break;
						if (*ptr == ',') ptr++;
						else return false;
					}
					if (*ptr == ']')
                    {
						ptr++;
                        length++;
						break;
					}
				}
			}
			else return -1;
		}
		else return -1;
	}
	else return -1;
    while (count < 10)
    {
        destiny[count][0]=0;
        count++;
    }
    return length;
}

int ResponseModel::recoveryText(char* ptr, char* destiny, int length)
{
    ptr = movePtr(ptr, length);
    if (*ptr == '\"')
    {
        ptr++;
        length++;
        if (*ptr == ':')
        {
            ptr++;
            length++;
            if (*ptr == '\"')
            {
                ptr++;
                length++;
                for (int i=0; i<max_text_size; i++)
                {
                    destiny[i] = *ptr;
                    ptr++;
                    length++;
                    if (*ptr == '\"')
                    {
                        i++;
                        destiny[i] = '\0';
                        ptr++;
                        length++;
                        break;
                    }
                }
            }
            else if (*ptr == 'n')
            {
                *destiny = '\0';
                ptr = movePtr(ptr, 3); // null
                length+=3;
            }
            else return -1;
        }
        else return -1;
    }
    else return -1;
    return length;
}

bool ResponseModel::validateTag(char* ptr, const char* tag, int length)
{
    for (int i=0; i < length; i++)
    {
        if (tag[i] != ptr[i]) return false;
    }
    return true;
}

int ResponseModel::tenPower(int exp)
{
	int n = 0, R = 1;
	while (n < exp)
	{
		R *= 10;
		n++;
	}
	return R;
}

int ResponseModel::getIntegerValue(char* ptr)
{
	int Count = 0;
	int Aux = 0;
	int Result = 0;
	int Exp = 0;
	while (*(ptr+Count)!=',')
    {
		if (*(ptr+Count)=='}') break;
		Count++;
	}
	do {
		Count--;
		Aux = (*(ptr+Count)-48);
		if (0 > Aux || Aux > 9) return -1;
		Result += Aux * tenPower(Exp);  
		Exp++;
	} while (Count>0);
	return Result;	
}

bool ResponseModel::getBooleanValue(char* ptr)
{
	const char TRUE[4] = {'t','r','u','e'};
	int i;
	for (i = 0; i < 4; i++)
	{
		if (*(ptr+i) != TRUE[i])
		{ 
			return false;
		}
	}
	return true;
}

char* ResponseModel::nextComma(char* ptr)
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

char* ResponseModel::movePtr(char* ptr, int incr)
{
	int n;
	for (n = 0; n < incr; n++) ptr++;
	return ptr;
}
#include "JsonCheckSyntax.h"

int JsonCheckSyntax::CheckSyntax (char* txt_Json, int size_Json)
{
    char* minifyResult = new char[size_Json+1]; // Array temporária de processamento
    int minifyIndex = 0;   // Indexador do minifyResult

    // Contadores para verificação de sintaxe
    unsigned short quotmarks = 0;
    unsigned short openbraces = 0;
    unsigned short closebraces = 0;
    unsigned short openbrakets = 0;
    unsigned short closebrakets = 0;

    bool textkey = false;    // Indicador de interior de aspas
    bool postcom = false;    // Indicador de pós virgula
    bool valuekey = false;   // Indicador de pós dois pontos
    bool inarray = false;    // Indicador de interior de array
		
    // Varredura de limpeza inicial
    for (int i = 0; i < size_Json; i++)
    {
        // Ignora comentário de linha
        if (txt_Json[i] == '/' && txt_Json[i + 1] == '/' && !textkey)
        {
            do {
                i++;
                if (i >= size_Json) break;
            } while (txt_Json[i] != '\n');
        }
        // Ignora comentário de bloco
        else if (txt_Json[i] == '/' && txt_Json[i + 1] == '*' && !textkey)
        {
            i++;
            do {
                i++;
                if (i >= size_Json) break;
            } while (txt_Json[i] != '*' || txt_Json[i + 1] != '/');
            i++;
        }
        else
        {
            // Não insere espaços
            if ((txt_Json[i] != ' ' && txt_Json[i] != '\r' && txt_Json[i] != '\n' && txt_Json[i] != '\t')||textkey)
            {
                // Adiciona caracter
                minifyResult[minifyIndex] = txt_Json[i];
                minifyIndex++;
                if (postcom && minifyResult[minifyIndex - 1] != ',') postcom = false;
                // Incrementa contadores
                switch (txt_Json[i])
                {
                    case '{':
                        openbraces++;
                        break;
                    case '[':
                        inarray = true;
                        openbrakets++;
                        break;
                    case '}':
                        if (postcom)
                        {
	                        delete[] minifyResult;
                            return 0;
                        }
                        valuekey = false;
                        closebraces++;
                        break;
                    case ']':
                        if (postcom)
                        {
	                        delete[] minifyResult;
                            return 0;
                        }
                        inarray = false;
                        valuekey = false;
                        closebrakets++;
                        break;
                    case ',':
                        if (postcom)
                        {
	                        delete[] minifyResult;
                            return 0;
                        }
                        valuekey = false;
                        postcom = true;
                        break;
                    case '\"':
                        textkey = !textkey;
                        quotmarks++;
                        break;
                    case ':':
                        if (minifyResult[minifyIndex - 2] != '\"' && !textkey)
                        {
	                        delete[] minifyResult;
                            return 0;
                        }
                        valuekey = true;
                        break;
                    default:
                        if (!textkey && !valuekey && !inarray)
                        {
	                        delete[] minifyResult;
                            return 0;
                        }
                        break;
                }
            }
            if (openbraces == closebraces) break;
        }
    }
    minifyResult[minifyIndex] = 0; // Indica final da string
    
    // Verifica discrepâncias nas contagens de colchetes, chaves ou aspas
    if (openbrakets != closebrakets || openbraces != closebraces || quotmarks % 2 != 0)
    {
	    delete[] minifyResult;
        return 0;
    }

    // Verifica se falta virgula entre objetos
    for (int i = 0; i < minifyIndex; i++)
    {
        if (minifyResult[i] == '\"' && minifyResult[i + 1] == '\"')
        {
            if ((minifyResult[i - 1] != ':' && minifyResult[i - 1] != '[') || minifyResult[i + 2] != ',')
            {
	            delete[] minifyResult;
                return 0;
            }
        }
        if ((i + 1) < minifyIndex)
        {
            if (minifyResult[i] == '}')
            {
                if (minifyResult[i + 1] != ',' && minifyResult[i + 1] != '}' && minifyResult[i + 1] != ']')
                {
	                delete[] minifyResult;
                    return 0;
                }
            }
            if (minifyResult[i] == ',')
            {
                if (minifyResult[i + 1] == '}' || minifyResult[i + 1] == ']')
                {
	                delete[] minifyResult;
                    return 0;
                }
            }
        }
    }
    
	// Transfere dados de volta para ponteiro (buffer)
    for (int i=0; i < minifyIndex; i++)
    {
        *txt_Json = minifyResult[i];
        txt_Json++;
    } 

	delete[] minifyResult;	

	for (int i=minifyIndex; i < size_Json; i++)
    {
        *txt_Json = 0;
        txt_Json++;
    }
	
    return minifyIndex+1;
}
#include <PrintDebug.h>

#ifdef MAIN_DEBUG
#define MAIN_DEBUG_PRINT(string) Serial.print(string)
void printDebug(const char* format, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer,255, format, args);
    Serial.print(buffer);
    va_end(args);
}
#else
#define MAIN_DEBUG_PRINT(string)
void printDebug(const char* format, ...){}
#endif
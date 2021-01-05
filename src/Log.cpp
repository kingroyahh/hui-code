#include "Log.h"
#include "Define.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

int log_level = 0;

CUSTOM_LOG_FUNC_PTR u_log_ptr = NULL_PTR;

int mprint(int level, const char *format, va_list args,const char* fileName,int lineNO)
{
    char buffer[1024] = { 0 };
    memset(buffer, 0, 1024);
#ifdef _WIN64
    int ret = vsnprintf_s(buffer, 1024, 1024, format, args);
#else
    int ret = vsnprintf(buffer, 1024, format, args);
#endif
    if (fileName != NULL && lineNO > 0)
    {
        if (strlen(fileName) < 255)
        {
            char temp[256] = { 0 };
            sprintf(temp, "[%s][%d]", fileName, lineNO);
            int bufSize = strlen(buffer);
            int tempSize = strlen(temp);
            if (bufSize + tempSize < 255)
            {
                strcat(buffer, temp);
            }
        }
    }
    if (u_log_ptr != NULL_PTR)
    {
        u_log_ptr(level, buffer);
    }
    else
    {
        printf("%s", buffer);
    }
    return ret;
}

int verb(const char *format, ...)
{
    if (log_level > 0)
    {
        return 0;
    }
    va_list args;
    va_start(args, format);
    int ret = mprint(0, format, args, NULL, 0);
    va_end(args);
    return ret;
}

int debug(const char *format, ...)
{
    if (log_level > 1)
    {
        return 0;
    }
    va_list args;
    va_start(args, format);
    int ret = mprint(1, format, args, NULL, 0);
    va_end(args);
    return ret;
}

int info(const char *format, ...)
{
    if (log_level > 2)
    {
        return 0;
    }
    va_list args;
    va_start(args, format);
    int ret = mprint(2, format, args, NULL, 0);
    va_end(args);
    return ret;
}

int warning(const char *format, ...)
{
    if (log_level > 3)
    {
        return 0;
    }
    va_list args;
    va_start(args, format);
    int ret = mprint(3, format, args, NULL, 0);
    va_end(args);
    return ret;
}
int fileError(const char * fileName, int line, const char * format, ...)
{
    if (log_level > 4)
    {
        return 0;
    }
    va_list args;
    va_start(args, format);
    int ret = mprint(4, format, args,fileName,line);
    va_end(args);
    return ret;
}
#ifdef __cplusplus
extern "C"
{
#endif
    
void GRAV_API SetCustomLogLevel(int level)
{
    log_level = level;
}

void GRAV_API SetCustomLogFunc(CUSTOM_LOG_FUNC_PTR ptr)
{
    u_log_ptr = ptr;
}

#ifdef __cplusplus
}
#endif
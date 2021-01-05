#ifndef _GRAV_LOG_H_
#define _GRAV_LOG_H_
#ifdef __MAC__
#define PRINTF_LIKE __printflike(1, 2)
#else
#define PRINTF_LIKE
#endif

#include "Define.h"
#include <stdio.h>
#define TRACE_ENABLE

#ifndef _FILE_ERROR_IMPL
#define _FILE_ERROR_IMPL fileError
#endif

#define error(content,...) \
        _FILE_ERROR_IMPL(__FILE__,__LINE__,content,##__VA_ARGS__)
extern int log_level;

// Unity Debug.Log Delegate
typedef void (*CUSTOM_LOG_FUNC_PTR)(int, const char *);
extern CUSTOM_LOG_FUNC_PTR u_log_ptr;

int verb(const char * __restrict, ...) PRINTF_LIKE;
int debug(const char * __restrict, ...) PRINTF_LIKE;
int info(const char * __restrict, ...) PRINTF_LIKE;
int warning(const char * __restrict, ...) PRINTF_LIKE;
int fileError(const char * __restrict, int line ,const char * __restrict, ...) PRINTF_LIKE;

#ifdef TRACE_ENABLE
#define TRACE(...) verb(__VA_ARGS__)
#else
#define TRACE(...)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

void GRAV_API SetCustomLogLevel(int);
void GRAV_API SetCustomLogFunc(CUSTOM_LOG_FUNC_PTR);

#ifdef __cplusplus
}
#endif

#endif

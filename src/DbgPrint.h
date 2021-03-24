#pragma once
#include <stdarg.h>

#define ENABLE_DBG_PRINT

#ifdef ENABLE_DBG_PRINT
#define dbgPrint(_str, ...)\
{\
    printf(_str, ##__VA_ARGS__);\
}
#else
#define dbgPrint(_str, ...)
#endif

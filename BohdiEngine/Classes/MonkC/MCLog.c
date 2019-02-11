
#include "MCLog.h"
#include "MCType.h"

#ifndef LINE_MAX
#define LINE_MAX 2048
#endif

static int LOG_LEVEL = MC_DEBUG;
extern void MCLogTypeSet(MCLogType type)
{
    LOG_LEVEL = type;
}

int printc(const char* fmt, ...)
{
    int ret;
    char log_buf[LINE_MAX];
    
    va_list ap;
    va_start(ap, fmt);
    log_buf[vsprintf(log_buf, fmt, ap)]=NUL;
    ret = printf("%s%s%s%s", FCOLOR, BCOLOR, log_buf, NONE);
    va_end(ap);
    return ret;
}

void error_log(const char* fmt, ...)
{
    va_list ap;
    char log_buf[LINE_MAX];
    
    if(LOG_LEVEL != MC_SILENT){
        printf(LOG_FMT, LOG_COLOR_RED, "[Error] - ");
        va_start(ap, fmt);
        log_buf[vsprintf(log_buf, fmt, ap)]=NUL;
        printf("%s", log_buf);
        va_end(ap);
    }
}

void debug_log(const char* fmt, ...)
{
    va_list ap;
    char log_buf[LINE_MAX];
    
    if(LOG_LEVEL != MC_SILENT
       &&LOG_LEVEL != MC_ERROR_ONLY){
        printf(LOG_FMT, LOG_COLOR_LIGHT_BLUE, "[Debug] - ");
        va_start(ap, fmt);
        log_buf[vsprintf(log_buf, fmt, ap)]=NUL;
        printf("%s", log_buf);
        va_end(ap);
    }
}

void runtime_log(const char* fmt, ...)
{
    va_list ap;
    char log_buf[LINE_MAX];
    
    if(LOG_LEVEL != MC_SILENT
       &&LOG_LEVEL != MC_ERROR_ONLY
       &&LOG_LEVEL != MC_DEBUG){
        printf(LOG_FMT, LOG_COLOR_DARK_GRAY, "[RTime] - ");
        va_start(ap, fmt);
        log_buf[vsprintf(log_buf, fmt, ap)]=NUL;
        printf("%s", log_buf);
        va_end(ap);
    }
}

void error_logt(const char* tag, const char* fmt, ...)
{
    va_list ap;
    char log_buf[LINE_MAX];
    
    if(LOG_LEVEL != MC_SILENT){
        printf(LOG_FMT, LOG_COLOR_RED, "[Error] - ");
        printf(LOG_FMT, LOG_COLOR_DARK_GRAY, tag);
        va_start(ap, fmt);
        log_buf[vsprintf(log_buf, fmt, ap)]=NUL;
        printf("%s", log_buf);
        va_end(ap);
    }
}

void debug_logt(const char* tag, const char* fmt, ...)
{
    va_list ap;
    char log_buf[LINE_MAX];
    
    if(LOG_LEVEL != MC_SILENT
       &&LOG_LEVEL != MC_ERROR_ONLY){
        printf(LOG_FMT, LOG_COLOR_LIGHT_BLUE, "[Debug] - ");
        printf(LOG_FMT, LOG_COLOR_DARK_GRAY, tag);
        va_start(ap, fmt);
        log_buf[vsprintf(log_buf, fmt, ap)]=NUL;
        printf("%s", log_buf);
        va_end(ap);
    }
}

void runtime_logt(const char* tag, const char* fmt, ...)
{
    va_list ap;
    char log_buf[LINE_MAX];
    
    if(LOG_LEVEL != MC_SILENT
       &&LOG_LEVEL != MC_ERROR_ONLY
       &&LOG_LEVEL != MC_DEBUG){
        printf(LOG_FMT, LOG_COLOR_DARK_GRAY, "[RTime] - ");
        printf(LOG_FMT, LOG_COLOR_DARK_GRAY, tag);
        va_start(ap, fmt);
        log_buf[vsprintf(log_buf, fmt, ap)]=NUL;
        printf("%s", log_buf);
        va_end(ap);
    }
}


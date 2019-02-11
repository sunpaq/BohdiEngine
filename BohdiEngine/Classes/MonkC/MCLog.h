//
// Created by 孙御礼 on 2018-12-29.
//

#ifndef MCLOG_H
#define MCLOG_H

#include <limits.h>
#include <stdarg.h>

/*
 Log.h
 */

/* *
 * Configure whether use colored output log
 * some terminal/IDE can not support ANSI color codes
 * (comment out it to avoid strange output strings on some IDE)
 * */
//#define MC_LOG_USE_COLOR
typedef enum {
    MC_SILENT = 0,
    MC_ERROR_ONLY,
    MC_DEBUG,
    MC_VERBOSE
} MCLogType;

void MCLogTypeSet(MCLogType type);

#ifdef __ANDROID__
#include <android/log.h>
#define runtime_log(...)       __android_log_print(ANDROID_LOG_VERBOSE, "[monkc]", __VA_ARGS__)
#define debug_log(...)         __android_log_print(ANDROID_LOG_INFO,    "[monkc]", __VA_ARGS__)
#define error_log(...)         __android_log_print(ANDROID_LOG_ERROR,   "[monkc]", __VA_ARGS__)
#define runtime_logt(tag, ...) __android_log_print(ANDROID_LOG_WARN,  tag, __VA_ARGS__)
#define debug_logt(tag, ...)   __android_log_print(ANDROID_LOG_INFO,  tag, __VA_ARGS__)
#define error_logt(tag, ...)   __android_log_print(ANDROID_LOG_ERROR, tag, __VA_ARGS__)
#else
int printc(const char* fmt, ...);
void error_log(const char* fmt, ...);
void debug_log(const char* fmt, ...);
void runtime_log(const char* fmt, ...);
void error_logt(const char* tag, const char* fmt, ...);
void debug_logt(const char* tag, const char* fmt, ...);
void runtime_logt(const char* tag, const char* fmt, ...);
#endif

/*
 Log
 */
#ifdef MC_LOG_USE_COLOR
#define LOG_COLOR_NONE "\033[0m"
#define LOG_COLOR_BLACK "\033[0;30m"
#define LOG_COLOR_DARK_GRAY "\033[1;30m"
#define LOG_COLOR_BLUE "\033[0;34m"
#define LOG_COLOR_LIGHT_BLUE "\033[1;34m"
#define LOG_COLOR_GREEN "\033[0;32m"
#define LOG_COLOR_LIGHT_GREEN "\033[1;32m"
#define LOG_COLOR_CYAN "\033[0;36m"
#define LOG_COLOR_LIGHT_CYAN "\033[1;36m"
#define LOG_COLOR_RED "\033[0;31m"
#define LOG_COLOR_LIGHT_RED "\033[1;31m"
#define LOG_COLOR_PURPLE "\033[0;35m"
#define LOG_COLOR_LIGHT_PURPLE "\033[1;35m"
#define LOG_COLOR_BROWN "\033[0;33m"
#define LOG_COLOR_YELLOW "\033[1;33m"
#define LOG_COLOR_LIGHT_GRAY "\033[0;37m"
#define LOG_COLOR_WHITE "\033[1;37m"
#define LOG_FMT "%s%s\033[0m"
#else
#define LOG_COLOR_NONE ""
#define LOG_COLOR_BLACK ""
#define LOG_COLOR_DARK_GRAY ""
#define LOG_COLOR_BLUE ""
#define LOG_COLOR_LIGHT_BLUE ""
#define LOG_COLOR_GREEN ""
#define LOG_COLOR_LIGHT_GREEN ""
#define LOG_COLOR_CYAN ""
#define LOG_COLOR_LIGHT_CYAN ""
#define LOG_COLOR_RED ""
#define LOG_COLOR_LIGHT_RED ""
#define LOG_COLOR_PURPLE ""
#define LOG_COLOR_LIGHT_PURPLE ""
#define LOG_COLOR_BROWN ""
#define LOG_COLOR_YELLOW ""
#define LOG_COLOR_LIGHT_GRAY ""
#define LOG_COLOR_WHITE ""
#define LOG_FMT "%s%s"
#endif

//fonts color
#define FBLACK      "\033[30;"
#define FRED        "\033[31;"
#define FGREEN      "\033[32;"
#define FYELLOW     "\033[33;"
#define FBLUE       "\033[34;"
#define FPURPLE     "\033[35;"
#define D_FGREEN    "\033[6;"
#define FWHITE      "\033[7;"
#define FCYAN       "\x1b[36m"

//background color
#define BBLACK      "40m"
#define BRED        "41m"
#define BGREEN      "42m"
#define BYELLOW     "43m"
#define BBLUE       "44m"
#define BPURPLE     "45m"
#define D_BGREEN    "46m"
#define BWHITE      "47m"

//end color
#define NONE        "\033[0m"

/*
 Logs with color tags
 we use the same syntex with printf
 */


#ifndef __ANDROID__

static const char* FCOLOR = FRED;
static const char* BCOLOR = BBLACK;


#endif
#endif //MCLOG_H

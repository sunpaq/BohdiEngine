#include "MCClock.h"

#include <limits.h>
#ifndef LINE_MAX
#define LINE_MAX 2048
#endif

oninit(MCClock)
{
    if (init(MCObject)) {
        var(currentTimeBuff) = (char*)malloc(sizeof(char) * LINE_MAX);
        var(currentGMTBuff) = (char*)malloc(sizeof(char) * LINE_MAX);
        return obj;
    }else{
        return null;
    }
}

fun(MCClock, void, bye, voida)
{
    free(var(currentTimeBuff));
    free(var(currentGMTBuff));
}

fun(MCClock, MCClock*, setTimeToNow, voida)
{
	time_t timer = time(NULL);
	obj->rawtime = *localtime(&timer);
    return obj;
}

fun(MCClock, void, setTime, int tm_sec, int tm_min, int tm_hour,
			   int tm_mday, int tm_mon, int tm_year,
			   int tm_wday)
{
	if(tm_sec!=no_change) obj->rawtime.tm_sec = tm_sec;
	if(tm_min!=no_change) obj->rawtime.tm_min = tm_min;
	if(tm_hour!=no_change) obj->rawtime.tm_hour = tm_hour;
	if(tm_mday!=no_change) obj->rawtime.tm_mday = tm_mday;
	if(tm_mon!=no_change) obj->rawtime.tm_mon = tm_mon;
	int year = tm_year-1900;
	if(tm_year!=no_change) obj->rawtime.tm_year = year;
	if(tm_wday!=no_change) obj->rawtime.tm_wday = tm_wday;
}

fun(MCClock, void, adjustTime, int tm_sec, int tm_min, int tm_hour,
                  int tm_mday, int tm_mon, int tm_year,
                  int tm_wday)
{
	if(tm_sec!=no_change) obj->rawtime.tm_sec += tm_sec;
	if(tm_min!=no_change) obj->rawtime.tm_min += tm_min;
	if(tm_hour!=no_change) obj->rawtime.tm_hour += tm_hour;
	if(tm_mday!=no_change) obj->rawtime.tm_mday += tm_mday;
	if(tm_mon!=no_change) obj->rawtime.tm_mon += tm_mon;
	if(tm_year!=no_change) obj->rawtime.tm_year += tm_year;
	if(tm_wday!=no_change) obj->rawtime.tm_wday += tm_wday;
}

fun(MCClock, void, setRawtime, struct tm rawtime_in)
{
	obj->rawtime = rawtime_in;
}

fun(MCClock, void, setRawtimeFields, int tm_sec, int tm_min, int tm_hour, 
						int tm_mday, int tm_mon, int tm_year,
						int tm_wday, int tm_yday, int tm_isdst)
{
	if(tm_sec!=no_change)obj->rawtime.tm_sec = tm_sec;
	if(tm_sec!=no_change)obj->rawtime.tm_min = tm_min;
	if(tm_sec!=no_change)obj->rawtime.tm_hour = tm_hour;
	//
	if(tm_sec!=no_change)obj->rawtime.tm_mday = tm_mday;
	if(tm_sec!=no_change)obj->rawtime.tm_mon = tm_mon;
	if(tm_sec!=no_change)obj->rawtime.tm_year = tm_year-1900;
	//
	if(tm_sec!=no_change)obj->rawtime.tm_wday = tm_wday;
	if(tm_sec!=no_change)obj->rawtime.tm_yday = tm_yday;
	if(tm_sec!=no_change)obj->rawtime.tm_isdst = tm_isdst;
}

fun(MCClock, void, getTime, time_t* const result)
{
	*result = mktime(&(obj->rawtime));
}

fun(MCClock, void, getRawtime, struct tm* const result)
{
	*result = obj->rawtime;
}

fun(MCClock, char*, getTimeByString, voida)
{
	strcpy((char*)obj->currentTimeBuff, asctime(&(obj->rawtime)));
	return (char*)obj->currentTimeBuff;
}

fun(MCClock, void, getCPUClocksPerSecond, clock_t* const result)
{
	*result = CLOCKS_PER_SEC;
}

fun(MCClock, void, getCPUClocksSinceStart, clock_t* const result)
{
	*result = clock();
}

fun(MCClock, time_t, getCPUSecondsSinceStart, voida)
{
    return (clock() / CLOCKS_PER_SEC);
}

fun(MCClock, time_t, getCPUSecondsSince, time_t since)
{
    return (clock() / CLOCKS_PER_SEC) - since;
}

fun(MCClock, char*, getCurrentTimeString, voida)
{
	time_t timer = time(NULL);
	strcpy((char*)obj->currentTimeBuff, asctime(localtime(&timer)));
	return (char*)obj->currentTimeBuff;
}

fun(MCClock, char*, getCurrentGMTTimeString, voida)
{
	time_t timer = time(NULL);
	strcpy((char*)obj->currentGMTBuff, asctime(gmtime(&timer)));
	return (char*)obj->currentGMTBuff;
}

fun(MCClock, void, printTime, voida)
{
	printf("\n%s", asctime(&obj->rawtime));
}

fun(MCClock, void, printCurrentTime, voida)
{
	time_t timer = time(NULL);
	printf("%s", asctime(localtime(&timer)));
}

fun(MCClock, void, printCurrentGMTTime, voida)
{
	time_t timer = time(NULL);
	printf("%s", asctime(gmtime(&timer)));
}

char* MCClock_rawtime2String(time_t* timeval)
{
	return ctime(timeval);
}

char* MCClock_settableTime2String(struct tm *tm)
{
	return asctime(tm);
}

struct tm* MCClock_rawtime2SettableTimeGMT(time_t* timeval)
{
	return gmtime(timeval);
}

struct tm* MCClock_rawtime2SettableTimeLocal(time_t* timeval)
{
	return localtime(timeval);
}

onload(MCClock)
{
    if (load(MCObject)) {
    	bid(MCClock, void, bye, voida);
        bid(MCClock, void, setTimeToNow);
        bid(MCClock,
                void, setTime, int tm_sec, int tm_min, int tm_hour,
                int tm_mday, int tm_mon, int tm_year,
                int tm_wday);
        bid(MCClock,
                void, adjustTime, int tm_sec, int tm_min, int tm_hour,
                int tm_mday, int tm_mon, int tm_year,
                int tm_wday);
        
        bid(MCClock, void, setRawtime, struct tm rawtime_in);
        bid(MCClock,
                void, setRawtimeFields, int tm_sec, int tm_min, int tm_hour,
                int tm_mday, int tm_mon, int tm_year,
                int tm_wday, int tm_yday, int tm_isdst);
        
        bid(MCClock, void, getTime, time_t* const result);
        bid(MCClock, void, getRawtime, struct tm* const result);
        bid(MCClock, char*, getTimeByString);
        bid(MCClock, void, getCPUClocksPerSecond, clock_t* const result);
        bid(MCClock, void, getCPUClocksSinceStart, clock_t* const result);
        bid(MCClock, time_t, getCPUSecondsSinceStart, voida);
        bid(MCClock, time_t, getCPUSecondsSince, time_t since);
        bid(MCClock, char*, getCurrentTimeString);//retrun the same format as asctime: Sun Sep 16 01:03:52 1973\n\0
        bid(MCClock, char*, getCurrentGMTTimeString);
        bid(MCClock, void, printTime);
        bid(MCClock, void, printCurrentTime);
        bid(MCClock, void, printCurrentGMTTime);
        return cla;
    }else{
        return null;
    }
}

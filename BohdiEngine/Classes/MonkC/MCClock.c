#include "MCClock.h"

#ifndef LINE_MAX
#define LINE_MAX 2048
#endif

fun(bye, void)) as(MCClock)
	if (it->currentTimeBuff) {
		free(it->currentTimeBuff);
	}
	if (it->currentGMTBuff) {
		free(it->currentGMTBuff);
	}
}

fun(setTimeToNow, MCClock_t*)) as(MCClock)
	time_t timer = time(NULL);
	it->RawTime = *localtime(&timer);
    return it;
}

fun(setTime, void), struct MCTime time) as(MCClock)
	if(time.tm_sec!=no_change) it->RawTime.tm_sec = time.tm_sec;
	if(time.tm_min!=no_change) it->RawTime.tm_min = time.tm_min;
	if(time.tm_hour!=no_change) it->RawTime.tm_hour = time.tm_hour;
	if(time.tm_mday!=no_change) it->RawTime.tm_mday = time.tm_mday;
	if(time.tm_mon!=no_change) it->RawTime.tm_mon = time.tm_mon;
	int year = time.tm_year-1900;
	if(time.tm_year!=no_change) it->RawTime.tm_year = year;
	if(time.tm_wday!=no_change) it->RawTime.tm_wday = time.tm_wday;
}

fun(adjustTime, void), struct MCTime time) as(MCClock)
	if(time.tm_sec!=no_change) it->RawTime.tm_sec += time.tm_sec;
	if(time.tm_min!=no_change) it->RawTime.tm_min += time.tm_min;
	if(time.tm_hour!=no_change) it->RawTime.tm_hour += time.tm_hour;
	if(time.tm_mday!=no_change) it->RawTime.tm_mday += time.tm_mday;
	if(time.tm_mon!=no_change) it->RawTime.tm_mon += time.tm_mon;
	if(time.tm_year!=no_change) it->RawTime.tm_year += time.tm_year;
	if(time.tm_wday!=no_change) it->RawTime.tm_wday += time.tm_wday;
}

fun(setRawTime, void), struct tm rawTime) as(MCClock)
	it->RawTime = rawTime;
}

fun(getTime, void), time_t* const result) as(MCClock)
	*result = mktime(&(it->RawTime));
}

fun(getRawTime, void), struct tm* const result) as(MCClock)
	*result = it->RawTime;
}

fun(getTimeByString, const char*)) as(MCClock)
	strcpy(it->currentTimeBuff, asctime(&(it->RawTime)));
	return it->currentTimeBuff;
}

fun(getCPUClocksPerSecond, void), clock_t* const result) {
	*result = CLOCKS_PER_SEC;
}

fun(getCPUClocksSinceStart, void), clock_t* const result) {
	*result = clock();
}

fun(getCPUSecondsSinceStart, void), time_t* const result) {
    *result = (clock() / CLOCKS_PER_SEC);
}

fun(getCPUSecondsSince, void), time_t since, time_t* const result) {
    *result = (clock() / CLOCKS_PER_SEC) - since;
}

fun(getCurrentTimeString, const char*)) as(MCClock)
	time_t timer = time(NULL);
	strcpy(it->currentTimeBuff, asctime(localtime(&timer)));
	return it->currentTimeBuff;
}

fun(getCurrentGMTTimeString, const char*)) as(MCClock)
	time_t timer = time(NULL);
	strcpy(it->currentGMTBuff, asctime(gmtime(&timer)));
	return it->currentGMTBuff;
}

fun(printTime, void)) as(MCClock)
	printf("\n%s", asctime(&it->RawTime));
}

fun(printCurrentTime, void))
{
	time_t timer = time(NULL);
	printf("%s", asctime(localtime(&timer)));
}

fun(printCurrentGMTTime, void))
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

constructor(MCClock)) {
	MCObject(any);
	as(MCClock)
		it->currentTimeBuff = (char*)malloc(sizeof(char) * LINE_MAX);
		it->currentGMTBuff = (char*)malloc(sizeof(char) * LINE_MAX);
	}
	dynamic(MCClock)
		funbind(bye);
		funbind(setTimeToNow);
		funbind(setTime);
		funbind(adjustTime);
		funbind(setRawTime);
		funbind(getTime);
		funbind(getRawTime);
		funbind(getTimeByString);
		funbind(getCPUClocksPerSecond);
		funbind(getCPUClocksSinceStart);
		funbind(getCPUSecondsSince);
		funbind(getCPUSecondsSinceStart);
		funbind(getCurrentTimeString);
		funbind(getCurrentGMTTimeString);
		funbind(printTime);
		funbind(printCurrentTime);
		funbind(printCurrentGMTTime);
	}
	return any;
}



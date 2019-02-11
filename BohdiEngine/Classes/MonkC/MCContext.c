#include "MCContext.h"
#include "MCString.h"
#include "MCLog.h"

//Old Mac9 end of line sequence: \r
//Unix OSX end of line sequence: \n
//Windows  end of line sequence: \r\n
bool isNewLine(const char* s)
{
	if (s) {
		if (*s == '\n') {
			return true;
		} else if (*s == '\r') { //Windows NewLine
			return true;
		}
	}
	return false;
}

static char get_one_char()
{
    char cf = '\0';
    while(!isNewLine(&cf)) {
        cf = (char)getchar();
    };//clear the buff
	return cf;
}

static void get_chars_until_enter(char resultString[])
{
	char tc = NUL;
	int i=0;
	while(!isNewLine(&tc)){
        tc = (char)getchar();
		resultString[i]=tc;
		i++;
		putchar(tc);
	}
	resultString[i] = NUL;
	putchar(tc);
}

fun(bye, void))
{
	runtime_log("%s\n", "MCContext goodbye");
}

fun(dumpParas, void)) as(MCContext)
	int i;
	for (i = 0; i < it->argc; ++i) {
		runtime_log("%s\n", it->argv[i]);
	}
}

fun(getPara, char*), int index) as(MCContext)
	return it->argv[index];
}

fun(isIndexedParaEqualTo, int), int index, char* para) as(MCContext)
	char* para1 = it->argv[index];
	if (para1==null)return 0;
	if (strcmp(para1, para)==0)return 1;
	else return 0;
}

fun(isHavePara, int), char* para) as(MCContext)
    if(it==null)return 0;
	int i, res;
	for (i = 0; i < it->argc; ++i)
	{
		char* tmp = it->argv[i];
        if (tmp!=null && para!=null) {
			res = strcmp(tmp, para);
		} else {
			return 0;
		}
		if(res==0) {
			return 1;
		}
	}
	return 1;
}

fun(showMenuAndGetSelectionChar, char), int count, ...) as(MCContext)
    va_list ap;
	va_start(ap, count);
	int i;
	for (i = 0; i < count; ++i)
	{
		char* arg = va_arg(ap, char*);
		printf("%d.%s\n", i+1, arg);
	}
	printf("%s\n", "your selection is?");
	va_end(ap);
	it->selectionChar = get_one_char();
	return it->selectionChar;
}

fun(showConfirmAndGetBOOL, int), const char* confirm)
{
    printf("%s (y/n)?\n", confirm);
	char cf = get_one_char();
	if (cf=='y'){
		return 1;
	}
	return 0;
}

fun(getUserInputString, void), char resultString[])
{
	get_chars_until_enter(resultString);
}

/*
#include <stdlib.h>
char *getenv(const char *name);
int putenv(char *str); //need "key=value"
int setenv(const char *name, const char *value, int rewrite);
int unsetenv(const char *name);
*/

fun(getEnvironmentVar, char*), const char* key)
{
	//char *getenv(const char *name);
	return getenv(key);
}

fun(setEnvironmentVar, int), const char* key, const char* value, int isOverwrite)
{
	//int setenv(const char *name, const char *value, int rewrite);
	if (setenv(key, value, isOverwrite)==0)
		return 0;
	else
		return -1;
}

fun(clearEnvironmentVar, int), const char* key)
{
	//int unsetenv(const char *name);
	if (unsetenv(key)==0)
		return 0;
	else
		return -1;
}

constructor(MCContext), int argc, char** argv) {
	MCObject(any);
	as(MCContext)
		it->selectionChar = 0;
	}
	dynamic(MCContext)
		funbind(bye);
		funbind(dumpParas);
		funbind(getPara);
		funbind(isIndexedParaEqualTo);
		funbind(isHavePara);
		funbind(showMenuAndGetSelectionChar);
		funbind(showConfirmAndGetBOOL);
		funbind(getUserInputString);
		funbind(getEnvironmentVar);
		funbind(setEnvironmentVar);
		funbind(clearEnvironmentVar);
	}
	return any;
}



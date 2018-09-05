#ifndef MCContext_
#define MCContext_

#include "monkc.h"

class(MCContext, MCObject,
	int argc;
	char** argv;
	char selectionChar;
);

fun(MCContext, MCContext*, newWithArgs, int argc, char** argv);
fun(MCContext, void, bye, voida);
fun(MCContext, void, dumpParas, voida);
fun(MCContext, char*, getPara, int index);
fun(MCContext, int, isIndexedParaEqualTo, int index, char* para);
fun(MCContext, int, isHavePara, char* para);
fun(MCContext, char, showMenuAndGetSelectionChar, int count, ...);
fun(MCContext, int, showConfirmAndGetBOOL, const char* confirm);
fun(MCContext, void, getUserInputString, char resultString[]);
fun(MCContext, char*, getEnvironmentVar, const char* key);
fun(MCContext, int, setEnvironmentVar, const char* key, const char* value, int isOverwrite);
fun(MCContext, int, clearEnvironmentVar, const char* key);

extern char **environ;

#endif

/*
#include <stdlib.h>
char *getenv(const char *name);
int putenv(char *str); //need "key=value"
int setenv(const char *name, const char *value, int rewrite);
int unsetenv(const char *name);
*/


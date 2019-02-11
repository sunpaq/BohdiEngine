#ifndef MCContext_h
#define MCContext_h

#include "MCObject.h"

structure(MCContext, MCObject)
    int argc;
    char** argv;
    char selectionChar;

    fundef(bye, void));
    fundef(dumpParas, void));
    fundef(getPara, char*), int index);
    fundef(isIndexedParaEqualTo, int), int index, char* para);
    fundef(isHavePara, int), char* para);
    fundef(showMenuAndGetSelectionChar, char), int count, ...);
    fundef(showConfirmAndGetBOOL, int), const char* confirm);
    fundef(getUserInputString, void), char resultString[]);
    fundef(getEnvironmentVar, char*), const char* key);
    fundef(setEnvironmentVar, int), const char* key, const char* value, int isOverwrite);
    fundef(clearEnvironmentVar, int), const char* key);
};

constructor(MCContext), int argc, char** argv);

alias(MCContext);

extern char **environ;

#endif

/*
#include <stdlib.h>
char *getenv(const char *name);
int putenv(char *str); //need "key=value"
int setenv(const char *name, const char *value, int rewrite);
int unsetenv(const char *name);
*/


#ifndef MCString_h
#define MCString_h

#include <string.h>
#include <stdbool.h>
#include "MCObject.h"

structure(MCString, MCObject)
    size_t length;
    size_t size;
    size_t cursor;
    char* buff;

    fundef(add, void), const char* str);
    fundef(toCString, const char*), char const buff[]);
    fundef(equalTo, int), struct MCString* stringToComp);
    fundef(getCharsUntilEnter, void), char resultString[]);
    fundef(getOneChar, char));
    fundef(print, void), bool withNewline);
    fundef(startWith, bool), const char* str);
    fundef(toDoubleValue, double), char** endptr);
    fundef(copyCompressedString, struct MCString*));
    fundef(copyExtractedString, struct MCString*));
    fundef(release, void));
};

constructor(MCString), const char* cstring);

alias(MCString);

MCString_t* MCString_newWithCString(const char* cstr);
MCString_t* MCString_newWithMCString(MCString_t* mcstr);
MCString_t* MCString_newForHttp(char* cstr, int isHttps);
bool MCString_contains(const char* str, const char* instr);
size_t MCString_replace(const char* str, const char* withstr, const char* instr, char (*buff)[]);
size_t MCString_reverse(const char* str, char *buff);
size_t MCString_extensionFromFilename(const char* name, char* basebuff, char* extbuff);

const char* MCString_percentEncode(const char* str, char *buff);
const char* MCString_percentDecode(const char* str, char *buff);
const char* MCString_baseFromPath(const char* path, char (*buff)[]);
const char* MCString_filenameFromPath(const char* path, char (*buff)[]);
const char* MCString_concate(const char** strings, size_t count, char (*buff)[]);
const char* MCString_concateWith(const char* sp, const char* path1, const char* path2, char (*buff)[]);
const char* MCString_concatePath(const char* path1, const char* path2, char (*buff)[]);
const char* MCString_compressToCharCount(const char* source, char* buff);
const char* MCString_extractFromCharCount(const char* source, char* buff);
void MCString_printPermutationOf(char str[]);


static char* MCStringFill(char* dest, const char* src) {
    char* res = strcpy(dest, src);
    dest[strlen(src)] = '\0';
    return res;
}

static char* MCStringFillLimited(char* dest, const char* src, size_t limit) {
    char* res = strncpy(dest, src, limit);
    dest[strlen(src)] = '\0';
    return res;
}

static bool MCStringEqualN(const char* A, const char* B, size_t n) {
    if (strncmp(A, B, n) == 0) {
        return true;
    }else{
        return false;
    }
}

static bool MCStringEqual(const char* A, const char* B) {
    if (strcmp(A, B) == 0) {
        return true;
    }else{
        return false;
    }
}

//#define NUL '\0'
//#define PATH_MAX 2048

#define S(string) #string

#endif




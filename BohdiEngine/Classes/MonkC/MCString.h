#ifndef MCString_h
#define MCString_h

#include <string.h>

#include "monkc.h"

class(MCString, MCObject,
	size_t length;
	size_t size;
    size_t cursor;
	char* buff;
)

//length not include NUL
//real size, include NUL include empty space in buffer

fun(MCString, MCString*, initWithCString, const char* str);
fun(MCString, void, add, char* str);
fun(MCString, const char*, toCString, char const buff[]);
fun(MCString, int, equalTo, MCString* stringToComp);
fun(MCString, void, getCharsUntilEnter, char resultString[]);
fun(MCString, char, getOneChar, voida);
fun(MCString, void, print, MCBool withNewline);
fun(MCString, void, bye, voida);
fun(MCString, MCBool, startWith, const char* str);
fun(MCString, double, toDoubleValue, char** endptr);
fun(MCString, MCString*, copyCompressedString, voida);
fun(MCString, MCString*, copyExtractedString, voida);

MCString* MCString_newWithCString(const char* cstr);
MCString* MCString_newWithMCString(MCString* mcstr);
MCString* MCString_newForHttp(char* cstr, int isHttps);

util(MCString, MCBool, contains, const char* str, const char* instr);
util(MCString, size_t, replace, const char* str, const char* withstr, const char* instr, char (*buff)[]);
util(MCString, size_t, reverse, const char* str, char *buff);

util(MCString, const char*, percentEncode, const char* str, char *buff);
util(MCString, const char*, percentDecode, const char* str, char *buff);

util(MCString, const char*, baseFromPath, const char* path, char (*buff)[]);
util(MCString, const char*, filenameFromPath, const char* path, char (*buff)[]);

util(MCString, size_t, extensionFromFilename, const char* name, char* basebuff, char* extbuff);

util(MCString, const char*, concate, const char** strings, size_t count, char (*buff)[]);
util(MCString, const char*, concateWith, const char* sp, const char* path1, const char* path2, char (*buff)[]);
util(MCString, const char*, concatePath, const char* path1, const char* path2, char (*buff)[]);

util(MCString, const char*, compressToCharCount, const char* source, char* buff);
util(MCString, const char*, extractFromCharCount, const char* source, char* buff);

util(MCString, void, printPermutationOf, char str[]);
util(MCString, const char*, trimWhiteSpace, const char** target_p);
util(MCString, MCBool, isNewLine, const char* s);

MCInline char* MCStringFill(char* dest, const char* src) {
    char* res = strcpy(dest, src);
    dest[strlen(src)] = NUL;
    return res;
}

MCInline char* MCStringFillLimited(char* dest, const char* src, size_t limit) {
    char* res = strncpy(dest, src, limit);
    dest[strlen(src)] = NUL;
    return res;
}

MCInline MCBool MCStringEqualN(const char* A, const char* B, size_t n) {
    if (strncmp(A, B, n) == 0) {
        return true;
    }else{
        return false;
    }
}

MCInline MCBool MCStringEqual(const char* A, const char* B) {
    if (strcmp(A, B) == 0) {
        return true;
    }else{
        return false;
    }
}

#endif




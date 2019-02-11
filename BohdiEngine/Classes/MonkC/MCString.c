#include "MCString.h"
#include "MCLog.h"

#include <limits.h>
#ifndef LINE_MAX
#define LINE_MAX 2048
#endif
#ifndef PATH_MAX
#define PATH_MAX 2048
#endif

static const char  MCTab = '\t';
static const char  MCWhiteSpace = ' ';
static const char  MCNewLineN = '\n';
static const char  MCNewLineR = '\r';
#define MCCond_PathDiv(w)   (*w == '/' || *w =='\\')

//return remain string
const char* trimWhiteSpace(const char** target_p)
{
    const char* iter = *target_p;
    while (*iter == MCWhiteSpace || *iter == MCTab)
        iter++;
    *target_p = iter;//update remain
    return iter;
}

//Old Mac9 end of line sequence: \r
//Unix OSX end of line sequence: \n
//Windows  end of line sequence: \r\n
static bool isNewLine(const char* s)
{
    if (s) {
        if (*s == MCNewLineN) {
            return true;
        } else if (*s == MCNewLineR) { //Windows NewLine
            return true;
        }
    }
    return false;
}

bool MCString_contains(const char* str, const char* instr)
{
    if (strstr(instr, str)) {
        return true;
    }
    return false;
}

size_t MCString_replace(const char* str, const char* withstr, const char* instr, char (*buff)[])
{
    size_t count = strlen(str);
    size_t wcount = strlen(withstr);
    
    size_t i=0, b=0, o=0;
    while (instr[i] != '\0') {
        while (instr[i+o] != '\0' && instr[i+o] == str[0+o]) {
            o++;
        }
        if (o == (int)count) {
            int k;
            for (k=0; k<wcount; k++) {
                (*buff)[b++] = withstr[k];
            }
            i += o;
        }
        o = 0;
        (*buff)[b++] = instr[i++];
    }
    (*buff)[b++] = '\0';
    
    return b;
}

size_t MCString_reverse(const char* str, char *buff)
{
    size_t count = strlen(str);
    char* c = (char*)&str[count-1];
    int i;
    for (i=0; i<count; i++) {
        buff[i] = *c;
        c--;
    }
    buff[count] = '\0';
    return count;
}

const char* MCString_percentEncode(const char* str, char *buff)
{
    if (!str || !buff) {
        return null;
    }
    char* iter = (char*)str; int b = 0;
    while (*iter != '\0') {
        //2
        if (*iter == ' ') { buff[b++] = '%';buff[b++] = '2';buff[b++] = '0'; }
        else if (*iter == '!') { buff[b++] = '%';buff[b++] = '2';buff[b++] = '1'; }
        else if (*iter == '"') { buff[b++] = '%';buff[b++] = '2';buff[b++] = '2'; }
        else if (*iter == '#') { buff[b++] = '%';buff[b++] = '2';buff[b++] = '3'; }
        else if (*iter == '$') { buff[b++] = '%';buff[b++] = '2';buff[b++] = '4'; }
        else if (*iter == '%') { buff[b++] = '%';buff[b++] = '2';buff[b++] = '5'; }
        else if (*iter == '&') { buff[b++] = '%';buff[b++] = '2';buff[b++] = '6'; }
        else if (*iter =='\'') { buff[b++] = '%';buff[b++] = '2';buff[b++] = '7'; }
        else if (*iter == '(') { buff[b++] = '%';buff[b++] = '2';buff[b++] = '8'; }
        else if (*iter == ')') { buff[b++] = '%';buff[b++] = '2';buff[b++] = '9'; }
        else if (*iter == '*') { buff[b++] = '%';buff[b++] = '2';buff[b++] = 'A'; }
        else if (*iter == '+') { buff[b++] = '%';buff[b++] = '2';buff[b++] = 'B'; }
        else if (*iter == ',') { buff[b++] = '%';buff[b++] = '2';buff[b++] = 'C'; }
        else if (*iter == '-') { buff[b++] = '%';buff[b++] = '2';buff[b++] = 'D'; }
        else if (*iter == '.') { buff[b++] = '%';buff[b++] = '2';buff[b++] = 'E'; }
        else if (*iter == '/') { buff[b++] = '%';buff[b++] = '2';buff[b++] = 'F'; }
        //3
        else if (*iter == ':') { buff[b++] = '%';buff[b++] = '3';buff[b++] = 'A'; }
        else if (*iter == ';') { buff[b++] = '%';buff[b++] = '3';buff[b++] = 'B'; }
        else if (*iter == '<') { buff[b++] = '%';buff[b++] = '3';buff[b++] = 'C'; }
        else if (*iter == '=') { buff[b++] = '%';buff[b++] = '3';buff[b++] = 'D'; }
        else if (*iter == '>') { buff[b++] = '%';buff[b++] = '3';buff[b++] = 'E'; }
        else if (*iter == '?') { buff[b++] = '%';buff[b++] = '3';buff[b++] = 'F'; }
        //4
        else if (*iter == '@') { buff[b++] = '%';buff[b++] = '4';buff[b++] = '0'; }
        //5
        else if (*iter == '[') { buff[b++] = '%';buff[b++] = '5';buff[b++] = 'B'; }
        else if (*iter =='\\') { buff[b++] = '%';buff[b++] = '5';buff[b++] = 'C'; }
        else if (*iter == ']') { buff[b++] = '%';buff[b++] = '5';buff[b++] = 'D'; }
        else if (*iter == '^') { buff[b++] = '%';buff[b++] = '5';buff[b++] = 'E'; }
        else if (*iter == '_') { buff[b++] = '%';buff[b++] = '5';buff[b++] = 'F'; }
        //6
        else if (*iter == '`') { buff[b++] = '%';buff[b++] = '6';buff[b++] = '0'; }
        //7
        else if (*iter == '{') { buff[b++] = '%';buff[b++] = '7';buff[b++] = 'B'; }
        else if (*iter == '|') { buff[b++] = '%';buff[b++] = '7';buff[b++] = 'C'; }
        else if (*iter == '}') { buff[b++] = '%';buff[b++] = '7';buff[b++] = 'D'; }
        else if (*iter == '~') { buff[b++] = '%';buff[b++] = '7';buff[b++] = 'E'; }
        
        else {
            buff[b++] = *iter;
        }
        iter++;
    }

    return buff;
}

const char* MCString_percentDecode(const char* str, char *buff)
{
    if (!str || !buff) {
        return null;
    }
    char* iter = (char*)str; int b = 0;
    while (*iter != NUL) {
        //debug_log("MCString_percentDecode: %s\n", iter);
        if (*iter == '%') {
            iter++;
            if (*iter == '2') {
                iter++;
                if (*iter == '0') { buff[b++] = ' '; iter++;}
                else if (*iter == '1') { buff[b++] = '!'; iter++;}
                else if (*iter == '2') { buff[b++] = '"'; iter++;}
                else if (*iter == '3') { buff[b++] = '#'; iter++;}
                else if (*iter == '4') { buff[b++] = '$'; iter++;}
                else if (*iter == '5') { buff[b++] = '%'; iter++;}
                else if (*iter == '6') { buff[b++] = '&'; iter++;}
                else if (*iter == '7') { buff[b++] ='\''; iter++;}
                else if (*iter == '8') { buff[b++] = '('; iter++;}
                else if (*iter == '9') { buff[b++] = ')'; iter++;}
                else if (*iter == 'A') { buff[b++] = '*'; iter++;}
                else if (*iter == 'B') { buff[b++] = '+'; iter++;}
                else if (*iter == 'C') { buff[b++] = ','; iter++;}
                else if (*iter == 'D') { buff[b++] = '-'; iter++;}
                else if (*iter == 'E') { buff[b++] = '.'; iter++;}
            }
            else if (*iter == '3') {
                iter++;
                if (*iter == 'A') { buff[b++] = ':'; iter++;}
                else if (*iter == 'B') { buff[b++] = ';'; iter++;}
                else if (*iter == 'C') { buff[b++] = '<'; iter++;}
                else if (*iter == 'D') { buff[b++] = '='; iter++;}
                else if (*iter == 'E') { buff[b++] = '>'; iter++;}
                else if (*iter == 'F') { buff[b++] = '?'; iter++;}
            }
            else if (*iter == '4') {
                iter++;
                if (*iter == '0') { buff[b++] = '@'; iter++; }
            }
            else if (*iter == '5') {
                iter++;
                if (*iter == 'B') { buff[b++] = '['; iter++; }
                if (*iter == 'C') { buff[b++] ='\\'; iter++; }
                if (*iter == 'D') { buff[b++] = ']'; iter++; }
                if (*iter == 'E') { buff[b++] = '^'; iter++; }
                if (*iter == 'F') { buff[b++] = '_'; iter++; }
            }
            else if (*iter == '6') {
                iter++;
                if (*iter == '0') { buff[b++] = '`'; iter++; }
            }
            else if (*iter == '7') {
                iter++;
                if (*iter == 'B') { buff[b++] = '{'; iter++; }
                if (*iter == 'C') { buff[b++] = '|'; iter++; }
                if (*iter == 'D') { buff[b++] = '}'; iter++; }
                if (*iter == 'E') { buff[b++] = '~'; iter++; }
            }
        }
        else {
            buff[b++] = *iter;
            iter++;
        }
    }
    
    return buff;
}

const char* MCString_baseFromPath(const char* path, char (*buff)[])
{
    char reversebuff[PATH_MAX] = {0};
    size_t count = MCString_reverse(path, reversebuff);
    
    char* head = &reversebuff[count-1];
    char* tail = &reversebuff[0];
    while (!MCCond_PathDiv(tail) && *head != NUL) {
        tail++;
    }
    
    int i=0;
    while (head != tail) {
        (*buff)[i++] = *head;
        head--;
    }
    (*buff)[i] = NUL;
    
    return &(*buff)[0];
}

const char* MCString_filenameFromPath(const char* path, char (*buff)[])
{
    trimWhiteSpace(&path);

    char reversebuff[PATH_MAX] = {0};
    MCString_reverse(path, reversebuff);
    
    char* head = &reversebuff[0];
    char* tail = &reversebuff[0];
    while (!MCCond_PathDiv(head) && *head != NUL) {
        head++;
    }
    head--;
    
    int i=0;
    while (head != tail) {
        (*buff)[i++] = *head;
        head--;
    }
    (*buff)[i] = *tail;
    
    return &(*buff)[0];
}

size_t MCString_filenameTrimExtension(const char* name, char* buff)
{
    trimWhiteSpace(&name);
    char reversebuff[PATH_MAX] = {0};
    MCString_reverse(name, reversebuff);
    
    char* head = &reversebuff[0];
    char* tail = &reversebuff[0];
    
    while (1) {
        if (*tail != '.' && *tail != NUL) {
            tail++;
        }
        if (*head != NUL) {
            head++;
        } else {
            head--;//skip NUL
            break;
        }
    }
    //have extension
    if (*tail == '.') {
        tail++;
        size_t i=0;
        while (head != tail) {
            buff[i++] = *head;
            head--;
        }
        buff[i] = *head;//last char
        buff[i+1] = NUL;
        return i+1;
    }
    //no extension
    else {
        size_t szn = sizeof(name);
        strncpy(buff, name, szn);
        return szn;
    }
}

size_t MCString_extensionFromFilename(const char* name, char* basebuff, char* extbuff)
{
    trimWhiteSpace(&name);
    size_t i=0, j=0;
    while (name != NULL && name[i] != NUL) {
        if (name[i] == '.') {
            j = i;
        }
        i++;
    }
    //no extension
    if (j==0) {
        strcpy(basebuff, name);
        extbuff[0] = NUL;
        return 0;
    }
    //have extension
    else {
        size_t b, e, x=0;
        for (b=0; b<j; b++) {
            basebuff[b] = name[b];
        }
        for (e=j+1; e<i; e++) {
            extbuff[x++] = name[e];
        }
        basebuff[b] = NUL;
        extbuff[x]  = NUL;
        return e;
    }
}

const char* MCString_concate(const char** strings, size_t count, char (*buff)[])
{
    strcpy(*buff, strings[0]);
    int i;
    for (i=1; i<count; i++) {
        strcat(*buff, strings[i]);
    }
    return *buff;
}

const char* MCString_concateWith(const char* sp, const char* path1, const char* path2, char (*buff)[])
{
    return MCString_concate((const char* []){
        path1, sp, path2
    }, 3, buff);
}

const char* MCString_concatePath(const char* path1, const char* path2, char (*buff)[])
{
    return MCString_concateWith("/", path1, path2, buff);
}

const char* MCString_compressToCharCount(const char* source, char* buff)
{
    //assume buff is large enough
    if (source && buff) {
        //first char
        int cur = 0, i = 0;
        buff[cur++] = *source;
        char last   = *source;
        
        char digits[LINE_MAX];
        size_t count = 1;
        
        for (i=1; i<strlen(source); i++) {
            if(source[i] != last) {
                if (count > 1) {
                    sprintf(digits, "%ld", count);
                    int d;
                    for (d=0; d<strlen(digits); d++)
                        buff[cur++] = digits[d];
                    count = 0;
                }
                buff[cur++] = source[i];
                last = source[i];
            } else {
                if (count > LINE_MAX)
                    count = LINE_MAX;
                else
                    count++;
            }
        }
        //end char
        buff[cur] = NUL;
    }
    return buff;
}

const char* MCString_extractFromCharCount(const char* source, char* buff)
{
    if (source && buff) {
        long cur = 0, count=0, i=0, d=0;
        char last = NUL;
        char digits[LINE_MAX];
        char* next;

        for (i=0; i<strlen(source); i++) {
            char c = source[i];
            if (c >= '0' && c <= '9') {
                digits[count++] = c;
                continue;
            } else {
                if (count > LINE_MAX)
                    count = LINE_MAX;
                digits[count] = NUL;
                if (count > 0) {
                    long number = strtol(digits, &next, 10);
                    for(d=0; d<number; d++)
                        buff[cur++] = last;
                    count=0;
                }
                buff[cur++] = c;
                last = c;
            }
        }
        //end char
        buff[cur] = NUL;
    }
    return buff;
}

static void swap(char* a, char* b)
{
    if (a != b) {
        char temp = *a;
        *a = *b;
        *b = temp;
    }
}

static void permutationOf(char str[], int index)
{
    if (index == strlen(str)) {
        printf("%s\n", str);
        return;
    }
    int i;
    for (i=index; i<strlen(str); i++) {
        swap(&str[index], &str[i]);
        permutationOf(str, index+1);
        swap(&str[index], &str[i]);
    }
}

void MCString_printPermutationOf(char str[])
{
    char buff[LINE_MAX];
    strcpy(buff, str);
    buff[strlen(str)] = NUL;
    permutationOf(buff, 0);
}

MCString_t* MCString_newWithCString(const char* cstr)
{
    return MCString(alloc(MCString), cstr);
}

MCString_t* MCString_newWithMCString(MCString_t* mcstr)
{
    return MCString(alloc(MCString), mcstr->buff);
}

MCString_t* MCString_newForHttp(char* cstr, int isHttps)
{
	MCString_t* res;
	if (isHttps)
        res = MCString_newWithCString("https://");
	else
		res = MCString_newWithCString("http://");
	res->add(res, cstr);
	return res;
}

static char get_one_char()
{
    char cf = NUL;
    while(!isNewLine(&cf)) {
        cf = (char)getchar();
    }//clear the buff
	return cf;
}

static void get_chars_until_enter(char resultString[])
{
	char tc = NUL;
	int i=0;
	while(!isNewLine(&tc)){
		resultString[i]=tc;
		i++;
	}
	resultString[i]=NUL;
}

#define MCStringBlock LINE_MAX

fun(add, void), const char* str) as(MCString)
    if (MCStringBlock-it->size < strlen(str)+1) {
        char* newbuff = malloc(sizeof(char) * (it->size + MCStringBlock));
        strncpy(newbuff, it->buff, it->size-1);
        newbuff[it->size-1]=NUL;
        free(it->buff);
        it->buff = newbuff;
        it->size = it->size + MCStringBlock;
    }
    strncat(it->buff, str, strlen(str));
}

fun(print, void), bool withNewline) as(MCString)
    if (withNewline) {
        debug_log("%s\n", it->buff);
    } else {
        debug_log("%s", it->buff);
    }
}

fun(toCString, const char*), char const buff[]) as(MCString)
	strcpy((char*)buff, it->buff);
	return buff;
}

fun(equalTo, int), MCString_t* stringToComp) as(MCString)
	int res;
	res = strcmp(it->buff, stringToComp->buff);
	if (res==0)
		return 1;
	else
		return 0;
}

fun(getOneChar, char))
{
	return get_one_char();
}

fun(getCharsUntilEnter, void), char resultString[])
{
	get_chars_until_enter(resultString);
}

fun(startWith, bool), const char* str) as(MCString)
    size_t len = strlen(str);
    if (len > it->length) {
        return false;
    }else{
        if (strncmp(it->buff, str, len) == 0) {
            return true;
        }else{
            return false;
        }
    }
}

fun(toDoubleValue, double), char** endptr) as(MCString)
    debug_log("MCString toDoubleValue called\n");
    return strtod(it->buff, endptr);
}

fun(copyCompressedString, MCString_t*)) as(MCString)
    MCString_t* string = MCString(alloc(MCString), it->buff);
    MCString_compressToCharCount(it->buff, string->buff);
    return string;
}

fun(copyExtractedString, MCString_t*)) as(MCString)
    MCString_t* string = MCString(alloc(MCString), it->buff);
    MCString_extractFromCharCount(it->buff, string->buff);
    return string;
}

fun(release, void)) as(MCString)
    debug_log("MCString bye");
    free(it->buff);
}

constructor(MCString), const char* str) {
    MCObject(any);
    as(MCString)
        it->buff = null;
        it->size = 0;
        it->length = 0;

        if (str != null) {
            size_t len = strlen(str);
            if (len <= MCStringBlock) {
                len = MCStringBlock;
            }
            it->buff = malloc(len*sizeof(char));
            strncpy(it->buff, str, len);
            it->buff[len] = '\0';
            it->length = len;
            it->size = len + 1;
        }
    };
    dynamic(MCString)
        funbind(add);
        funbind(print);
        funbind(toCString);
        funbind(equalTo);
        funbind(getOneChar);
        funbind(getCharsUntilEnter);
        funbind(startWith);
        funbind(toDoubleValue);
        funbind(copyCompressedString);
        funbind(copyExtractedString);
        funbind(release);
    };
    return any;
}

#ifndef WIN32

#include "MCIO.h"

#include <limits.h>
#include "MCLog.h"
#include "MCString.h"

#ifndef LINE_MAX
#define LINE_MAX 2048
#endif

#ifndef PATH_MAX
#define PATH_MAX 2048
#endif

#pragma mark - MCFile unbuffered IO

void MCFile_flushAllCacheToDisk()
{
	sync();
}

int MCFile_flushAFileCacheToDisk(int fd)
{
    return fsync(fd);
}

int MCFile_isPathExist(char* pathname)
{
    //file exist test
    int res;
    if ((res = access(pathname, F_OK)) != -1)
        return 1;
    return 0;
}

int MCFile_chmod(char* pathname, mode_t mode)
{
    int res;
    if ((res = chmod(pathname, mode))!= -1)
        return 1;
    return 0;
}

int MFFile_truncateFileTo(char* pathname, off_t length)
{
    int res;
    if ((res = truncate(pathname, length))!= -1)
        return 1;
    return 0;
}

mode_t MCFile_setNewFilePermissionMask4Process(mode_t cmask)
{
    return umask(cmask);
}

struct MCFile* MCFile_newReadOnly(char* pathname)
{
    struct MCFile* file = MCFile(alloc(MCFile));
    return file->initWithPathName(file, pathname, O_RDONLY|O_CREAT);
}

struct MCFile* MCFile_newWriteOnly(char* pathname, int isClear)
{
    struct MCFile* file = MCFile(alloc(MCFile));
    if(isClear)
        return file->initWithPathName(file, pathname, O_WRONLY|O_CREAT|O_TRUNC);
    else
        return file->initWithPathName(file, pathname, O_WRONLY|O_CREAT);
}

struct MCFile* MCFile_newReadWrite(char* pathname, int isClear)
{
    struct MCFile* file = MCFile(alloc(MCFile));
    if(isClear)
        return file->initWithPathName(file, pathname, O_RDWR|O_CREAT|O_TRUNC);
    else
        return file->initWithPathName(file, pathname, O_RDWR|O_CREAT);
}

int MCFile_createSymbolLink(char* pathname, char* linkname)
{
    int res;
    if ((res = symlink(pathname, linkname))!= -1)
        return 1;
    return 0;
}

int MCFile_createDirectory(char* pathname)
{
    int res;
    if ((res = mkdir(pathname, S_IRWXU))!= -1)
        return 1;
    return 0;
}

int MCFile_removeDirectory(char* pathname)
{
    int res;
    if ((res = rmdir(pathname))!= -1)
        return 1;
    return 0;
}




fun(initWithPathName, struct MCFile*), char* pathname, int oflag) as(MCFile)
	if((it->fd = open(pathname, oflag, 0774))==-1)
		return null;
	it->pathname = pathname;
	if(fstat(it->fd, &it->attribute)<0)
		return null;
	it->buffer = malloc((size_t)it->attribute.st_blksize*10);
	return it;
}

fun(initWithPathNameDefaultFlag, struct MCFile*), char* pathname) as(MCFile)
    return initWithPathName(it, pathname, MCFileReadWriteTrunc);
}

fun(readFromBegin, ssize_t), off_t offset, size_t nbytes) as(MCFile)
    //use pread/pwrite for atomic operation
    return pread(it->fd, it->buffer, nbytes, offset);
}

fun(readAllFromBegin, ssize_t), off_t offset) as(MCFile)
    return readFromBegin(it, offset, (size_t)it->attribute.st_size);
}

fun(readAtLastPosition, size_t), off_t offset, size_t nbytes) as(MCFile)
    return (size_t)pread(it->fd, it->buffer, nbytes, offset);
}

fun(readFromEnd, size_t), off_t offset, size_t nbytes) as(MCFile)
    return (size_t)pread(it->fd, it->buffer, nbytes, offset);
}

fun(writeToBegin, size_t), off_t offset, void* buf, size_t nbytes) as(MCFile)
    return (size_t)pwrite(it->fd, buf, nbytes, offset);
}

fun(writeToLastTime, size_t), off_t offset, void* buf, size_t nbytes) as(MCFile)
    return (size_t)pwrite(it->fd, buf, nbytes, offset);
}

fun(writeToEnd, size_t), off_t offset, void* buf, size_t nbytes) as(MCFile)
    return (size_t)pwrite(it->fd, buf, nbytes, offset);
}

fun(duplicateFd, int)) as(MCFile)
    return dup(it->fd);
}

fun(duplicateFdTo, int), int fd) as(MCFile)
    return dup2(it->fd, fd);
}

fun(printAttribute, void)) as(MCFile)
    printf("uid:%d gid:%d size:%lld st_mode:%o lmtime:%s",
           it->attribute.st_uid,
           it->attribute.st_gid,
           it->attribute.st_size,
           it->attribute.st_mode,
           ctime(&it->attribute.st_mtime));
}

fun(checkPermissionUseRealIDOfProcess, int), int mode) as(MCFile)
    int res;
    if ((res = access(it->pathname, mode)) != -1)
        return 1;
    return 0;
}

fun(closeFile, void)) as(MCFile)
    //release it->buffer
    free(it->buffer);
    close(it->fd);
}

constructor(MCFile)) {
    MCObject(any);
    as(MCFile)
        it->fd = 0;
        it->pathname = "";
        it->buffer = null;
    }
    dynamic(MCFile)
        funbind(initWithPathName);
        funbind(initWithPathNameDefaultFlag);

        funbind(readAllFromBegin);
        funbind(readFromBegin);
        funbind(readAtLastPosition);
        funbind(readFromEnd);
        funbind(writeToBegin);
        funbind(writeToLastTime);
        funbind(writeToEnd);

        funbind(duplicateFd);
        funbind(duplicateFdTo);
        funbind(printAttribute);
        funbind(checkPermissionUseRealIDOfProcess);
        funbind(closeFile);
    }
    return any;
}

#pragma mark - MCStream buffered IO

fun(initWithPath, struct MCStream*), MCStreamType type, const char* path) as(MCStream)
    //FILE *fopen(const char *restrict pathname, const char *restrict type);
    //type:
    //r/w/a/ & b & +
    //int setvbuf(FILE *restrict fp, char *restrict buf, int mode, size_t size);
    //[NULL _IOFBF/_IOLBF/_IONBF BUFSIZ]
    
    char decodepath[PATH_MAX] = {0};
    it->fileObject = fopen(MCString_percentDecode(path, decodepath), type.fopenMode);
    if (it->fileObject) {
        //file size
        fseek(it->fileObject, 0, SEEK_END);
        long size = ftell(it->fileObject);
        fseek(it->fileObject, 0, SEEK_SET);
        
        it->buffer = (char*)malloc(size * sizeof(char));
        char* iter = it->buffer;
        
        char c;
        while ((c = (char)fgetc(it->fileObject)) != EOF) {
            *iter++ = c;
        }
        *iter = NUL;
        fseek(it->fileObject, 0, SEEK_SET);
    }
    
    return it;
}

fun(initWithPathDefaultType, struct MCStream*), const char* path) as(MCStream)
    return it->initWithPath(it, MakeMCStreamType(MCStreamBuf_FullBuffered, MCStreamOpen_ReadWrite), path);
}

fun(bye, void)) as(MCStream)
    if (it->buffer) {
        free(it->buffer);
    }
    //0=OK/NUL=ERROR
    if(fclose(it->fileObject))
        error_log("MCStream close file error\n");
}

fun(getFileDescriptor, int)) as(MCStream)
    return fileno(it->fileObject);
}

fun(getChar, int)) as(MCStream)
    return fgetc(it->fileObject);
}

fun(putChar, int), int charCode) as(MCStream)
    return fputc(charCode, it->fileObject);
}

fun(pushbackChar, int), int charCodeToBePushBack) as(MCStream)
    return ungetc(charCodeToBePushBack, it->fileObject);
}

fun(getCString, char*), MCCharBuffer* recvBuffer) as(MCStream)
    fgets(recvBuffer->data, (int)recvBuffer->size, it->fileObject);
    return recvBuffer->data;
}

fun(putCString, char*), MCCharBuffer* sendBuffer) as(MCStream)
    fputs(sendBuffer->data, it->fileObject);
    return sendBuffer->data;
}

fun(getMCString, struct MCString*)) as(MCStream)
    char buff[LINE_MAX];
    fgets(buff, sizeof(buff), it->fileObject);
    return MCString(alloc(MCString), &buff[0]);
}

fun(putMCString, int), struct MCString* str) as(MCStream)
    return fputs(str->buff, it->fileObject);
}

fun(getBianryObject, size_t), void* recvBuffer, size_t objectSize, size_t numberOfObjs) as(MCStream)
    //size_t fread(void *restrict ptr, size_t size, size_t nobj, FILE *restrict fp);
    return fread(recvBuffer, objectSize, numberOfObjs, it->fileObject);
}

fun(putBianryObject, size_t), void* sendBuffer, size_t objectSize, size_t numberOfObjs) as(MCStream)
    //size_t fwrite(const void *restrict ptr, size_t size, size_t nobj, FILE *restrict fp);
    return fwrite(sendBuffer, objectSize, numberOfObjs, it->fileObject);
}

fun(tellOffset, off_t)) as(MCStream)
    //off_t ftello(FILE *fp);
    return ftello(it->fileObject);
}

fun(seekFromBegin, int), off_t offset) as(MCStream)
    //int fseeko(FILE *fp, off_t offset, int whence);//SEEK_SET/SEEK_CUR/SEEK_END
    return fseeko(it->fileObject, offset, SEEK_SET);
}

fun(seekFromCurrent, int), off_t offset) as(MCStream)
    return fseeko(it->fileObject, offset, SEEK_CUR);
}

fun(seekFromEnd, int), off_t offset) as(MCStream)
    return fseeko(it->fileObject, offset, SEEK_END);
}

fun(tellSize, long)) as(MCStream)
    fseek(it->fileObject, 0, SEEK_END);
    long size = ftell(it->fileObject);
    rewind(it->fileObject);
    return size;
}

fun(dump, void)) as(MCStream)
    printf("%s", it->buffer);
}

constructor(MCStream)) {
    MCObject(any);
    as(MCStream)
        it->buffer = null;
    }
    dynamic(MCStream)
        funbind(initWithPath);
        funbind(initWithPathDefaultType);

        funbind(bye);
        funbind(getFileDescriptor);

        funbind(getChar);
        funbind(putChar);
        funbind(pushbackChar);

        funbind(getCString);
        funbind(putCString);
        funbind(getMCString);
        funbind(putMCString);

        funbind(getBianryObject);
        funbind(putBianryObject);

        funbind(tellOffset);
        funbind(seekFromBegin);
        funbind(seekFromCurrent);
        funbind(seekFromEnd);
        funbind(tellSize);
        funbind(dump);
    }
    return any;
}


#pragma mark - MCSelect

fun(initWithSecondAndMicrosec, void), long second, long microsecond) as(MCSelect)
    //timeout.tv_sec
    //timeout.tv_usec
    it->timeout.tv_sec = second;
    it->timeout.tv_usec = (int)microsecond;
}

fun(waitForFdsetChange, int)) as(MCSelect)
    it->readfd_result_set = it->readfd_set;
    it->writefd_result_set = it->writefd_set;
    it->exceptionfd_result_set = it->exceptionfd_set;
    
    return select(it->maxfd + 1,
                  &it->readfd_result_set,
                  &it->writefd_result_set,
                  &it->exceptionfd_result_set,
                  (it->timeout.tv_usec==0 && it->timeout.tv_sec==0)? NULL : &it->timeout);
}

fun(addFd, void), MCSelect_fd_type type, int fd) as(MCSelect)
    if(fd > it->maxfd) it->maxfd = fd;
    switch(type){
        case MCSelect_Readfd:
            FD_SET(fd, &it->readfd_set);
            break;
        case MCSelect_Writefd:
            FD_SET(fd, &it->writefd_set);
            break;
        case MCSelect_Exceptionfd:
            FD_SET(fd, &it->exceptionfd_set);
            break;
    }
}

fun(removeFd, void), MCSelect_fd_type type, int fd) as(MCSelect)
    switch(type){
        case MCSelect_Readfd:
            FD_CLR(fd, &it->readfd_set);
            break;
        case MCSelect_Writefd:
            FD_CLR(fd, &it->writefd_set);
            break;
        case MCSelect_Exceptionfd:
            FD_CLR(fd, &it->exceptionfd_set);
            break;
    }
}

fun(isFdReady, int), MCSelect_fd_type type, int fd) as(MCSelect)
    switch(type){
        case MCSelect_Readfd:
            return FD_ISSET(fd, &it->readfd_result_set);
            break;
        case MCSelect_Writefd:
            return FD_ISSET(fd, &it->writefd_result_set);
            break;
        case MCSelect_Exceptionfd:
            return FD_ISSET(fd, &it->exceptionfd_result_set);
            break;
    }
}

constructor(MCSelect)) {
    MCObject(any);
    as(MCSelect)
        FD_ZERO(&it->readfd_set);
        FD_ZERO(&it->writefd_set);
        FD_ZERO(&it->exceptionfd_set);

        FD_ZERO(&it->readfd_result_set);
        FD_ZERO(&it->writefd_result_set);
        FD_ZERO(&it->exceptionfd_result_set);
    }
    dynamic(MCSelect)
        funbind(initWithSecondAndMicrosec);
        funbind(waitForFdsetChange);
        funbind(addFd);
        funbind(removeFd);
        funbind(isFdReady);
    }
    return any;
}

#pragma mark - MCProgress

int MCProcess_changeCurrentWorkingDir(char* pathname)
{
	int res;
	if ((res = chdir(pathname))!= -1)
		return 1;
	return 0;
}

int MCProcess_changeCurrentWorkingDirByFd(int fd)
{
	int res;
	if ((res = fchdir(fd))!= -1)
		return 1;
	return 0;
}

char* MCProcess_getCurrentWorkingDir(MCCharBuffer* buff)
{
	return getcwd(buff->data, buff->size);
}

#endif








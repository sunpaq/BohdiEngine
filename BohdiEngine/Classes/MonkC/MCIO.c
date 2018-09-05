#include "MCIO.h"

#include <limits.h>
#ifndef LINE_MAX
#define LINE_MAX 2048
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

MCFile* MCFile_newReadOnly(char* pathname)
{
    return ff(new(MCFile), initWithPathName, pathname, O_RDONLY|O_CREAT);
}

MCFile* MCFile_newWriteOnly(char* pathname, int isClear)
{
    if(isClear)
        return ff(new(MCFile), initWithPathName, pathname, O_WRONLY|O_CREAT|O_TRUNC);
    else
        return ff(new(MCFile), initWithPathName, pathname, O_WRONLY|O_CREAT);
}

MCFile* MCFile_newReadWrite(char* pathname, int isClear)
{
    if(isClear)
        return ff(new(MCFile), initWithPathName, pathname, O_RDWR|O_CREAT|O_TRUNC);
    else
        return ff(new(MCFile), initWithPathName, pathname, O_RDWR|O_CREAT);
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

oninit(MCFile)
{
    if (init(MCObject)) {
        obj->fd = 0;
        obj->pathname = "";
        obj->buffer = null;
        //obj->attribute;
        return obj;
    }else{
        return null;
    }
}

fun(MCFile, MCFile*, initWithPathName, char* pathname, int oflag)
{
	if((obj->fd = open(pathname, oflag, 0774))==-1)
		return null;
	obj->pathname = pathname;
	if(fstat(obj->fd, &obj->attribute)<0)
		return null;
	obj->buffer = malloc(obj->attribute.st_blksize*10);
	return obj;
}

fun(MCFile, MCFile*, initWithPathNameDefaultFlag, char* pathname)
{
    return MCFile_initWithPathName(obj, pathname, MCFileReadWriteTrunc);
}

fun(MCFile, ssize_t, readAllFromBegin, off_t offset)
{
    return MCFile_readFromBegin(obj, offset, (size_t)obj->attribute.st_size);
}

fun(MCFile, ssize_t, readFromBegin, off_t offset, size_t nbytes)
{
    //use pread/pwrite for atomic operation
    return pread(obj->fd, obj->buffer, nbytes, offset);
}

fun(MCFile, size_t, readAtLastPosition, off_t offset, size_t nbytes)
{
    return pread(obj->fd, obj->buffer, nbytes, offset);
}

fun(MCFile, size_t, readFromEnd, off_t offset, size_t nbytes)
{
    return pread(obj->fd, obj->buffer, nbytes, offset);
}

fun(MCFile, size_t, writeToBegin, off_t offset, void* buf, size_t nbytes)
{
    return pwrite(obj->fd, buf, nbytes, offset);
}

fun(MCFile, size_t, writeToLastTime, off_t offset, void* buf, size_t nbytes)
{
    return pwrite(obj->fd, buf, nbytes, offset);
}

fun(MCFile, size_t, writeToEnd, off_t offset, void* buf, size_t nbytes)
{
    return pwrite(obj->fd, buf, nbytes, offset);
}

fun(MCFile, int, duplicateFd, voida)
{
    return dup(obj->fd);
}

fun(MCFile, int, duplicateFdTo, int fd)
{
    return dup2(obj->fd, fd);
}

fun(MCFile, void, bye, voida)
{
    //release obj->buffer
    free(obj->buffer);
    close(obj->fd);
}

fun(MCFile, void, printAttribute, voida)
{
    printf("uid:%d gid:%d size:%lld st_mode:%o lmtime:%s",
           obj->attribute.st_uid,
           obj->attribute.st_gid,
           obj->attribute.st_size,
           obj->attribute.st_mode,
           ctime(&obj->attribute.st_mtime));
}

fun(MCFile, int, checkPermissionUseRealIDOfProcess, int mode)
{
    int res;
    if ((res = access(obj->pathname, mode)) != -1)
        return 1;
    return 0;
}

onload(MCFile)
{
    if (load(MCObject)) {
        bid(MCFile, MCFile*, initWithPathName, char* pathname, int oflag);
        bid(MCFile, MCFile*, initWithPathNameDefaultFlag, char* pathname);
        
        bid(MCFile, size_t, readAllFromBegin, off_t offset);
        bid(MCFile, int, readFromBegin, off_t offset, size_t nbytes);
        bid(MCFile, int, readAtLastPosition, off_t offset, size_t nbytes);
        bid(MCFile, int, readFromEnd, off_t offset, size_t nbytes);
        bid(MCFile, int, writeToBegin, off_t offset, void* buf, size_t nbytes);
        bid(MCFile, int, writeToLastTime, off_t offset, void* buf, size_t nbytes);
        bid(MCFile, int, writeToEnd, off_t offset, void* buf, size_t nbytes);
        
        bid(MCFile, int, duplicateFd);
        bid(MCFile, int, duplicateFdTo, int fd);
        bid(MCFile, void, printAttribute);
        bid(MCFile, void, bye);
        bid(MCFile, int, checkPermissionUseRealIDOfProcess, int mode);
        return cla;
    }else{
        return null;
    }
}

#pragma mark - MCStream buffered IO

oninit(MCStream)
{
    if (init(MCObject)) {
        obj->buffer = null;
        return obj;
    }else{
        return null;
    }
}

fun(MCStream, MCStream*, initWithPath, MCStreamType type, const char* path)
{
    //FILE *fopen(const char *restrict pathname, const char *restrict type);
    //type:
    //r/w/a/ & b & +
    //int setvbuf(FILE *restrict fp, char *restrict buf, int mode, size_t size);
    //[NULL _IOFBF/_IOLBF/_IONBF BUFSIZ]
    
    char decodepath[PATH_MAX] = {0};
    obj->fileObject = fopen(MCString_percentDecode(path, decodepath), type.fopenMode);
    if (obj->fileObject) {
        //file size
        fseek(obj->fileObject, 0, SEEK_END);
        long size = ftell(obj->fileObject);
        fseek(obj->fileObject, 0, SEEK_SET);
        
        obj->buffer = (char*)malloc(size * sizeof(char));
        char* iter = obj->buffer;
        
        char c;
        while ((c = fgetc(obj->fileObject)) != EOF) {
            *iter++ = c;
        }
        *iter = NUL;
        fseek(obj->fileObject, 0, SEEK_SET);
    }
    
    return obj;
}

fun(MCStream, MCStream*, initWithPathDefaultType, const char* path)
{
    return MCStream_initWithPath(obj, MakeMCStreamType(MCStreamBuf_FullBuffered, MCStreamOpen_ReadWrite), path);
}

fun(MCStream, void, bye, voida)
{
    if (obj->buffer) {
        free(obj->buffer);
    }
    //0=OK/NUL=ERROR
    if(fclose(obj->fileObject))
        error_log("MCStream close file error\n");
    //other clean up works
    superbye(MCObject);
}

fun(MCStream, int, getFileDescriptor, voida)
{
    return fileno(obj->fileObject);
}

fun(MCStream, int, getChar, voida)
{
    return fgetc(obj->fileObject);
}

fun(MCStream, int, putChar, int charCode)
{
    return fputc(charCode, obj->fileObject);
}

fun(MCStream, int, pushbackChar, int charCodeToBePushBack)
{
    return ungetc(charCodeToBePushBack, obj->fileObject);
}

fun(MCStream, char*, getCString, MCCharBuffer* recvBuffer)
{
    fgets(recvBuffer->data, cast(int, recvBuffer->size), obj->fileObject);
    return recvBuffer->data;
}

fun(MCStream, char*, putCString, MCCharBuffer* sendBuffer)
{
    fputs(sendBuffer->data, obj->fileObject);
    return sendBuffer->data;
}

fun(MCStream, MCString*, getMCString, voida)
{
    char buff[LINE_MAX];
    fgets(buff, sizeof(buff), obj->fileObject);
    return ff(new(MCString), initWithCString, &buff[0]);
}

fun(MCStream, int, putMCString, MCString* str)
{
    return fputs(str->buff, obj->fileObject);
}

fun(MCStream, size_t, getBianryObject, void* recvBuffer,  size_t objectSize, size_t numberOfObjs)
{
    //size_t fread(void *restrict ptr, size_t size, size_t nobj, FILE *restrict fp);
    return fread(recvBuffer, objectSize, numberOfObjs, obj->fileObject);
}

fun(MCStream, size_t, putBianryObject, void* sendBuffer,  size_t objectSize, size_t numberOfObjs)
{
    //size_t fwrite(const void *restrict ptr, size_t size, size_t nobj, FILE *restrict fp);
    return fwrite(sendBuffer, objectSize, numberOfObjs, obj->fileObject);
}

fun(MCStream, off_t, tellOffset, voida)
{
    //off_t ftello(FILE *fp);
    return ftello(obj->fileObject);
}

fun(MCStream, int, seekFromBegin, off_t offset)
{
    //int fseeko(FILE *fp, off_t offset, int whence);//SEEK_SET/SEEK_CUR/SEEK_END
    return fseeko(obj->fileObject, offset, SEEK_SET);
}

fun(MCStream, int, seekFromCurrent, off_t offset)
{
    return fseeko(obj->fileObject, offset, SEEK_CUR);
}

fun(MCStream, int, seekFromEnd, off_t offset)
{
    return fseeko(obj->fileObject, offset, SEEK_END);
}

fun(MCStream, long, tellSize, voida)
{
    fseek(obj->fileObject, 0, SEEK_END);
    long size = ftell(obj->fileObject);
    rewind(obj->fileObject);
    return size;
}

fun(MCStream, void, dump, voida)
{
    printf("%s", obj->buffer);
}

onload(MCStream)
{
    if (load(MCObject)) {
        bid(MCStream, MCStream*, initWithPath, MCStreamType type, char* path);
        bid(MCStream, MCStream*, initWithPathDefaultType, const char* path);
        
        bid(MCStream, void, bye);
        bid(MCStream, int, getFileDescriptor);
        
        bid(MCStream, int, getChar);
        bid(MCStream, int, putChar, int charCode);
        bid(MCStream, int, pushbackChar, int charCodeToBePushBack);
        
        bid(MCStream, char*, getCString, MCCharBuffer* recvBuffer);
        bid(MCStream, char*, putCString, MCCharBuffer* sendBuffer);
        bid(MCStream, MCString*, getMCString);
        bid(MCStream, MCString*, putMCString, MCString* str);
        
        bid(MCStream, size_t, getBianryObject, void* recvBuffer,  size_t objectSize, size_t numberOfObjs);
        bid(MCStream, size_t, putBianryObject, void* sendBuffer,  size_t objectSize, size_t numberOfObjs);
        
        bid(MCStream, off_t, tellOffset);
        bid(MCStream, int, seekFromBegin, off_t offset);
        bid(MCStream, int, seekFromCurrent, off_t offset);
        bid(MCStream, int, seekFromEnd, off_t offset);
        bid(MCStream, size_t, tellSize, voida);
        bid(MCStream, void, dump, voida);
        
        return cla;
    }else{
        return null;
    }
}

#pragma mark - MCSelect

oninit(MCSelect)
{
    if (init(MCObject)) {
        FD_ZERO(&obj->readfd_set);
        FD_ZERO(&obj->writefd_set);
        FD_ZERO(&obj->exceptionfd_set);
        
        FD_ZERO(&obj->readfd_result_set);
        FD_ZERO(&obj->writefd_result_set);
        FD_ZERO(&obj->exceptionfd_result_set);
        return obj;
    }else{
        return null;
    }
}

fun(MCSelect, void, initWithSecondAndMicrosec, long second, long microsecond)
{
    //timeout.tv_sec
    //timeout.tv_usec
    obj->timeout.tv_sec = second;
    obj->timeout.tv_usec = cast(int, microsecond);
    return;
}

fun(MCSelect, int, waitForFdsetChange, voida)
{
    obj->readfd_result_set = obj->readfd_set;
    obj->writefd_result_set = obj->writefd_set;
    obj->exceptionfd_result_set = obj->exceptionfd_set;
    
    return select(obj->maxfd + 1,
                  &obj->readfd_result_set,
                  &obj->writefd_result_set,
                  &obj->exceptionfd_result_set,
                  (obj->timeout.tv_usec==0 && obj->timeout.tv_sec==0)? NULL : &obj->timeout);
}

fun(MCSelect, void, addFd, MCSelect_fd_type type, int fd)
{
    if(fd > obj->maxfd) obj->maxfd = fd;
    switch(type){
        case MCSelect_Readfd:
            FD_SET(fd, &obj->readfd_set);
            break;
        case MCSelect_Writefd:
            FD_SET(fd, &obj->writefd_set);
            break;
        case MCSelect_Exceptionfd:
            FD_SET(fd, &obj->exceptionfd_set);
            break;
    }
}

fun(MCSelect, void, removeFd, MCSelect_fd_type type, int fd)
{
    switch(type){
        case MCSelect_Readfd:
            FD_CLR(fd, &obj->readfd_set);
            break;
        case MCSelect_Writefd:
            FD_CLR(fd, &obj->writefd_set);
            break;
        case MCSelect_Exceptionfd:
            FD_CLR(fd, &obj->exceptionfd_set);
            break;
    }
}

fun(MCSelect, int, isFdReady, MCSelect_fd_type type, int fd)
{
    switch(type){
        case MCSelect_Readfd:
            return FD_ISSET(fd, &obj->readfd_result_set);
            break;
        case MCSelect_Writefd:
            return FD_ISSET(fd, &obj->writefd_result_set);
            break;
        case MCSelect_Exceptionfd:
            return FD_ISSET(fd, &obj->exceptionfd_result_set);
            break;
    }
}

onload(MCSelect)
{
    if (load(MCObject)) {
        bid(MCSelect, void, initWithSecondAndMicrosec, long second, long microsecond);
        bid(MCSelect, int, waitForFdsetChange);
        bid(MCSelect, void, addFd, MCSelect_fd_type type, int fd);
        bid(MCSelect, void, removeFd, MCSelect_fd_type type, int fd);
        bid(MCSelect, int, isFdReady, MCSelect_fd_type type, int fd);
        return cla;
    }else{
        return null;
    }
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








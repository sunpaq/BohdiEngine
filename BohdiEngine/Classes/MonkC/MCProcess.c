#include "MCProcess.h"

oninit(MCProcess)
{
    if (init(MCObject)) {
        obj->pid=getpid();
        obj->ppid=getppid();
        obj->uid=getuid();
        obj->euid=geteuid();
        obj->gid=getgid();
        obj->egid=getegid();
        return obj;
    }else{
        return null;
    }
}

fun(MCProcess, void, printIDs, voida)
{
	printf("pid[%d]ppid[%d]uid[%d]euid[%d]gid[%d]egid[%d]\n", 
		obj->pid, obj->ppid, obj->uid, obj->euid, obj->gid, obj->egid);
}

fun(MCProcess, int, fork, voida)
{
	//pid_t fork(void);
	//copy-on-write (COW)
	//typically a "page" in a virtual memory system
	return fork();
}

fun(MCProcess, int, registerAtExitCallback, void (*func)(void))
{
	if(atexit(func)==0)
		return 0;//success
	else
		return -1;//error
}

fun(MCProcess, void, exitWithStatus, int status)
{
	//this is a system call
	_exit(status);
}

fun(MCProcess, pid_t, waitAnyChildExit, int* statusAddr)
{
	return wait(statusAddr);
}

fun(MCProcess, pid_t, waitPIDChildExit, pid_t pid, int* statusAddr, int options)
{
	return waitpid(pid, statusAddr, options);
}

fun(MCProcess, int, isChildExitNormal, int status)
{
	if(WIFEXITED(status))
		return 1;
	else
		return 0;
}

fun(MCProcess, int, getChildExitLowOrder8Bit, int status)
{
	return WEXITSTATUS(status);
}

fun(MCProcess, int, isChildExitBySignal, int status)
{
	if (WIFSIGNALED(status))
		return 1;
	else
		return 0;
}

fun(MCProcess, int, getChildTerminateSignal, int status)
{
	return WTERMSIG(status);
}

fun(MCProcess, int, isCoreDumpFileGenerated, int status)
{
	return WCOREDUMP(status);
}

fun(MCProcess, int, isChildStopped, int status)
{
	if (WIFSTOPPED(status))
		return 1;
	else
		return 0;
}

fun(MCProcess, int, getChildStopSignal, int status)
{
	return WSTOPSIG(status);
}

//fun(MCProcess, int, isChildContinued, int status)
//{
//	if(WIFCONTINUED(status))
//		return 1;
//	else
//		return 0;
//}

//fun(MCProcess, pid_t, waitAnyChildExitGetResourceUseage, int* statusAddr, int options, MCProcessRUseage* useage)
//{
//	return wait3(statusAddr, options, useage->rusage_p);
//}

fun(MCProcess, pid_t, waitPIDChildExitGetResourceUseage, pid_t pid, int* statusAddr, int options, MCProcessRUseage* useage)
{
	return wait4(pid, statusAddr, options, useage->rusage_p);
}

onload(MCProcess)
{
    if (load(MCObject)) {
        bid(MCProcess, void, printIDs);
        bid(MCProcess, int, fork);
        
        bid(MCProcess, int, registerAtExitCallback, void (*func)(void));
        bid(MCProcess, void, exitWithStatus, int status);
        bid(MCProcess, pid_t, waitAnyChildExit, int* statusAddr);
        
        bid(MCProcess, pid_t, waitPIDChildExit, pid_t pid, int* statusAddr, int options);
        bid(MCProcess, int, isChildExitNormal, int status);
        bid(MCProcess, int, getChildExitLowOrder8Bit, int status);
        bid(MCProcess, int, isChildExitBySignal, int status);
        bid(MCProcess, int, getChildTerminateSignal, int status);
        bid(MCProcess, int, isCoreDumpFileGenerated, int status);
        bid(MCProcess, int, isChildStopped, int status);
        bid(MCProcess, int, getChildStopSignal, int status);
        //bid(MCProcess, int, isChildContinued, int status);
        //bid(MCProcess, pid_t, waitAnyChildExitGetResourceUseage, int* statusAddr, int options, MCProcessRUseage* useage);
        bid(MCProcess, pid_t, waitPIDChildExitGetResourceUseage, pid_t pid, int* statusAddr, int options, MCProcessRUseage* useage);
        return cla;
    }else{
        return null;
    }
}

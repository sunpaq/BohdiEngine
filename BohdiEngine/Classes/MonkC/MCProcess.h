#ifndef WIN32

#include "MCContext.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

/*
pid 0 is a swapper(scheduler)
pid 1 is a init
*/

#ifndef MCProcess_
#define MCProcess_ 

typedef enum {
	wait_no_option=0,
	wait_continued=WCONTINUED,
	wait_no_hang=WNOHANG,
	//The waitpid function will not block if a child specified by pid is not immediately available. In
	//this case, the return value is 0.
	wait_untraced=WUNTRACED
	//If the implementation supports job control, the status of any child specified by pid that has
	//stopped, and whose status has not been reported since it has stopped, is returned.
} MCProcessOptions;

typedef struct MCProcessRUseage_struct {
	struct rusage* rusage_p;
	char* description;
} MCProcessRUseage;

structure(MCProcess, MCObject)
	pid_t pid;
	pid_t ppid;
	uid_t uid;
	uid_t euid;
	gid_t gid;
	gid_t egid;

	fundef(printIDs, void));
	//returns(0 in child/child-pid in parent/-1 on error)
	fundef(forkProcess, int));

	//may be not supported by OS
	fundef(registerAtExitCallback, int), void (*func)(void));
	fundef(exitWithStatus, void), int status);
	fundef(waitAnyChildExit, pid_t), int* statusAddr);

	fundef(waitPIDChildExit, pid_t), pid_t pid, int* statusAddr, int options);

	fundef(isChildExitNormal, int), int status);
	fundef(getChildExitLowOrder8Bit, int), int status);

	fundef(isChildExitBySignal, int), int status);
	fundef(getChildTerminateSignal, int), int status);
	fundef(isCoreDumpFileGenerated, int), int status);

	fundef(isChildStopped, int), int status);
	fundef(getChildStopSignal, int), int status);
	fundef(waitPIDChildExitGetResourceUseage, pid_t), pid_t pid, int* statusAddr, int options, MCProcessRUseage* useage);
};

constructor(MCProcess));

alias(MCProcess);

#endif
/*
The origin APIs:
(i strongly recommand to use the origin API than these wrappers
however these wrappers only as a document for esay )

pid_t getpid(void);
pid_t getppid(void);
uid_t getuid(void);
uid_t geteuid(void);
gid_t getgid(void);
gid_t getegid(void);

pid_t fork(void);
pid_t vfork(void);

SIGCHLD
#include <sys/wait.h>
pid_t wait(int *statloc);
pid_t waitpid(pid_t pid, int *statloc, int options);

WIFEXITED(status) //True if status was returned for a child that terminated normally.
WEXITSTATUS (status) //to fetch the low-order 8 bits of the argument that the child passed.

WIFSIGNALED(status)
WTERMSIG(status)

WCOREDUMP(status)

WIFSTOPPED(status)
WSTOPSIG(status)

WIFCONTINUED(status) //for waitpid only
int atexit(void (*func)(void));

zombie is cause by unresponsed living father

1. responsed father will clean the child death.
2. father dead init will clean the child death.

fork twice and exit first one.
will force child death be management by init.
to avoid unresponsed father.

by calling fork twice, [grandfather, father, child] and father exit
can avoid child became zombie process(parent did not handle his death)
[grandfather, x, child] child became orphan, will management by init(pid=1) process

*/

#endif

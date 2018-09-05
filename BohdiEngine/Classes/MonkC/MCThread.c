#include "MCThread.h"

oninit(MCThread)
{
	//init the vars
	pthread_once_t ponce = PTHREAD_ONCE_INIT;
	obj->once_control = ponce;
	obj->isRunOnce = 0;//default is NO
	//if you need, you can set the attribute use the raw pthread APIs
	//example: pthread_attr_getstacksize(m_thread->attribute);
	pthread_attr_init(&obj->attribute);
    
    obj->functionPointer = null;
    obj->functionArgument = null;
	return obj;
}

fun(MCThread, void, bye, voida)
{
    pthread_attr_destroy(&obj->attribute);
}

fun(MCThread, MCThread*, initWithFPointerArgument, void* fp, void* farg)
{
    if (fp==null)
    {
        error_log("%s\n","fp can not be nil, do nothing");
        return null;
    }
    obj->functionPointer = fp;
    obj->functionArgument = farg;
    return obj;
}

fun(MCThread, MCThread*, initWithFPointer, void* fp)
{
    return MCThread_initWithFPointerArgument(obj, fp, null);
}

fun(MCThread, int, detach, voida)
{
    return pthread_detach(obj->tid);
}

fun(MCThread, int, start, voida)
{
    int res;
    if (obj->isRunOnce==1)
    {
        res = pthread_once(&(obj->once_control), obj->functionPointer);
        
    }else{
        
        res = pthread_create(&obj->tid,//tid, pthread_t type
                             &obj->attribute,
                             obj->functionPointer,
                             obj->functionArgument);
    }
    return res;
}

fun(MCThread, int, equal, MCThread* thread)
{
    return pthread_equal(obj->tid, thread->tid);
}

util(MCThread, int, cancelThread, pthread_t tid)
{
#ifdef __APPLE__
	return pthread_cancel(tid);
#elif __ANDROID__
	return 0;
#else
	return pthread_cancel(tid);
#endif
}

util(MCThread, int, joinThread, pthread_t tid)
{
    //did not pass an returen value pointer
    return pthread_join(tid, NULL);
}

util(MCThread, void, exitWithStatus, void* status)
{
    pthread_exit(status);
}

util(MCThread, pthread_t, currentThread)
{
    return pthread_self();
}

onload(MCThread)
{
    if (load(MCObject)) {
        bid(MCThread, void, bye, voida);
        bid(MCThread, MCThread*, initWithFPointerArgument, void* fp, void* farg);
        bid(MCThread, MCThread*, initWithFPointer, void* fp);
        bid(MCThread, int, detach, voida);
        bid(MCThread, int, start, voida);
        bid(MCThread, int, equal, MCThread* thread);
        return cla;
    }else{
        return null;
    }
}


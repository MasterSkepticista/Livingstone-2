/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// threadobject.cpp for unix platforms (pthreads)

#include <realtime_api/posix/threadobject.h>
#include <livingstone/L2_assert.h>

ThreadObject::ThreadObject() 
{
	int err = pthread_attr_init(&thr_attr);
	L2_assert(!err, L2_resource_error,("Error in ThreadObject constructor while attempting to initialize pthread attribute."));
	pthread_attr_setscope(&thr_attr, PTHREAD_SCOPE_SYSTEM);
}

ThreadObject::~ThreadObject()
{
}

void ThreadObject::start_thread()
{
	int err = pthread_create(&thread_id, &thr_attr, thread_func, this);
	L2_assert(!err, L2_resource_error,("Error in ThreadObject::start_thread while attempting to create pthread."));
}

void ThreadObject::wait_for_exit()
{
	pthread_cancel(thread_id);
	pthread_join(thread_id, NULL); 
}

void *ThreadObject::thread_func( void * param)
{
	ThreadObject *pobj = (ThreadObject *) param;
	return pobj->thread_member_func();
}

void *ThreadObject::thread_member_func(){ return 0; }


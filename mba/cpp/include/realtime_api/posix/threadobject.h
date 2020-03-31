/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// threadobject.h for posix

#ifndef _THREAD_OBJECT_H
#define _THREAD_OBJECT_H

#include <livingstone/l2conf.h>

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>



class ThreadObject
{
public:
	// constructor which takes thread attribute structure as argument
	ThreadObject( pthread_attr_t attr );
	ThreadObject(); // uses default thread attributes, except for joinable
	virtual ~ThreadObject();

	void start_thread();
	void wait_for_exit();

	// static function needed to invoke thread_member_func
	// since system won't automatically push this pointer onto stack
	static void * thread_func( void * param);

protected:
	// the actual function which will run in a separate thread
	virtual void *thread_member_func();
	
	pthread_t thread_id;
	pthread_attr_t thr_attr;

};

#endif

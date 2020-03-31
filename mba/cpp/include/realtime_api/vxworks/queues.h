/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

#ifndef QUEUES_H
#define QUEUES_H

#include <msgQLib.h>
#include <mqueue.h>  

#define MAX_BUF_SIZE 100
/*
#define MSG_PRI_NORMAL 5;
#define MSG_PRI_URGENT 10;
#define MSG_PRI_DEFAULT -1;
*/

enum msgqids  {MQIDTI_LIV=0, MQIDTI_DIS, MQIDTI_END};

extern mqd_t mqid_tbl[MQIDTI_END +1];

void readMsgQ(int mqid, void *buf, int priority);
int sendMsgQ(int mqid, void *buf, size_t size, int priority);


typedef struct {
   unsigned int seconds;
   unsigned int nanosec;
} TIME_TAG;

/* message: command, arg1(command or observable), arg2(value), arg3(time delay)*/

typedef struct {
   int    msgType;
   unsigned int arg1;
   int    arg2;
   long   arg3;
   float  arg4;
   TIME_TAG Time;
} NDAS_DATA;


#endif /* QUEUES_H */



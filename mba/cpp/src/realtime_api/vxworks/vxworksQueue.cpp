/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// vxworksQueue.cpp

#include <realtime_api/vxworks/queues.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


void ltoa(long n, char s[]);
void reverse(char s[]);

int MAX_MSGS = 100;


void createVxWorksQueue(int mqid)
{
   qTable[mqid] = msgQCreate(MAX_MSGS, 100, MSG_Q_FIFO);
   fprintf(stderr,"created vxworks message queue\n");
}


void add_message_to_queue(void *msg, int mqid)
{
   long pointer = (long)msg;
   char msg_ptr[20];
   ltoa(pointer, msg_ptr); 

   /* fprintf(stderr,"adding msg at %s to Q %d\n", msg_ptr, mqid);
      fprintf(stderr,"aka at %ld\n", pointer); */
   
   msgQSend(qTable[mqid], msg_ptr, strlen (msg_ptr) + 1, WAIT_FOREVER, MSG_PRI_NORMAL);
}



void *get_message_from_queue(int mqid) 
{
     long pointer;
     char *msg_ptr = (char *)calloc(20, 1);

     msgQReceive(qTable[mqid], msg_ptr, 20, WAIT_FOREVER);

     pointer = atol(msg_ptr);

     /* fprintf(stderr,"getting msg at %s to Q %d\n", msg_ptr, mqid);
        fprintf(stderr,"aka at %ld\n", pointer);
     */

     return (void *)pointer;
}


void ltoa(long n, char s[])
{
  int i, sign;

  if ((sign = n) < 0)
    n = -n;
  i = 0;

  do {
    s[i++] = n % 10 + '0';
  } while ((n /= 10) > 0);

  if (sign < 0)
    s[i++] = '-';
  s[i] = '\0';
  reverse(s);
}


void reverse(char s[])
{
  int c, i, j;

  for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

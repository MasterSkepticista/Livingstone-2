/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/***************************************************************************/
/*                                                                         */
/*    FILE: posixqueue.c                                                   */
/*                                                                         */
/***************************************************************************/

/***************************************************************************/
/*                                                                         */
/* This program or documentation was funded and developed by the National  */
/* Aeronautics and Space Administration, Glenn Research Center, and is     */
/* disseminated under the sponsorship of the National Aeronautics and      */
/* Space Administration.  The United States Government makes no represen-  */
/* tation or warranties, express or implied, including without limitation  */
/* any representations or warranties as to merchantability or fitness for  */
/* a particular purpose respecting this software package.                  */
/*                                                                         */
/* This computer program is intended for installation on a hardware/soft-  */
/* ware configuration which is identical to the configuration on which the */
/* program was developed.  The National Aeronautics and Space Administra-  */
/* tion or the United States Government cannot provide assurance that this */
/* program will execute properly on any hardware other then that on which  */
/* it was developed.  The National Aeronautics and Space Administration    */
/* and the United States Government also do not represent this program as  */
/* being suitable for any applications other than those for which it was   */
/* developed.                                                              */
/***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
/*#include "global.h" */
#include <realtime_api/vxworks/queues.h>


/*
 * this is included only for purposes of debugging.  it, and all calls to
 * outputMsg should be removed for the NITEX demo -cef
 */
/*#include "monitorIO_Utils.h" */
enum monErrors { EXCESSIVE_DATA_RATE=-45,
                 MISSING_ARGS,
                 QUEUE_FULL,
                 NO_DATA,
                 GEN_ERROR,
                 SUCCESS=1};



/*@header_start@*/
/***************************************************************************/
/*                                                                         */
/*  Routine: void * readMsgQ(MsgQIDType mqid, void *buf, int priority)     */
/*                                                                         */
/*  Purpose: waits for a successful read of the specified message queue.   */
/*           returns the incoming buffer.                                  */
/*                                                                         */
/*  Inputs :                                                               */
/*  --------                                                               */
/*  buf      - valid pointer to an input structure;                        */
/*  mqid     - the message queue identifier;                               */
/*  priority - expected priority of message or -1 if 'don't care'          */
/*                                                                         */
/*  Returned Values:                                                       */
/*  ----------------                                                       */
/*  pointer to the incoming message buffer.                                */
/*                                                                         */
/***************************************************************************/
/*  Local Variables:                                                       */
/*  ----------------                                                       */
/*  pri - priority of incoming message;                                    */
/*                                                                         */
/***************************************************************************/
/*  Modification History:                                                  */
/*  ---------------------                                                  */
/*  07/20/2000 - Initial code:  to try to insulate monitors from changes   */
/*               in the message queue methodology.  Currently working with */
/*               POSIX (per 7/19/2000 telecon)                             */
/*  01/08/2000 - moved to a more global file for everyone to use;          */
/***************************************************************************/
/*@header_end@*/

void readMsgQ(int mqid, void *buf, int priority){

   int pri;

   if(priority == -1)
      pri = 0;
   else
      pri = priority;

   while(1){
      if(mq_receive(mqid_tbl[mqid], buf, MAX_BUF_SIZE, &pri) == -1){
         fprintf(stderr,"readMsgQ: error reading message queue\n");
        /*outputMsg(MON_DEBUGGING,"readMsgQ: error reading message queue\n");*/
      }
      else{
         return;
      }
   }
}

/*@header_start@*/
/***************************************************************************/
/*                                                                         */
/*  Routine: int sendMsgQ(int mqid, void *buf, size_t size, int priority)  */
/*                                                                         */
/*  Purpose: sends out a message to the local message que.                 */
/*                                                                         */
/*  Inputs :                                                               */
/*  --------                                                               */
/*  buf      - valid output buffer;                                        */
/*  mqid     - the message queue identifier.                               */
/*  pri      - expected priority of message or -1;                         */
/*  size     - size of the outgoing message;                               */
/*                                                                         */
/*  Returned Values:                                                       */
/*  ----------------                                                       */
/*  SUCCESS or error code;                                                 */
/*                                                                         */
/***************************************************************************/
/*  Local Variables:                                                       */
/*  ----------------                                                       */
/*  pri    - priority code for the message sent to the queue;              */
/*  status - return code from subroutine calls;                            */
/*                                                                         */
/***************************************************************************/
/*  Modification History:                                                  */
/*  ---------------------                                                  */
/*  07/20/2000 - Initial code:  to try to insulate monitors from changes   */
/*               in the message queue methodology                          */
/*                                                                         */
/***************************************************************************/
/*@header_end@*/

int sendMsgQ(int mqid, void *buf, size_t size, int priority){

   int pri;
   int status;

   if(priority == -1)
      pri = MSG_PRI_NORMAL;
   else
      pri = priority;

   if((status = mq_send(mqid_tbl[mqid], buf, size, pri)) == 0){
      return SUCCESS;
   }
   else{
      if(errno == EAGAIN){
           fprintf(stderr,"sendMsgQ: queue appears to be full\n");
           /*outputMsg(MON_DEBUGGING,"sendMsgQ: queue appears to be full\n");*/
          return QUEUE_FULL;
      }
      else{
          fprintf(stderr,"sendMsgQ: general error\n");
          /*outputMsg(MON_DEBUGGING,"sendMsgQ: general error\n");*/
          return GEN_ERROR;
      }
   }
}

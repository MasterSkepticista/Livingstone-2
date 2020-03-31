/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// api test
#include "vxWorks.h"
#include "kernelLib.h"
#include "taskLib.h" 
#include <stdlib.h>
#include "ioLib.h"
#include <iostream.h>
#include <time.h>
#include "sysLib.h" 

#include <realtime_api/vxworks/queues.h>
#include <realtime_api/vxworks/livapi_rt_debug.h>
#include <realtime_api/vxworks/livingstone_dispatcher.h>
#include <realtime_api/userfcns_debug.h>
#include <livingstone/livingstone_debug.h>
#include "x34_time_delays.h"


#ifndef WIN32
#include <unistd.h>
#endif

int monitorTask, realTimeTask, dispatcherTask;

mqd_t mqid_tbl[MQIDTI_END +1];

class CBFS_tracker;
class Cover_tracker;

L2_rtapi_debug *realTime;
LivingstoneDispatcher *dispatcher;


extern void startMonitors();

int max_rank;
int dummy;
int length;
char *model;



MEM_PART_STATS myPartStats;
unsigned long dwOrigBytesAlloc;
timespec aTime;
unsigned long dwMilliseconds;


void usage()
{
	cout << "Usage:\n";
	cout << "l2test-rt <model-name> [scenario-file]\n";
	cout << "l2test-rt <model-name> [scenario-file] cbfs [#-candidates] [search-size] [length]\n";
	cout << "l2test-rt <model-name> [scenario-file] cover [max-rank] [length]\n";
}


LivingstoneMessage xlate(NDAS_DATA monitorMsg)
{
  LivingstoneMessage *msg = new
    LivingstoneMessage((livingstone_message_calls)monitorMsg.msgType,
		       monitorMsg.arg1, monitorMsg.arg2, 0);

   return *msg;
}


void startL2realTime()
{
   cout << "Livingstone Real-Time API spawned." << endl;
  
   realTime = NULL;
   realTime = new L2_rtapi_debug ();

   realTime->processMonitorData();

}


void startDispatcher(int max_rank, int dummy, int length, char *model)
{
   cout << "Livingstone Dispatcher spawned." << endl;

   cout << "Instantiating Livingstone" << endl << endl;
   cout << "  Search settings:" << endl;
   cout << "     Search type    = conflict coverage" << endl; 
   cout << "     Max Rank       = " << max_rank << endl;
   cout << "     History length = " << length << endl << endl;

   Livingstone<Cover_tracker> livingstone(max_rank,dummy,length);
   Livingstone_debug<Cover_tracker_debug, Cover_tracker> livdebug(&livingstone, cout);
   livingstone.read(model); //"/tgtsvr/cb");
   ReportTransitionsDebug liv_rep(&livingstone.tracker);

   dispatcher = NULL;
   dispatcher = new LivingstoneDispatcher(&livingstone.tracker, liv_rep);
   L2_assert(dispatcher, L2_resource_error("Error while attempting to
allocate LivingstoneDispatcher object"));

   dispatcher->thread_member_func();

}


void createPosixQueue(char *qName, int mqid)
{
   struct mq_attr newattr;

   newattr.mq_msgsize = MAX_BUF_SIZE;
   newattr.mq_maxmsg  = 200;
   newattr.mq_flags = 0;

   if((mqid_tbl[mqid] = mq_open(qName, O_CREAT | O_RDWR, 0, &newattr)) == 
       (mqd_t)ERROR)
   {
      fprintf(stderr,"couldn't create message queue %s \n", qName);
      /* if already created */
      mq_unlink("TimerMsgQ");
      mq_unlink("CmdMsgQ");
      mq_unlink("LivMsgQ");
      mq_unlink("ValveTimerMsgQ"); 
      exit(EXIT_FAILURE);
   }

}


void initializeQueues()
{
   createPosixQueue("LivMsgQ", MQIDTI_LIV);  

   createPosixQueue("DisMsgQ", MQIDTI_DIS);

}



void startTasks()
{

        // enable round-robin scheduling
        kernelTimeSlice(20);


        dispatcherTask = taskSpawn("Dispatcher", 75, 0, 16000,
                                       (FUNCPTR) startDispatcher,
                                       max_rank,dummy,length,model, 
                                        0, 0, 0, 0, 0, 0);


        taskDelay(18 * sysClkRateGet());


        realTimeTask = taskSpawn("RealTime", 75, 0, 1000,
                                (FUNCPTR) startL2realTime,
                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);                       

                                         
        monitorTask = taskSpawn("Monitor", 75, 0, 1000,
                                (FUNCPTR) startMonitors,
                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);           


        taskDelay(8 * sysClkRateGet());

}


/*        
   parameters: model file name
   monitorStub reads in scenario file specifying commands and arguments 
   (including sleeps)-- error check all
   monitorStub enqueues msgs to realtime in order read from the scenario file.
   Start up Livingstone as before with dispatcher
   realtime api enqueues msgs to dispatcher
   */

void RxU()
{

   initializeQueues();

   startTasks();

}



int setupParameters(int argc,char** argv)
{

    if (argc < 2) {
        usage();
        return(-1);
    }

    // check args and instantiate parameterized Livingstone accordingly
    //    if (argc==2 || (infile && argc==3) || (!infile && !strcmp("cover",argv[2])) || (infile && !strcmp("cover",argv[3]))) {

        max_rank = 10;
      //  int length = Tracker::DEFAULT_HISTORY_LENGTH;
        length = 20; // history length screwing up right now

        //if (argc >= (4+infile)) max_rank = atoi(argv[(3+infile)]);
        //if (argc == (5+infile)) length = atoi(argv[(4+infile)]);

        /// The constructor to cover_tracker has a dummy arg to give it the
        /// same args as the CBFS tracker.  This was made to accomodate the
        /// way the JNI interface was written.  
        /// \todo  Look into the JNI and see if the dummy arg to cover_tracker() is necessary.

        dummy=0;
        model = argv[1];

        return 0;
}




void testRxU(char *szCmdLine, char *szInput, char *szOutput) {
  int nInFd, nOutFd;

  if(szInput)
  {
    // Since szInput wasn't NULL, the user must want to read commands from a
    // file. So open an already existing file so that we can read from it.
    nInFd = open(szInput, O_RDONLY, 0);

    // Redirect input from a file.
    ioTaskStdSet(0,0,nInFd);
  }

  if(szOutput)
  {
    // Since szOutput wasn't NULL the user must want to output the display
    // text to a file. To do this we creat a new file and open it for
    // writing. If the file already exist, then we overwrite it.
    nOutFd = creat(szOutput, O_RDWR);

    // Redirect output to a file.
    ioTaskStdSet(0,1,nOutFd);
  }

  // Take a snapshot of memory for later memory leak troubleshooting.
  if((memPartInfoGet(memSysPartId,&myPartStats))!=ERROR)
    dwOrigBytesAlloc = myPartStats.numBytesAlloc;;

  // Start the clock to see how long it will take to read in a model.
  clock_gettime(CLOCK_REALTIME, &aTime);
  dwMilliseconds = (aTime.tv_sec * 1000 + aTime.tv_nsec / 1000000);

  // Command line declarations.
  char *argv[7];
  int argc = 0;

  // First argument value is the name of the program, but VxWorks doesn't
  // really care. As far as it's concern, the value can be zero.
  argv[argc] = "testLivingstone";

  // Get the first token.
  argv[++argc] = strtok(szCmdLine, " ");

  // Find subsequent tokens.
  while(argv[argc] != 0 && argc < 6)
    argv[++argc] = strtok(0, " ");

  setupParameters(argc, argv);

  // Run the real McCoy!
  RxU();

  // Close the input and output files as necessary.
  if(szInput)
    close(nInFd);
  if(szOutput)
    close(nOutFd);
}


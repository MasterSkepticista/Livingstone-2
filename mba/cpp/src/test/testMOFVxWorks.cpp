/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

#include <iostream.h>


#include <livingstone/livingstone.h>
#include <livingstone/livingstone_debug.h>
#include <stdlib.h>
#include "memLib.h"
#include <time.h>
#include "ioLib.h"

class CBFS_tracker;
class Cover_tracker;

MEM_PART_STATS myPartStats;
unsigned long dwOrigBytesAlloc;
timespec aTime;
unsigned long dwMilliseconds;


void usage(char* program_name) {
    cout << "Usage: " << endl;
    cout << "       " << program_name << " <model-name>" << endl;
    cout << "       " << program_name << " <model-name> cbfs [#-candidates] [search-size] [length]" << endl;
    cout << "       " << program_name << " <model-name> cover [max-rank] [length]" << endl;
}


int testMOF(int argc,char** argv) 

{ 

    if (argc < 2) {
        usage(argv[0]);
        return(-1);
    }

    // check args and instantiate parameterized Livingstone accordingly
    if (argc==2 || !strcmp("cover",argv[2]))
    {

        if (argc > 5) {
            usage(argv[0]);
            return(-1);
        }


        int max_rank = 10;
        int length = Tracker::DEFAULT_HISTORY_LENGTH;

        if (argc >= 4) max_rank = atoi(argv[3]);
        if (argc == 5) length = atoi(argv[4]);

        /// The constructor to cover_tracker has a dummy arg to give it the
        /// same args as the CBFS tracker.  This was made to accomodate the
        /// way the JNI interface was written.  
        /// \todo   Look into the JNI and see if the dummy arg to cover_tracker() is necessary.

        int dummy=0;

        cout << "Instantiating Livingstone" << endl << endl;
        cout << "  Search settings:" << endl;
        cout << "     Search type    = conflict coverage" << endl; 
        cout << "     Max Rank       = " << max_rank << endl;
        cout << "     History length = " << length << endl << endl;


        Livingstone<Cover_tracker> livingstone(max_rank,dummy,length);
        Livingstone_debug<Cover_tracker_debug,Cover_tracker> debugger(&livingstone, cout);
        livingstone.read(argv[1]);

        cout <<  endl << endl;
        debugger.listener();
    }
    else
    {
        if (argc > 6) {
            usage(argv[0]);
            return(-1);
        }

        int number_t = 1;
        int number_m = 100;
        int length = Tracker::DEFAULT_HISTORY_LENGTH;
        if (argc >= 4) number_t = atoi(argv[3]);
        if (argc >= 5) number_m = atoi(argv[4]);
        if (argc == 6) length = atoi(argv[5]);

        cout << "Instantiating Livingstone" << endl << endl;
        cout << "  Search settings:" << endl;
        cout << "     Search type             =  CBFS" << endl; 
        cout << "     Max candidates returned = " << number_t << endl;
        cout << "     Max candidates searched = " << number_m << endl;
        cout << "     History length          = " << length << endl << endl;

        Livingstone<CBFS_tracker> livingstone(number_t, number_m, length);
        Livingstone_debug<CBFS_tracker_debug,CBFS_tracker> debugger(&livingstone,cout);
        livingstone.read(argv[1]);

        cout <<  endl << endl;
        debugger.listener();
    } 
    return(0);

}

void testL2(char *szCmdLine, char *szInput, char *szOutput) {
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
  argv[argc] = "testMOF";

  // Get the first token.
  argv[++argc] = strtok(szCmdLine, " ");

  // Find subsequent tokens.
  while(argv[argc] != 0 && argc < 6)
    argv[++argc] = strtok(0, " ");

  // Run the real McCoy!
  testMOF(argc, argv);

  // Close the input and output files as necessary.
  if(szInput)
    close(nInFd);
  if(szOutput)
    close(nOutFd);
}
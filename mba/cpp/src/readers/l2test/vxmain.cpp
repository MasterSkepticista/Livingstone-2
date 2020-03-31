/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

#include <ioLib.h>
#include <memLib.h>
#include <time.h>
#include <string.h>

#define NUMBER_OF_ARGUMENTS 15

// Prototype main.
int main(int, char**);


// Variables for measuring performance.
static MEM_PART_STATS myPartStats;
static unsigned long dwOrigBytesAlloc;
static timespec aTime;
static unsigned long dwMilliseconds;


// VxWorks does not have this implemented, so we do it ourselves.
char* strdup(const char *string)
{
    char *memory;

    // Do some boundary checking.
    if (!string)
        return NULL;

    memory = new char [strlen(string) + 1];

    if(memory)
        return strcpy(memory, string);

    return NULL;
}

// VxWorks' programs are not console applications; therefore, a command line
// is not tokenized and sent as an array of arguments. To overcome this
// problem is to pass the command line as a zero terminated string, in which
// case we tokenized it ourselves and pass it to the main function.
int l2test(char *szCmdLine, char *szInput, char *szOutput) {
    int nInFd, nOutFd;

    // Initialize.
    nInFd = nOutFd = 0;

    if(szInput)
    {
        // Since szInput wasn't NULL, the user must want to read commands
        // from a file. So open an already existing file so that we can
        // read from it.
        nInFd = open(szInput, O_RDONLY, 0);

        // Redirect input from a file.
        ioTaskStdSet(0,0,nInFd);
    }

    if(szOutput)
    {
        // Since szOutput wasn't NULL the user must want to output the
        // display text to a file. To do this we create a new file and open
        // it for writing. If the file already exist, then we overwrite it.
        nOutFd = creat(szOutput, O_RDWR);

        // Redirect output to a file.
        ioTaskStdSet(0,1,nOutFd);
    }

    // Take a snapshot of memory for later memory leak troubleshooting.
    if((memPartInfoGet(memSysPartId,&myPartStats))!=ERROR)
        dwOrigBytesAlloc = myPartStats.numBytesAlloc;

    // Start the clock to see how long it will take to read in a model.
    clock_gettime(CLOCK_REALTIME, &aTime);
    dwMilliseconds = (aTime.tv_sec * 1000 + aTime.tv_nsec / 1000000);

    // Command line declarations.
    char *argv[NUMBER_OF_ARGUMENTS];
    int argc = 0;

    // First argument value is the name of the program, but VxWorks doesn't
    // really care. As far as it's concern, the value can be zero.
    argv[argc] = "L2test";

    // Get the first token.
    // Note: strtok skips additional spaces, allowing "  cbfs   1000"
    //       also note that strtok is not reentrant.
    argv[++argc] = strtok(szCmdLine, " ");

    // Find subsequent tokens.
    while(argv[argc] != 0 && argc < NUMBER_OF_ARGUMENTS - 1)
        argv[++argc] = strtok(0, " ");

    // Run the real McCoy!
    int ret = main(argc, argv);

    // Close the input and output files as necessary.
    if(szInput)
        close(nInFd);
    if(szOutput)
        close(nOutFd);

    return ret;
}

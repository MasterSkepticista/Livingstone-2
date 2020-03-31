/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

#include <fstream.h>
#include "taskLib.h" 
#include "sysLib.h" 


#include <realtime_api/livingstonemessage.h>
#include <realtime_api/vxworks/queues.h>



unsigned int NUMBER_VALID_MSG_TYPES = 8;
const unsigned int WAIT = REPORT_FULL_STATE + 1;


// this function reads in an enum scenario and and stores it in the msg_array
// if error found in input, returns number of bad input line


int read_scenario(unsigned int &how_many, ifstream &fin)
{
        how_many = 0;

        int type, arg1, arg2;

        fin >> type;

        while ( !fin.eof() && (how_many<1024)) 
        {
                if (type > NUMBER_VALID_MSG_TYPES)
                {cout << "Input error: Invalid message function on msg " << how_many;
                return how_many;}

                switch (type)
                {
                case COMMAND:
                case OBSERVATION:
                case START_COMMAND_AND_TIME:
                case START_OBSERVE_AND_TIME:
                        fin >> arg1;
                        fin >> arg2;
                        break;
                case WAIT:
                        fin >> arg1;
                        arg2 = 0;
                        taskDelay(arg1 * sysClkRateGet());
                        break;
                case FIND_CANDIDATES:
                case REPORT_FULL_STATE:
                        break;
                }
                how_many++;
                if (how_many == 1024)
                        return 0;

                if (type != WAIT) {
                   NDAS_DATA *msg = (NDAS_DATA *)malloc(sizeof(NDAS_DATA));

                   msg->msgType = type;
                   msg->arg1 = arg1; 
                   msg->arg2 = arg2; 

                   cout << "  Monitor adding msg " << msg->msgType << ", " <<
                     msg->arg1 << endl;
                   //add_message_to_queue(msg, MQIDTI_LIV);
                   int status = sendMsgQ(MQIDTI_LIV, (void *) msg, sizeof(NDAS_DATA), MSG_PRI_NORMAL);
                }

                fin >> type;
        }

        return 0;
}



void startMonitors()
{
   cout << "Monitors spawned." << endl;

   // this is the scenario script
   ifstream fin;
   int infile=0;

       fin.open("scenario.rt");
       if (!fin) {
                cout << "Input file \"scenario.rt\" not found. \n";
       }

   unsigned int nmsgs = 0;
   int error = 0;

   while (error == 0)
     // read and send monitor msgs to the realtime api queue
      error = read_scenario(nmsgs, fin);
               
}


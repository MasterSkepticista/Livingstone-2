/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

#include <iostream.h>


#include <livingstone/livingstone.h>
#include <livingstone/livingstone_debug.h>
#include <stdlib.h>
#include <livingstone/L2_assert.h>

class CBFS_tracker;
class Cover_tracker;

void usage(char* program_name) {
    cout << "Usage: " << endl;
    cout << "       " << program_name << " <model-name>" << endl;
    cout << "       " << program_name << " <model-name> cbfs [#-candidates] [search-size] [length]" << endl;
    cout << "       " << program_name << " <model-name> cover [max-rank] [length]" << endl;
}



/// For running the profiler without having to try to redirect standard
/// input.  This test program can be used with the model in 
///  cpp/tests/cb.jmpl, which has to be compiled to cb.xmpl 
#undef MBA_PROFILE


#ifdef MBA_PROFILE

void pause() {

 cout << "Press return to coninue.." << endl;
 char line[MBA_MAX_LINE_SIZE];
 cin.getline(line,MBA_MAX_LINE_SIZE);
	}

void test_program() {

    int max_rank=100;
	int dummy=0;
    int length=5;
    char* model_name = "c:\\kurien\\work\\mba\\cpp\\tests\\cb\\cb";

	
	Livingstone<Cover_tracker> livingstone(max_rank,dummy,length);
	Livingstone_debug<Cover_tracker_debug,Cover_tracker> debugger(&livingstone, cout);
    livingstone.read(model_name);


	//args cannot be a constant string.  Should fix this.
 char args[MBA_MAX_LINE_SIZE];
 
 //pause();
 strcpy(args,"cbAndLed.cb15.cmdIn=on");
 debugger.parse_cmd("progress", args);

 strcpy(args,"cbAndLed.cb14.cmdIn=on");
 debugger.parse_cmd("progress", args);

 strcpy(args,	 "cbAndLed.cb12.cmdIn=on");
 debugger.parse_cmd("progress", args);

 strcpy(args,	 "cbAndLed.cb8.cmdIn=on");
 debugger.parse_cmd("progress", args);

 strcpy(args,	 "cbAndLed.led8.ledState=off");
 debugger.parse_cmd("assign", args);


 strcpy(args,"");
 debugger.parse_cmd("fc",args);


 strcpy(args,	 "cbAndLed.cb8.cmdIn=reset");
 debugger.parse_cmd("progress", args);

 strcpy(args,	 "cbAndLed.cb8.cmdIn=on");
 debugger.parse_cmd("progress", args);

 strcpy(args,	 "cbAndLed.led8.ledState=on");
 debugger.parse_cmd("assign", args);

 strcpy(args,"");
 debugger.parse_cmd("fc",args);

}
#endif

int main(int argc,char** argv) { 

#ifdef MBA_PROFILE
    test_program();
	exit(0);
#endif

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




		try {

        Livingstone<Cover_tracker> livingstone(max_rank,dummy,length);
        Livingstone_debug<Cover_tracker_debug,Cover_tracker> debugger(&livingstone, cout);
        livingstone.read(argv[1]);

        cout <<  endl << endl;
        debugger.listener(); 
			}
		catch (L2_error lobj) { cout << lobj.error_string << "\n"; }

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

		try {

        Livingstone<CBFS_tracker> livingstone(number_t, number_m, length);
        Livingstone_debug<CBFS_tracker_debug,CBFS_tracker> debugger(&livingstone,cout);
        livingstone.read(argv[1]);

        cout <<  endl << endl;
		debugger.listener();
	}
		catch (L2_error lobj) { cout << lobj.error_string << "\n"; }



			} 
    return(0);

}



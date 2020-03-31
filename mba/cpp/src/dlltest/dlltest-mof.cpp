/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// dlltest-mof.cpp

#include <iostream.h>


#include <livingstone/Livingstone.h>
#include <livingstone/Livingstone_debug.h>
#include <stdlib.h>
#include <livdll/livdll.h>

class CBFS_tracker;
class Cover_tracker;

int main(int argc,char** argv) 

{ 

  if (argc < 2) {
	  cout << "Usage: " << endl;
	  cout << "       " << argv[0] << " <model-name>" << endl;
	  cout << "       " << argv[0] << " <model-name> cbfs [#-candidates]" << endl;
	  cout << "       " << argv[0] << " <model-name> cover " << endl;
	  return(-1);
	  }

	// check args and instantiate parameterized Livingstone accordingly

  if (!strcmp("cover",argv[2]))
  {
	int max_rank = 10;
	if (argc == 4) max_rank = atoi(argv[3]);

	cout << "Instantiating Livingstone to use conflict coverage search.\n";
	Livingstone<Cover_tracker> livingstone(max_rank);
	Livingstone_debug<Cover_tracker_debug,Cover_tracker> debugger(&livingstone, cout);
	livingstone.read(argv[1]);
	debugger.listener();
  }
  else
  {
	int number_t = 1;
	if (argc == 4) number_t = atoi(argv[3]);

	cout << "Instantiating Livingstone to use conflict-directed best-first search with candidates tracked = " << number_t << ".\n";
	Livingstone<CBFS_tracker> livingstone(number_t);
	Livingstone_debug<CBFS_tracker_debug,CBFS_tracker> debugger(&livingstone,cout);
	livingstone.read(argv[1]);
	debugger.listener();
  } 
  return(0);

}

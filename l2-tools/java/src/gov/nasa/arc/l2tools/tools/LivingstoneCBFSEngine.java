//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
* Author : Mark Shirley
* Description : LivingstoneCBFSEngine.java
*
* CHANGELOG:
* 05/06/00      Mhs     Created.
*/

package gov.nasa.arc.l2tools.tools;

public class LivingstoneCBFSEngine extends LivingstoneEngine2 {

    public LivingstoneCBFSEngine(int ptr, int debug_ptr) {
        super(ptr, debug_ptr);
        search_tracker_type = LivingstoneEngine2.CBFS;
    }

    public static int setCBFSEngineParams (int max_candidate_classes_returned,
                                           int max_candidates_returned,
                                           int max_candidates_searched,
                                           int max_cutoff_weight,
                                           int max_history_cutoff,
                                           int max_trajectories_tracked,
                                           String progress_cmd_type, String fc_cmd_type) {
      int L2_ptr = setCBFSEngineParamsJNI( max_candidate_classes_returned,
                                           max_candidates_returned,
                                           max_candidates_searched,
                                           max_cutoff_weight, max_history_cutoff,
                                           max_trajectories_tracked,
                                           progress_cmd_type, fc_cmd_type);
      return L2_ptr;
    }

    public static LivingstoneCBFSEngine factory( int L2_ptr) {
      int ptr = allocateCBFSEngineJNI( L2_ptr);
      // create debugger object
      int debug_ptr = allocateCBFSDebuggerJNI ( ptr);
      LivingstoneCBFSEngine e = new LivingstoneCBFSEngine( ptr, debug_ptr);
      engines.add(e);
      return e;
    }
}

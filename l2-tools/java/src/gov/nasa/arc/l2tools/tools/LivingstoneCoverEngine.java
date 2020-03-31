//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
* Author : Mark Shirley
* Description : LivingstoneCoverEngine.java
*
* CHANGELOG:
* 05/06/00      Mhs     Created.
*/

package gov.nasa.arc.l2tools.tools;

public class LivingstoneCoverEngine extends LivingstoneEngine2 {

    public LivingstoneCoverEngine(int ptr, int debug_ptr) {
        super( ptr, debug_ptr);
        search_tracker_type = LivingstoneEngine2.COVER;
    }

    public static int setCoverEngineParams (int max_candidate_rank, int max_history_cutoff, int max_trajectories_tracked, String progress_cmd_type, String fc_cmd_type) {
      int L2_ptr = setCoverEngineParamsJNI( max_candidate_rank, max_history_cutoff, max_trajectories_tracked, progress_cmd_type, fc_cmd_type);
      return L2_ptr;
    }

    public static LivingstoneCoverEngine factory( int L2_ptr) {
      int ptr = allocateCoverEngineJNI( L2_ptr);
      // create debugger object
      int debug_ptr = allocateCoverDebuggerJNI( ptr);
      LivingstoneCoverEngine e = new LivingstoneCoverEngine( ptr, debug_ptr);
      engines.add(e);
      return e;
    }
}

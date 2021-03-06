package LivingstoneCorba;


/**
* LivingstoneCorba/LivingstoneEngineManagerOperations.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from LivingstoneCorba.idl
* Thursday, April 20, 2006 4:53:49 PM PDT
*/

public interface LivingstoneEngineManagerOperations 
{
  String describe ();
  LivingstoneCorba.LivingstoneCommandLine getCommandLine (String search_method, int max_candidate_classes_returned, int max_candidates_returned, int max_candidates_searched, int max_cutoff_weight, int max_history_cutoff, int max_trajectories_tracked, String progress_cmd_type, String fc_cmd_type);
  LivingstoneCorba.LivingstoneCommandLine getRunningCommandLine ();
  void exit ();
} // interface LivingstoneEngineManagerOperations

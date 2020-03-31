/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

//
// Non-flight debugging/inspection code for Conflict_db
//

#include <ctype.h> // isdigit
#include <stdlib.h>
#include <debuggers/tracker_debug.h>
#include <livingstone/L2_assert.h>
#include <mba_utils/algos.h>

/***************************************************************************
  Mapping from user-input fc styles to an index.
 ***************************************************************************/

// This array is used to map between name strings for the find-candidates style
// and the corresponding Tracker::FC_style enum
// If you have multiple strings for the same value, put the one you
// want printed out first.

Tracker_debug::FC_dict_item Tracker_debug::fc_dict[] = {
  // this first one is just for printing really
  { "the tracker default fc", Tracker::tracker_default_fc },
  { "default",	              Tracker::tracker_default_fc },
  { "none",	              Tracker::tracker_default_fc },
  { "extend",	              Tracker::extend_fc },
  { "prune-and-search",	      Tracker::prune_and_search_fc },
  { "prune-search",	      Tracker::prune_and_search_fc },
  { "ps",	              Tracker::prune_and_search_fc },
  { "find-fresh",	      Tracker::find_fresh_fc },
  { "ff",	              Tracker::find_fresh_fc },
  { "findfresh",              Tracker::find_fresh_fc },
  // this one must be last ; name is ignored
  { "invalid",	              Tracker::invalid_fc }
};


/**
 * Map the name string for the find-candidates style onto its FC_style enum.
 * If not found, return Tracker_debug::invalid
 * \todo Use String_key_dict_item in array_dict.h
 */

Tracker::FC_style Tracker_debug::find_style(MBA_string name) {
  unsigned i;
  for (i = 0; fc_dict[i].style != Tracker::invalid_fc; ++i) {
    if (strcmp(name.c_str(), fc_dict[i].name) == 0) {
      break; // found it
    }
  }
  return fc_dict[i].style;
}


/**
 * Map the FC_style enum onto the name string for the find-candidates style.
 * If not found, return "invalid"
 * \todo Use String_key_dict_item in array_dict.h
 */

MBA_string Tracker_debug::find_style(Tracker::FC_style style) {
  unsigned i;
  for (i = 0; fc_dict[i].style != Tracker::invalid_fc; ++i) {
    if (style == fc_dict[i].style)
      break;
  }
  return fc_dict[i].name;
}


/***************************************************************************
 Constructor
 ***************************************************************************/

Tracker_debug::Tracker_debug(Tracker *system,
			     L2_string_map& map,
			     _STD_ ostream& outstream) :
  T_system_debug(system->get_t_system(), map, outstream),
  tracker(system),
  stream(outstream),
  nconflicts(0)
#ifdef ENABLE_CHECKPOINT
  ,
  ckpt_by_name(hashpjw),
  ckpt_by_id(hash_integral_key),
  nckpt(0),
  last_ckpt_id(0),
  t_system(system->get_t_system())
#endif
{ }


// If the argument string is empty, print a message and return the null pointer
// Otherwise, map the argument string onto an Assignment

Assignment* Tracker_debug::getCommandAssignment(const char *args) {
  if (0 == strlen(args)) {
    stream << "Issuing idle transition" << _STD_ endl;
    return 0;
  } else {
    return find_assignment(args, true);
  }
}


// Set the progress style

void Tracker_debug::doProgressStyle(const char *cmd, const char *args) {
  if (0 != strlen(args)) {
    if (!strcmp(args, "min")) {
#ifndef DISABLE_MIN_PROGRESS
      t_system.set_progress_uses_full(false);
#else
      L2_throw(L2_disabled_error,
	       ("min-progress was disabled at compile-time\n"));
#endif
    } else if(!strcmp(args, "full")) {
#ifndef DISABLE_FULL_PROGRESS
      t_system.set_progress_uses_full(true);
#else
      L2_throw(L2_disabled_error,
	       ("full-progress was disabled at compile-time\n"));
#endif
    } else {
      L2_assert(false, L2_parse_scenario_error,
		(MBA_string("invalid progress style `") +args+"'"));
    }
  }
  stream << "progress means "
	 << (t_system.get_progress_uses_full() ? "full" : "min")
	 << "-progress\n";
}


// Print all Candidates

void Tracker_debug::doPrintCandidates(const char *cmd, const char *args) {
  stream <<  "The " << t_system.getCandidates().size()
	 << " candidates are:" << _STD_ endl;
  print_candidates();
}


// Print all the Candidate equivalence classes

void Tracker_debug::doPrintClasses(const char *cmd, const char *args) {
  if (tracker->size() == 0) {
    stream << "There are no Candidates" << _STD_ endl;
    // No need to return; the loop will have 0 iterations
  } else if ((tracker->getCandidatePartition().size() == 0)) {
    // Partition the Candidates
    tracker->partition();
  }
  const Array<CandidateEqClass*, false>& candidatePartition =
    tracker->getCandidatePartition();
  for (unsigned i = 0; i < candidatePartition.size(); i++) {
    const CandidateEqClass* const candidateEqClass = candidatePartition[i];
    stream << "Equivalence Class " << i << _STD_ endl;
    for (unsigned j = 0; j < candidateEqClass->size(); j++) {
      const Candidate* const pCandidate = candidateEqClass->get(j);
      const unsigned index = tracker->get_t_system().getIndex(*pCandidate);
      stream << "   Candidate " << index << ")" << _STD_ endl;
      print_candidate(stream, *pCandidate, 8);
    }
  }
}


// Install the Candidate specified by its index

void Tracker_debug::doInstall(const char *cmd, const char *args) {
  int candidate_num = atoi(args);
  L2_assert(candidate_num >= 0 &&
	    (unsigned)candidate_num < t_system.getCandidates().size(),
	    L2_bounds_error,
	    ("candidates list", candidate_num,
	     t_system.getCandidates().size()));
  t_system.install(candidate_num);
}


// Truncate at the specified horizon time step

void Tracker_debug::doTruncate(const char *cmd, const char *args) {
  begin_timing();
  if (strlen(args) == 0)  { t_system.truncate_if_needed(); }
  else                    { t_system.truncate(atoi(args)); }
  end_timing();
}


// Set the find-candidates style

void Tracker_debug::doFindCandidatesStyle(const char *cmd, const char *args) {
  MBA_string args_str (args);
  if (args_str == "help") {
    _STD_ cout << "allowable fc-styles are:\n";
    for (unsigned i=0; fc_dict[i].style != Tracker::invalid_fc; ++i) {
      _STD_ cout << '\t' << fc_dict[i].name << _STD_ endl;
    }
  } else if (args_str != "") {
    Tracker::FC_style style = find_style(args);
    L2_assert(style != Tracker::invalid_fc,
	      L2_argument_error,
	      ("fc style `" + args_str + "' is invalid; try `fc-style help'"));
    tracker->set_fc_style(style);
  }
  _STD_ cout << "fc now does " << find_style(tracker->get_fc_style())
	    << _STD_ endl;
}


// Perform find candidates according to the current find-candidates style

void Tracker_debug::doFindCandidates(const char *cmd, const char *args) {
  switch(tracker->get_fc_style()) {
  case Tracker::tracker_default_fc:
    // Leave it to the Tracker to determine the find-candidates style
    start_find_candidates();
    tracker->find_candidates();
    end_find_candidates();
    break;
  case Tracker::extend_fc:
    start_find_candidates();
    tracker->extend_candidates();
    end_find_candidates();
    break;
  case Tracker::find_fresh_fc:
    doFindFreshCandidates(cmd, args);
    break;
  case Tracker::prune_and_search_fc:
    doPruneAndSearch(cmd, args);
    break;
  default:
    L2_throw(L2_argument_error,
	     ("fc style is invalid"));
    break;
  }
}


// Perform find candidate according to the prune-and-search find-candidates
// style

void Tracker_debug::doPruneAndSearch(const char *cmd, const char *args) {
  start_find_candidates();
  tracker->prune_and_search();
  end_find_candidates();
}


// Perform find candidate according to the find-fresh-candidates
// find-candidates style

void Tracker_debug::doFindFreshCandidates(const char *cmd, const char *args) {
  start_find_candidates();
  tracker->find_fresh_candidates();
  end_find_candidates();
}


// Print both the Tracker and Transition System statistics

void Tracker_debug::doPrintAllStats(const char *cmd, const char *args) {
  print_stats();
  T_system_debug::parse_cmd(cmd, args);
}


void Tracker_debug::doAddAssumption(const char *cmd, const char *args) {
  Assignment *pAssignment = find_assignment(args, true);
  if (pAssignment != NULL) { t_system.addAssumption(pAssignment); }
}


// Interpret the command string

bool Tracker_debug::parse_cmd(const char* cmd, const char* args) {
  // These commands are common to all trackers
  if (!strcmp("progress", cmd)) {
    progress(getCommandAssignment(args));
  } else if (!strcmp("full-progress", cmd)) {
    full_progress(getCommandAssignment(args));
  } else if (!strcmp("min-progress", cmd)) {
    minimal_progress(getCommandAssignment(args));
  } else if (!strcmp("progress-style", cmd)) {
    doProgressStyle(cmd, args);
  } else if (!strcmp("candidates", cmd)) {
    doPrintCandidates(cmd, args);
  } else if (!strcmp("install", cmd)) {
    doInstall(cmd, args);
  } else if (!strcmp("truncate", cmd)) {
    doTruncate(cmd, args);
  } else if (!strcmp("fc-style", cmd)) {
    doFindCandidatesStyle(cmd, args);
  } else if (!strcmp("find-candidates",cmd) ||
	     !strcmp("fc",cmd) ){
    doFindCandidates(cmd, args);
  } else if (!strcmp("prune-search", cmd) ||
	     !strcmp("ps", cmd)) {
    doPruneAndSearch(cmd, args);
  } else if (!strcmp("find-fresh", cmd) ||
	     !strcmp("ff", cmd)) {
    doFindFreshCandidates(cmd, args);
  } else if (!strcmp("tracker-stats", cmd)) {
    print_stats();
  } else if (!strcmp("stats", cmd)) {
    doPrintAllStats(cmd, args);
  } else if (!strcmp("classes", cmd)) {
    doPrintClasses(cmd, args);
  } else if (!strcmp("add-assumption", cmd)) {
    doAddAssumption(cmd, args);
  }
#ifdef ENABLE_CHECKPOINT
  else if (!strcmp("checkpoint", cmd) ||
      !strcmp("ckpt", cmd)) {
    create_checkpoint(args);
  } else if (!strcmp("restore", cmd)) {
    restore_checkpoint(args);
  } else if (!strcmp("list-checkpoints", cmd) ||
	     !strcmp("list-ckpt", cmd) ||
	     !strcmp("ckpts", cmd)) {
    list_checkpoints();
  } else if (!strcmp("delete-checkpoint", cmd) ||
      !strcmp("delete-ckpt", cmd)) {
    delete_checkpoint(args);
  }
#endif
  // Check to see if the lower level debugger can handle the command.
  else {
    return T_system_debug::parse_cmd(cmd, args);
  }
  return true;
}


// Print the help message for Tracker commands

void Tracker_debug::listener_usage() const {
  stream
    << "Tracker commands:\n"
    << "---------------------\n"
    << "  progress [cmd=value]        --> use min- or full- depending on style\n"
    << "  progress-style [min|full]   <-> no-arg, display; 1-arg, set progress style\n"
#ifndef DISABLE_FULL_PROGRESS
    << "  full-progress [cmd=value]   --> move time forward and assign command\n"
#endif
#ifndef DISABLE_MIN_PROGRESS
    << "  min-progress  [cmd=value]   --> move time forward with minimum representation\n"
#endif
    << _STD_ endl
    << "  find-candidates | fc        --> find candidates in some way\n"
    << "  fc-style <style>            <-> no-arg, display; 1-arg, set effect of fc\n"
    << "  find-fresh | ff             --> start search from scratch\n"
    << "  prune-search | ps           --> prune and search from there\n"
    << "  install i                   --> install i-th candidate\n"
    << "  candidates                  <-- list candidates\n"
    << "  classes                     <-- list Candidates after partitioning\n"
    << _STD_ endl
    << "  truncate <horizon>          --> truncate before horizon\n"
#ifdef ENABLE_CHECKPOINT
    << "  checkpoint | ckpt [name]    --> store a checkpoint, with optional name\n"
    << "  restore [i|name]            --> restore checkpoint i or `name'\n"
    << "  delete-checkpoint [i|name]  --> delete checkpoint i or `name'\n"
    << "  list-checkpoints            <-- list checkpoints in alpha order\n"
#endif
    << "  tracker-stats               <-- number of candidates\n"
    << "  add-assumption <var>=<value> --> add assumption in all candidates\n"
    << _STD_ endl;
  T_system_debug::listener_usage();
}


// Print the number of Candidates and Checkpoints

void Tracker_debug::print_stats() {
  stream << "Tracker has " << t_system.getCandidates().size()
	 << " candidates\n";
#ifdef ENABLE_CHECKPOINT
  stream << "Tracker debugger has " << nckpt << " checkpoints\n";
#endif
}


// Progress using the minimal-progress style

void Tracker_debug::minimal_progress(Assignment *pCommandAssignment) {
#ifdef DISABLE_MIN_PROGRESS
  _STD_ cerr << "warning: min-progress is disabled; issuing full progress\n";
  full_progress(pCommandAssignment);
#else
  // Unless this is done LTMS::get_clause_support() will abort.
  tracker->prune_and_search();
  begin_timing();
  t_system.minimal_progress(pCommandAssignment);
  end_timing();
#endif
}


// Progress using the full-progress style

void Tracker_debug::full_progress(Assignment *pCommandAssignment) {
#ifdef DISABLE_FULL_PROGRESS
  _STD_ cerr << "warning: full-progress is disabled; issuing min progress\n";
  minimal_progress(pCommandAssignment);
#else
  // Unless this is done LTMS::get_clause_support() will abort.
  tracker->prune_and_search();
  begin_timing();
  t_system.full_progress(pCommandAssignment);
  end_timing();
#endif
}


// Progress using the current progress style

void Tracker_debug::progress(Assignment *pCommandAssignment) {
  // Unless this is done LTMS::get_clause_support() will abort.
  tracker->prune_and_search();
  begin_timing();
  t_system.progress(pCommandAssignment);
  end_timing();
}


/***************************************************************************
  Printing stuff.
 ***************************************************************************/

// Print Candidates to the current output stream

void Tracker_debug::print_candidates() {
  print_candidates(stream);
}


// Print Candidates to the specified output stream

void Tracker_debug::print_candidates(_STD_ ostream& outstream) {
  unsigned int num = 0;
  for (Tracker::iterator it = t_system.getCandidates().begin();
       it != t_system.getCandidates().end(); ++it)  {
    Candidate *pCandidate = *it;
    outstream << "   Candidate " << num
#ifdef DEBUG_PRINT_CANDIDATE_WEIGHT
	      << " weight " << pCandidate->get_weight()
#endif
	      << ")\n";
    print_candidate(outstream, *pCandidate, 8);
    num++;
  }
  outstream << _STD_ endl;
}


/***************************************************************************
  Prologue and epilogue to a find-candidates call of some sort.
 ***************************************************************************/

// Begin timing and set the conflict count

void Tracker_debug::start_find_candidates() {
  begin_timing();
  nconflicts = t_system.num_conflicts();
}


// End timing and print out the find-candidates results

void Tracker_debug::end_find_candidates() {
  end_timing();
  stream
    << "Step "<< t_system.get_time_step() << "; "
    << t_system.size()                    << " variables" << "; "
    << nconflicts                         << " conflicts before" << "; "
    << t_system.num_conflicts()           << " after.\n"
    << search_termination()               << _STD_ endl
    << "The " << t_system.getCandidates().size() << " candidates are:"
    << _STD_ endl;
  print_candidates();
}


#ifdef ENABLE_CHECKPOINT

/***************************************************************************
  Checkpointing code.  First, the functions that correspond
  to the user commands.
 ***************************************************************************/


// Create a checkpoint and bind it to the name. If a Checkpoint is already
// bound to the name, replace it.

bool Tracker_debug::create_checkpoint(const MBA_string& name) {
  if (!t_system.get_progress_uses_full()) {
    // Can not create a checkpoint if progress style is "min-progress"
    _STD_ cerr << "Cannot create checkpoint with min-progress" << _STD_ endl;
    return false;
  } else if (t_system.get_max_history_length() == 0) {
    // Can not create checkpoint with unbounded history (can't saturate)
    _STD_ cerr << "Cannot create checkpoint with unbounded history"
	      << _STD_ endl;
    return false;
  } else if (t_system.get_horizon() == 0) {
    // Can not create checkpoint if history is not saturated
    _STD_ cerr << "Cannot create checkpoint until history is full" << _STD_ endl;
    return false;
  } else {
    L2_checkpoint *ckpt = new L2_checkpoint(tracker);
    ckpt_descriptor cd (ckpt, ++last_ckpt_id, name);
    _STD_ cout << "Created ";
    briefly_print_ckpt(cd);

    ckpt_descriptor cd_old = find_ckpt(name,false);
    if (cd_old.is_valid()) {
      _STD_ cout << "Replaces ";
      briefly_print_ckpt(cd_old);
      delete_ckpt(cd_old);
    }

    register_ckpt(cd);
    return true;
  }
}


// Look up the Checkpoint specified by the name string. If found, restore it.

bool Tracker_debug::restore_checkpoint(const MBA_string& name) {
  ckpt_descriptor cd = find_ckpt(name);
  if (cd.is_valid()) {
    _STD_ cout << "Restoring ";
    briefly_print_ckpt(cd);
    cd.ckpt->restore();
    return true;
  } else {
    return false;
  }
}


// Look up the Checkpoint specified by the name string. If found, delete it.

void Tracker_debug::delete_checkpoint(const MBA_string& name) {
  ckpt_descriptor cd = find_ckpt(name);
  if (cd.is_valid()) {
    _STD_ cout << "Removing ";
    briefly_print_ckpt(cd);
    delete_ckpt(cd);
  }
}


// sort by index

static bool ckpt_is_smaller(const Tracker_debug::ckpt_descriptor *a,
			    const Tracker_debug::ckpt_descriptor *b) {
  return a->index < b->index;
}


void Tracker_debug::list_checkpoints() {
    // sort them ; loop over by_id since by_name doesn't include unnamed ones.
    // The usector arg is true because the sort algo requires it
    Array< const ckpt_descriptor*, true > ckpts;
    Hash_table<unsigned,ckpt_descriptor>::iterator it = ckpt_by_id.begin();
    for( ; it != ckpt_by_id.end(); ++it)
        ckpts.push(&(*it));
    sort(ckpts, ckpt_is_smaller);

    // print them
    _STD_ cout << nckpt << " checkpoints:\n";
    for(unsigned i=0; i<ckpts.size(); ++i) {
        const ckpt_descriptor *ckpt = ckpts[i];
        _STD_ cout << "    "; briefly_print_ckpt(*ckpt);
    }
}


/***************************************************************************
  Checkpointing code.  The functions that are used by those above.
 ***************************************************************************/


// If the name is not empty, add to the hash table with name key
// Add to the hash table with ID key
// Increment the Checkpoint count

void Tracker_debug::register_ckpt(Tracker_debug::ckpt_descriptor cd) {
  if (cd.name != "") {
    ckpt_by_name.insert(cd.name, cd);
  }
  ckpt_by_id.insert(cd.index, cd);
  nckpt++;
}


// If the name is not empty, remove from the hash table with name key
// Remove from the hash table with ID key
// Delete the descriptor
// Decrement the Checkpoint count

void Tracker_debug::delete_ckpt(Tracker_debug::ckpt_descriptor cd) {
  if (cd.name != "") {
    ckpt_by_name.erase(cd.name);
  }
  ckpt_by_id.erase(cd.index);
  delete cd.ckpt;
  nckpt--;
}


// Parameter name is either an ID number or a name. Return the specified
// Checkpoint or, if it can't be found, and "invalid" Checkpoint

Tracker_debug::ckpt_descriptor
Tracker_debug::find_ckpt(const MBA_string& name, bool print_errors) {
  if (nckpt == 0) {
    if (print_errors) { _STD_ cerr << "No checkpoints are defined.\n"; }
    return ckpt_descriptor::invalid();
  } else {
    // figure out whether the name is an integer or a name
    bool by_id = true;
    for (unsigned i = 0; i < name.size(); ++i) {
      if (!isdigit(name[i])) {
	by_id = false;
	break;
      }
    }

    // get the checkpoint
    if (by_id) { // lookup by ID; some error-checking first
      int which;
      if (name.size() == 0) {
	which = last_ckpt_id;
      } else {
	which = atoi(name.c_str());
      }
      if (which < 0) {
	if (print_errors) { _STD_ cerr << "Checkpoint number must be positive.\n"; }
	return ckpt_descriptor::invalid();
      } else {
        Hash_table<unsigned, ckpt_descriptor>::iterator
            it = ckpt_by_id.find(which);
        if (it == ckpt_by_id.end()) {
	  if (print_errors) _STD_ cerr << "Checkpoint " << which << " not found.\n";
	  return ckpt_descriptor::invalid();
        } else {
	  return *it;
	}
      }
    } else { // lookup by name
      Hash_table<MBA_string,ckpt_descriptor>::iterator
	it = ckpt_by_name.find(name);
      if (it == ckpt_by_name.end()) {
	if (print_errors) {
	  _STD_ cerr << "Checkpoint `" << name << "' not found.\n";
	}
	return ckpt_descriptor::invalid();
      } else {
        return *it;
      }
    }
  }
}


// Print the index and, if not empty, the name

void Tracker_debug::briefly_print_ckpt(Tracker_debug::ckpt_descriptor cd) {
  _STD_ cout << "checkpoint " << cd.index;
  if (cd.name != "") {
    _STD_ cout << " `" << cd.name << "'";
  }
  _STD_ cout << _STD_ endl;
}


#endif

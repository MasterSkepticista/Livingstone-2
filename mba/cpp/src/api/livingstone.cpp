/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: livingstone.cpp,v 1.70 2004/08/11 20:49:20 lbrown Exp $ */

#include <ctype.h>              // for isspace()
#include <stdlib.h>             // for atoi()
#include <api/livingstone.h>
#include <readers/L2_file.h>
#include <readers/to_t_system.h>
#include <readers/variable.h>    /* For recovery */

#ifndef DISABLE_CBFS
#  include <opsat/cbfs_tracker.h>
#endif

#ifndef DISABLE_COVER
#  include <cover/cover_tracker.h>
#endif

// This is here because Tracker::~Tracker() requires tracker.h to be included
Livingstone::~Livingstone() {
  delete tracker_;
  delete pLivingstone_reader;
}


#ifndef DISABLE_MODEL_FILES
// Read parameters file before invoking Livingstone_reader::read_file()

bool Livingstone::read_file() {
  L2_TRY {
    // Do not append the extension; L2_parameters::read_from_file() does that.
    if (params_.read_from_file(getReader().getFilename())) {
      // To remain consistent with Livingstone::set_progress_style()
      // L2_parameters::read_from_file() can't do this because it doesn't
      // know about class Livingstone
      if (params_.get_progress_style() == T_system::min) {
	getReader().setUseOptimizer(true);
      }
      _STD_ cout << "Read parameters from "
		 << getReader().getFilename()
		 << ".params"
		 << _STD_ endl;
    }
  }
#ifdef ENABLE_EXCEPTIONS
  catch (const L2_parse_params_error& err) {
    _STD_ cerr << err.error_string
	      << _STD_ endl;
    return false;
  }
#endif

  // forward to data member
  return getReader().read_file();
}
#endif


// Create the subclass of the Tracker specified by get_use_cbfs_tracker()

void Livingstone::create_tracker() {
  L2_assert(tracker_ == NULL,
	    L2_fatal_error,
	    ("tracker already created"));

  if (get_use_cbfs_tracker()) {
#ifdef DISABLE_CBFS
    L2_throw( L2_disabled_error, ("CBFS is disabled"));
#else
    tracker_ = new CBFS_tracker(t_system,
				get_cbfs_maxcand(),
				get_cbfs_max_candidate_classes(),
				get_cbfs_search(),
				get_cbfs_max_cutoff_weight());
#endif
  } else {
#ifdef DISABLE_COVER
    L2_throw( L2_disabled_error, ("creating Cover tracker"));
#else
    L2_assert(tracker_==NULL,
	      L2_fatal_error, ("creating two trackers"));
    tracker_ =
      new Cover_tracker(t_system,
			static_cast<Assignment::weight_t>(get_cover_maxrank()),
			0);
#endif
  }
  t_system.setTracker(tracker_);
  // Set the Tracker's Find Candidates style from the L2_parameters value.
  {
    Tracker::FC_style fc_style = params_.get_find_candidates_style();
    if (fc_style == Tracker::invalid_fc) {
      L2_throw(L2_fatal_error, ("setting fc style to `invalid'"));
      tracker_->set_fc_style(Tracker::tracker_default_fc);
    } else {
      tracker_->set_fc_style(fc_style);	
    }
  }
  t_system.set_max_history_length(get_history());
  t_system.set_max_truncated_candidates(get_max_truncated_candidates());
  t_system.set_progress_uses_full(get_use_full_progress());
}


// Copy the contents of the L2_file into the tracker

bool Livingstone::initialize_tracker() {
  L2_TRY {
    // There must be a tracker
    L2_assert(tracker_,
	      L2_argument_error,
	      ("initialize_tracker called before create_*_tracker"));
    // There must be a model file
    L2_assert(getReader().get_file(),
	      L2_argument_error,
	      ("initialize_tracker has no file to use"));

    to_t_system writer(getReader().get_file(), &t_system);
    init_tracker_created_writer(&writer);
    if (!writer.write()) { // Writing failed
      return false;
    } else {
      writer.createInitialState();
      return true;
    }
  }
#ifdef ENABLE_EXCEPTIONS
  catch(L2_error err) {
    _STD_ cerr<< "l2test: Error in writing: "
	     << err.error_string
	     << _STD_ endl;
    return false;
  }
#endif
  // There is no longer any use for pLivingstone_reader; delete it
#ifndef ENABLE_RECOVERY
  delete pLivingstone_reader;
  pLivingstone_reader = NULL;
#endif
}


/***************************************************************************
        Event API.
 ***************************************************************************/

// Set the value of an Observable (Variable)

bool Livingstone::observe(unsigned varID, int value) {
  Variable *pVariable = t_system.get_present_variable(varID);
  if (pVariable != 0 && pVariable->is_observable()) {
    // Variable is Observable
    Observable *pObservable = static_cast<Observable *>(pVariable);
    if (value >= 0 && unsigned(value) >= pObservable->get_nvalues()) {
      // Value index out of range
      return false;
    } else {
      // Value index within range
      pObservable->observe(value);
      return true;
    }
  } else {
    // Variable is not Observable
    return false;
  }
}


// Set the value of a Command (Variable)

bool Livingstone::command(unsigned varID, unsigned value) {
  // Clear the candidate
  current_candidate_ = NULL;
  Variable *pVariable = t_system.get_present_variable(varID);
  if (pVariable != 0 && pVariable->is_command()) {
    // Unless this is done LTMS::get_clause_support() will abort
    t_system.getTracker()->prune_and_search();
    Command *pCommand = static_cast<Command *>(pVariable);
    if (value >= pCommand->get_nvalues()) {
      // Value index out of range
      return false;
    } else {
      // Value index within range
      t_system.progress(pCommand->get_assignment(value));
      return true;
    }
  } else {
    // Variable is not a Command
    return false;
  }
}


// This is here because Tracker::progress() requires tracker.h to be included
void Livingstone::idle() {
  // Unless this is done LTMS::get_clause_support() will abort
  t_system.getTracker()->prune_and_search();
  current_candidate_ = NULL;
  t_system.progress();
}


// Find Candidates according to the current method, and return their number

unsigned Livingstone::diagnose() {
  // Clear the favored candidate; this may be re-assigned
  current_candidate_ = NULL;
  switch(params_.get_find_candidates_style()) {
  case Tracker::tracker_default_fc:
    get_tracker()->find_candidates();
    break;

  case Tracker::extend_fc:
    get_tracker()->extend_candidates();
    break;

  case Tracker::find_fresh_fc:
    get_tracker()->find_fresh_candidates();
    break;

  case Tracker::prune_and_search_fc:
    get_tracker()->prune_and_search();
    break;

  case Tracker::invalid_fc:
    L2_throw(L2_fatal_error, ("diagnose using invalid fc style"));
    break;
  }

  return t_system.getCandidates().size();
}


/***************************************************************************
  Getting output from Livingstone.
 ***************************************************************************/


// This is here because Tracker::size() requires tracker.h to be included
unsigned Livingstone::ncandidates() const {
  return t_system.getCandidates().size();
}

// Install the indexed Candidate

bool Livingstone::install(unsigned cand_idx) {
  if (cand_idx >= ncandidates()) {
    return false;
  } else {
    current_candidate_ = t_system.getCandidate(cand_idx);
    t_system.install(*current_candidate_);
    return true;
  }
}


// This is here because T_system::NOW requires T_system.h to be included
int Livingstone::get_value(unsigned varID, bool& ok) const {
  return get_value(varID, T_system::NOW, ok);
}


// Retrieve the value of the indexed Variable at the specified time step

int Livingstone::get_value(unsigned varID, unsigned t, bool& ok) const {
  // error checking
  if (varID >= t_system.npresent_variables()) {
    ok = false;
    return NOT_FOUND;
  } else {

    // find the NOW variable
    Variable *v = t_system.get_present_variable(varID);

    // find at time t
    while(v && v->get_time_step() > t) v = v->get_previous();

    if (!v) {
      // reached earliest without finding ==> not found
      ok = false;
      return NOT_FOUND;
    } else {
      // else we've got the variable at the right time
      ok = true;
      return v->get_value_index();
    }
  }
}


/***************************************************************************
  Getting the trajectory as a list of failures.
 ***************************************************************************/

bool Livingstone::failure::is_valid() const {
  return xvalue != static_cast<unsigned>(INVALID_VALUE_INDEX); 
}


/* static */ 
Livingstone::failure Livingstone::failure::invalid() {
  return failure(static_cast<unsigned>(INVALID_VARIABLE_ID),
		 static_cast<unsigned>(INVALID_VALUE_INDEX),
		 static_cast<unsigned>(INVALID_TIME_STEP));
}


unsigned Livingstone::nfailures(const Candidate& candidate) const {
  unsigned result = 0;
  // count everything but the state=si assignment
  const Assignable* const pState_variable = t_system.get_state_variable();
  for (unsigned i = 0; i < candidate.size(); ++i) {
    if (candidate.get_assignment(i)->get_variable() != pState_variable) {
      result++;
    }
  }
  return result;
}


Livingstone::failure Livingstone::get_failure(const Candidate& candidate,
					      unsigned targetIndex) const {
  // No matter what the targetIndex, the empty Candidate has no elements
  if (candidate.size() == 1) {
    return failure::invalid();
  }
  // count everything but the state=si assignment
  const Assignable * const pState_variable = t_system.get_state_variable();
  const Assignment* pAssignment = 0;
  unsigned fail_idx = 0;
  for (unsigned i = 0; i < candidate.size(); i++) {
    pAssignment = candidate.get_assignment(i);
    if (pAssignment->get_variable() != pState_variable) {
      // this order means we correctly return fail_idx 0
      if (fail_idx == targetIndex) break;
      fail_idx++;
    }
  }
  if (fail_idx != targetIndex) {
    // we didn't find the failure; must be out of range
    return failure::invalid();
  } else {
    // in range; return it
    L2_assert(pAssignment, L2_fatal_error,
	      ("assignment is unset at " __FILE__));
    return failure(pAssignment->get_variable()->get_id(),
		   pAssignment->get_index(),
		   pAssignment->get_time_step());
  }
}


/***************************************************************************
  Print the parameters, including filename.
 ***************************************************************************/

// Dump the configuration parameters to the output stream

void Livingstone::print_search_parameters(_STD_ ostream& os) const {
#ifndef DISABLE_MODEL_FILES
  os << "Read `" << get_filename() << "'";
  if (getReader().getFiletype() != "") {
    os << " (" << getReader().getFiletype() << " format)";
  }
  os << _STD_ endl;
#endif
  params_.print(os);
}


// Get why the last search terminated

SearchTermination Livingstone::get_search_termination_reason() const {
  return tracker_->search_termination();
}

#ifdef ENABLE_RECOVERY


void Livingstone::reconfigureProgress(unsigned planSteps, T_system& t_system) {
  for (unsigned i = 0; i < planSteps; i++) {
    t_system.planner_progress();
  }
}


void Livingstone::reconfigureAssign(Array< MBApair<unsigned, unsigned>, true >&
				 goalObservations,
				 T_system& t_system) {
  for (Array< MBApair<unsigned, unsigned>, true >::iterator it =
	 goalObservations.begin(); it != goalObservations.end(); ++it) {
    MBApair<unsigned, unsigned> thePair = *it; // copy
    unsigned variableID = thePair.first;
    unsigned valueIndex = thePair.second;
    Observable* pObservable = t_system.get_observable(variableID);
    assert(pObservable != NULL);
    pObservable->observe(valueIndex);
  }
}


void Livingstone::reconfigureFindCandidates(Tracker* pTracker) {
  pTracker->find_candidates();
}


bool Livingstone::reconfigureResult(Array<Action*, false>& plan,
				    Tracker* pTracker) {
  bool result;
  plan.erase();
  if (t_system.getCandidates().size() == 0) {
    result = false;
  } else {
    // Get the first Candidate
    Candidate* pCandidate = *(t_system.getCandidates().begin());
    for (unsigned i = 0; i < pCandidate->size(); i++) {
      const Assignment* pAssignment = pCandidate->get_assignment(i);
      const Assignable* pAssignable = pAssignment->get_variable();
      unsigned variableID = pAssignable->get_id();
      Theory::time_step_t planStep = pAssignment->get_time_step();
      unsigned valueIndex = pAssignment->get_index();
      Action* pAction = new Action(variableID, valueIndex, planStep);
      plan.push_back(pAction); // client must delete
    }
    result = true;
  }
  return result;
}


void Livingstone::setupInitialModes(L2_file* l2_file,
				 Array< MBApair<unsigned, unsigned>, true >&
				 initialModes) {
  for (Array< MBApair<unsigned, unsigned >, true >::iterator it =
	 initialModes.begin(); it != initialModes.end(); ++it) {
    MBApair<unsigned, unsigned> thePair = *it; // copy
    unsigned variableID = thePair.first;
    unsigned valueIndex = thePair.second;
    L2rVariable* pL2rVariable = l2_file->getVar_friend(variableID);
    assert(pL2rVariable != NULL);
    pL2rVariable->shadowInitial(valueIndex);
  }
}


void Livingstone::cleanupInitialModes(L2_file* l2_file,
				   Array< MBApair<unsigned, unsigned>, true >&
				   initialModes) {
  for (Array< MBApair<unsigned, unsigned >, true >::iterator it =
	 initialModes.begin(); it != initialModes.end(); ++it) {
    MBApair<unsigned, unsigned> thePair = *it; // copy
    unsigned variableID = thePair.first;
    L2rVariable* pL2rVariable = l2_file->getVar_friend(variableID);
    assert(pL2rVariable != NULL);
    pL2rVariable->restoreInitial();
  }
}

bool Livingstone::reconfigure(L2_file* l2_file,
			   unsigned planSteps,
			   Array< MBApair<unsigned, unsigned>, true >&
			      initialModes,
			   Array< MBApair<unsigned, unsigned>, true >&
			      goalObservations,
			   Array<Action*, false>& plan) {
  assert(l2_file != NULL);
  assert(planSteps > 0);

  bool result;
  // Make a Livingstone instance
  Livingstone livingstone;
  setupInitialModes(l2_file, initialModes);
  getReader().set_file(l2_file); // this had to be added to L2
  livingstone.create_tracker();
  // Declare this to be for recovery
  livingstone.get_t_system().setPlanning();
  if (!livingstone.initialize_tracker()) {
    _STD_ cerr << "Failed to initialize tracker" << _STD_ endl;
    result = false;
  } else {
    T_system& t_system = livingstone.get_t_system();
    reconfigureProgress(planSteps, t_system);
    reconfigureAssign(goalObservations, t_system);
    reconfigureFindCandidates(t_system.getTracker());
    result = reconfigureResult(plan, t_system.getTracker());
  }
  cleanupInitialModes(l2_file, initialModes);
  // We must not let ~Livingstone() delete the L2_file!
  getReader().pop_file(); // this had to be added to L2
  return result;
}
#endif

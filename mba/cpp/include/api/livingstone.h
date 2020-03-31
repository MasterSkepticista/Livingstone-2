/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: livingstone.h,v 1.52 2004/02/10 22:26:34 lbrown Exp $ */

#ifndef livingstone_HEADER
#define livingstone_HEADER

#include <readers/livingstone_reader.h>
#include <transition/termination.h> // for SearchTermination
#include <api/parameters.h>

class to_t_system;
class Candidate;
class Tracker;
#ifdef ENABLE_RECOVERY
class L2_file;
class T_system;
#endif

/**
 * This handy package is the Livingstone API.
 * It joins together the capability to:
 *  - read a model;
 *  - possibly optimize it;
 *  - create a Tracker;
 *  - provide a clean API to the Tracker
 * This API is considered stable (nothing will be removed or renamed). Call
 * methods on Livingstone instead of calling methods on Tracker.
 */

class Livingstone {
 public:

  enum public_constants {
    NOT_FOUND = -1,
    INVALID_VARIABLE_ID = -1,
    INVALID_VALUE_INDEX = -1,
    INVALID_TIME_STEP   = -1
  };

  /// Nothing special to do
  Livingstone() :
    tracker_(NULL), verbose_writer_(false), current_candidate_(NULL),
    t_system(T_system::DEFAULT_HISTORY_LENGTH,
	     T_system::DEFAULT_NUM_HISTORIES,
	     T_system::DEFAULT_PROGRESS_USES_FULL) {
    pLivingstone_reader = new Livingstone_reader();
  }

  /// Delete the Tracker and the Livingstone_reader
  virtual ~Livingstone();

  /************************************************************
   *Reading in a model and putting it into the tracker.
   ************************************************************/

#ifndef DISABLE_MODEL_FILES
  /**
   * Read the *.params file and then the model file from the file system.
   * The file name and type are specified by set_filename() and set_filetype().
   * \return whether a model was read
   */

  bool read_file();
#endif


#ifndef DISABLE_OPTIMIZER
  /**
   * Optimize the file, if needed or requested.
   * Overridden by class Livingstone_debug.
   * \return whether the model was optimized
   */
  virtual bool optimize() { return getReader().optimize(); }
#endif

  /**
   * Create a Tracker.
   * If the configuration parameters in the *.params file are to be used,
   * this must be called after reading in the model file. It must be followed
   * by a call to initialize_tracker() before issuing any commands.
   * If search style is L2_parameters::cover, the Tracker will be a
   * Cover_tracker; if search style is L2_parameters::cbfs, the Tracker will
   * be a CBFS_tracker.
   */
  void create_tracker();

  /**
   * Copy the model into the Tracker
   * Both create_tracker() and read_file() must already have been called.
   * \return whether the tracker was initialized
   */
  bool initialize_tracker();

  /************************************************************
   *          Run-time input into Livingstone.
   ************************************************************/

  /**
   * Observe the current value of the Observable Variable with the given ID.
   * If value is negative, the variable value is set to be unknown.
   * \param varID the ID of the Observable Variable
   * \param valueIndex the index of the value
   * \return whether (1) varID is the ID of an Observable Variable and
   * (2) valueIndex is the index of a value in its domain
   */
  bool observe(unsigned varID, int valueIndex);

  /**
   * Tell Livingstone about a command that has just been issued and progress
   * the time step by one.
   * \param varID the ID of the Command Variable
   * \param valueIndex the index of the value, which must be nonnegative
   * \return whether (1) varID is the ID of a Command Variable and
   * (2) valueIndex is the index of a value in its domain
   */
  bool command(unsigned varID, unsigned value);

  /**
   * Idle the system for one time step.
   * Time is progressed forward by one after this.
   */
  void idle();

  /**
   * Ask Livingstone for a diagnosis that explains the current observations.
   * Also called "find candidates."
   * The kind of search is determined by the current 'find_candidates_style'.
   * \return the number of Candidate diagnoses.
   */
  unsigned diagnose();

  /************************************************************
   *          Getting output from Livingstone.
   ************************************************************/

  /**
   * Returns the number of current Candidate diagnoses.
   * \return the number of current Candidate diagnoses
   */
  unsigned ncandidates() const;

  /**
   * Install the specified Candidate and propagate the system.
   * The assignments of values to Assumption Variables is not persistent, and
   * will be undone whenever Tracker::install(const Candidate&, bool) is
   * called, which happens during diagnosis.
   * Note that propagation affects the state of the LTMS, thereby affecting
   * the order in which Conflicts are created, ultimately affecting the order
   * in which Candidates are generated.
   * The functions following this one assume a Candidate has been installed,
   * and otherwise give undefined answers (possibly with no error), unless
   * they take a Candidate or its index as an argument.
   * \param candidateIndex the index of the Candidate
   * \return whether candidateIndex is within the range [0..ncandidates()-1]
   */
  bool install(unsigned candidateIndex);

  /**
   * Is a Candidate is currently installed?
   * \return whether a Candidate is currently installed
   */
  bool have_installed() const { return current_candidate_ != NULL; }

  /**
   * Get the current value of a Variable.
   * \param varID the ID of the Variable
   * \param validID whether varID is the ID of a Variable
   * \return the index of the value; NOT_FOUND if unknown or there's an error.
   */
  int get_value(unsigned varID, bool& validID) const;

  /**
   * Get the value of a Variable at the given time step.
   * \param varID the ID of the Variable
   * \param timeStep the time step
   * \param validID whether (1) varID is the ID of a Variable and
   * (2) the Variable exists at timeStep
   * \return the index of the value; negative if unknown or there's an error.
   */
  int get_value(unsigned varID, unsigned timeStep, bool& validID) const;

  /**
   * A Candidate can be viewed as two things:
   *     1) a list of mode assignments
   *     2) a list of failures from initial modes
   * This class is a representation of the latter. Note that 'initial' means
   * at the truncation horizon. Anything prior will have been lost. There is
   * currently no public API to retain that knowledge, but see class
   * Trajectory.
   */
  
  class failure {
  private:
    /// The index of the Transition Variable
    unsigned xvarID;
    /// The value of the Transition Variable
    unsigned xvalue;
    /// The time step at which the failure Transition occurred.
    int timestep; // signed so that BEFORE_HORIZON makes sense

  public:
    /**
     * Specify the Transition Variable, its value and the time step.
     * \param varID the ID of the Transition Variable
     * \param valueIndex the index of the value
     * \param timeStep the time step
     */
    failure(unsigned varID, unsigned valueIndex, unsigned timeStep) :
      xvarID(varID), xvalue(valueIndex), timestep(timeStep) { }
    
    /**
     * get_failure() returns an invalid failure if the index is invalid. This
     * function allows checking that.
     * \return whether the failure object is valid
     */
    bool is_valid() const;
    
    /**
     * Return an invalid failure.
     * \return a failure object such that failure::is_valid() returns false
     */
    static failure invalid();
    
    /**
     * Return the ID of the Transitioned (mode) and Transition Variables.
     * \return the ID of the Transitioned and Transition Variables
     */
    unsigned get_id() const { return xvarID; }

    /**
     * Return the Transition the mode took.
     * This is NOT the new value of the mode.
     * \return the Transition Variable's value index
     */
    unsigned get_value() const { return xvalue; }

    /**
     * Return the time at which the failure happened.
     * The mode at the time step itself is nominal; the mode at the following
     * time step is failed.
     * A time of T_system::BEFORE_HORIZON means that the failure happened at
     * the "initial" time step (before the horizon). No other time will be
     * negative.
     * \return the time step at which the failure happened;
     * T_system::BEFORE_HORIZON if before the horizon
     */
    int get_time_step() const { return timestep; }
  }; // end class failure


  /**
   * Get the number of failures associated with the currently-installed
   * Candidate diagnosis.
   * \return the failure count, or 0 if no Candidate is installed
   */
  unsigned nfailures() const {
    if (current_candidate_ == NULL) { return 0; }
    else { return nfailures(*current_candidate_); }
  }

  /**
   * Get the number of failures associated with the Candidate diagnosis
   * specified by its index.
   * \param i the index of the Candidate
   * \return the failure count or 0 if the index is out of range
   */
  unsigned nfailures(unsigned i) const {
    return (i < ncandidates()) ? nfailures(*t_system.getCandidate(i)) : 0;
  }

  /**
   * Get the number of failures associated with the specified Candidate
   * diagnosis.
   * \param candidate the Candidate
   * \return the failure count
   */

  unsigned nfailures(const Candidate& candidate) const;

  /**
   * Get the jth failure associated with the currently-installed Candidate
   * diagnosis. Returns an invalid failure if the index is out of bounds
   * (greater than nfailures()).
   * \param j index of the failure to return
   * \return the jth failure if it exists; otherwise an invalid failure object
   */
  failure get_failure(unsigned j) const {
    if (current_candidate_ == NULL) { return failure::invalid(); }
    else { return get_failure(*current_candidate_, j); }
  }

  /**
   * Get the jth failure associated with the ith Candidate diagnosis.
   * Returns an invalid failure if either index is out of bounds (i greater
   * than ncandidates() or j greater than nfailures)
   * \param i the index of the Candidate
   * \return the jth failure if it exists; otherwise an invalid failure object
   */
  failure get_failure(unsigned i, unsigned j) const {
    if (i < ncandidates()) {
      return get_failure(*(t_system.getCandidate(i)), j);
    } else { return failure::invalid(); }
  }

  /**
   * Get the jth failure associated with the specified Candidate diagnosis.
   * Returns an invalid failure if the index is out of bounds (greater than
   * nfailures()
   * \param candidate the Candidate
   * \param j index of the failure to return
   * \return the jth failure if it exists; otherwise an invalid failure object
   */
  failure get_failure(const Candidate& candidate, unsigned j) const;


  /************************************************************
   *          Managing the various parameters to the search.
   *        NOTE these functions are subject to change after 2.3.
   ************************************************************/

  /**
   * Return the object encapsulating the configuration parameters.
   * \return the L2_parameters object specifying configuration settings
   */
  const L2_parameters *get_parameters() const { return &params_; }

  /**
   * Set the object encapsulating the configuration parameters.
   * \param p an L2_parameters objects specifying configuration settings
   */
  void set_parameters(const L2_parameters& p) { params_ = p; }

  /// Pretty-print the configuration parameters to _STD_ cout.
  void print_search_parameters() const { print_search_parameters(_STD_ cout); }

  /**
   * Pretty-print the configuration parameters to the supplied ostream.
   * \param outstream where to send output
   */
  void print_search_parameters(_STD_ ostream& outstream) const ;

  /**
   * Is the writer verbose?
   * \return whether the writer is in verbose mode
   */
  bool verbose_writer() const { return verbose_writer_; }

  /**
   * Set the verbosity of the reader, optimizer and writer.
   * \param reader whether the reader is to be in verbose mode
   * \param optimizer whether the optimizer is to be in verbose mode
   * \param writer whether the writer is to be in verbose mode
   */
  void set_verbosity(bool reader, bool optimizer, bool writer) {
    getReader().set_verbosity(reader, optimizer);
    verbose_writer_  = writer;
  }

  /**
   * Set the verbosity of the writer.
   * \param b whether the writer is in verbose mode
   */
  void set_verbose_writer(bool b) { verbose_writer_ = b; }

#ifndef DISABLE_MODEL_FILES
  /**
   * Set the pathname of the model file.
   * \param modelFileName the pathname of the model file (without extension)
   */
  void set_filename(const MBA_string& modelFileName) {
    getReader().setFilename(modelFileName);
  }

  /**
   * Return the pathname of the model file.
   * \return the pathname of the model file (without extension)
   */
  const MBA_string& get_filename() const { return getReader().getFilename(); }

  /**
   * Set the type (format) of the model file.
   * If empty (the default), it will be determined from the file name.
   * \param modelFileFormat the name of the format, "bin", "l2", "l2bin",
   * "l2s", "xmpl" or ""
   */
  void set_filetype(const MBA_string& modelFileFormat) {
    getReader().setFiletype(modelFileFormat);
  }

  /**
   * Return the type (format) of the model file.
   * \return  the name of the format, "bin", "l2", "l2bin", "l2s", "xmpl" or ""
   */
  const MBA_string& get_filetype() const { return getReader().getFiletype(); }
#endif

  /**
   * Return the search style.
   * \return "cbfs" or "cover"
   */
  MBA_string get_search_style_str () const {
    return params_.get_search_style_str();
  }

  /**
   * Set the search style.
   * Meaningful only before calling Livingstone::create_tracker().
   * \param searchStyle, L2_parameters::cbfs or L2_parameters::cover
   */
  void set_search_style(Tracker::Search_style searchStyle) {
    params_.set_search_style(searchStyle);
  }

  /**
   * Is the search style CBFS?
   * \return whether the search style is Tracker::cbfs
   */
  bool get_use_cbfs_tracker () const {
    return params_.get_search_style() == Tracker::cbfs;
  }

  /**
   * Return the maximum number of Candidates returned by CBFS_tracker.
   * \return the maximum number of Candidates returned by CBFS_tracker
   */
  unsigned get_cbfs_maxcand () const {
    return params_.get_cbfs_max_candidates();
  }

  /**
   * Set the maximum number of Candidates returned by CBFS_tracker.
   * \param max_value the maximum number of Candidates returned by
   * CBFS_tracker; a value of 0 means unbounded
   */
  void set_cbfs_maxcand (unsigned max_value) {
    params_.set_cbfs_max_candidates(max_value);
  }

  /**
   * Return the maximum number of Candidate equivalence classes returned by the
   * CBFS_tracker.
   */
  unsigned get_cbfs_max_candidate_classes() const {
    return params_.get_cbfs_max_candidate_classes();
  }

  /**
   * Set the maximum number of Candidate equivalence classes returned by the
   * CBFS_tracker.
   * \param maxClassCount the maximum number of equivalence classes; 0 means
   * unlimited.
   */
  void set_cbfs_max_candidate_classes(unsigned maxClassCount) {
    params_.set_cbfs_max_candidate_classes(maxClassCount);
  }

  /**
   * Return the maximum number of Candidates searched by CBFS_tracker.
   * \return the maximum number of Candidates searched by CBFS_tracker
   */
  unsigned get_cbfs_search () const {
    return params_.get_cbfs_search_space();
  }

  /**
   * Set the maximum number of Candidates searched by CBFS_tracker.
   * \param max_value the maximum number of Candidates searched by CBFS_tracker
   */
  void set_cbfs_search (unsigned max_value) {
    params_.set_cbfs_search_space(max_value);
  }

  /**
   * Return the maximum rank of a Candidate searched by CBFS_tracker.
   * \return the maximum rank of a Candidate searched by CBFS_tracker
   */
  unsigned get_cbfs_max_cutoff_weight () const {
    return params_.get_cbfs_max_rank();
  }

  /**
   * Set the maximum rank of a Candidate searched by CBFS_tracker.
   * \param max_value the maximum rank of a Candidate searched by CBFS_tracker;
   * a value of 0 means unbounded
   */
  void set_cbfs_max_cutoff_weight (unsigned max_value) {
    params_.set_cbfs_max_rank(max_value);
  }

  /**
   * Return the maximum rank of a Candidate searched by Cover_tracker.
   * \return  the maximum rank of a Candidate searched by Cover_tracker
   */
  unsigned get_cover_maxrank () const { return params_.get_cover_max_rank(); }

  /**
   * Set the maximum rank of a Candidate searched by Cover_tracker.
   * \param max_value the maximum rank of a Candidate searched by Cover_tracker
   */
  void set_cover_maxrank (unsigned max_value) {
    params_.set_cover_max_rank(max_value);
  }

  /**
   * Return the length of history kept
   * \return the number of time steps of history kept; 0 is unbounded
   */
  unsigned get_history () const { return params_.get_history(); }

  /**
   * Set the length of history kept.
   * \param historyLength the number of time steps of history kept; 0 is
   * unbounded
   */
  void set_history (unsigned historyLength) {
    params_.set_history(historyLength);
  }

  /**
   * Return the progress style
   * \return the progress style, L2_parameters::full or L2_parameters::min
   */
  T_system::Progress_style get_progress_style() const {
    return params_.get_progress_style();
  }

  /**
   * Return the progress style as "full" or "min".
   * \return the progress style string, "full" or "min"
   */
  MBA_string get_progress_style_str () const {
    return params_.get_progress_style_str();
  }

  /**
   * Set the progress style.
   * If L2_parameters::min, set a flag to optimize during reading.
   * \param progress_style, L2_parameters::full or L2_parameters::min
   */
  void set_progress_style (T_system::Progress_style progress_style) {
    // set in the params list
    params_.set_progress_style(progress_style);
    // and, if min-progress, know to optimize
    if (progress_style == T_system::min) { getReader().setUseOptimizer(true); }
  }

  /**
   * Set the progress style.
   * Ignored if the value is not "full" or "min".
   * If "min", set a flag to optimize during reading.
   * \param progressStyleString the progress style, one of "full" or "min"
   */
  void set_progress_style_str (const MBA_string& progressStyleString) {
    set_progress_style(params_.get_progress_style(progressStyleString));
  }

  /**
   * Is the progress style L2_parameters::full?
   * \return whether progress style is L2_parameters::full
   */
  bool get_use_full_progress () const {
    return params_.get_progress_style() == T_system::full;
  }

  /**
   * Return how many Candidates to keep through a truncate.
   * \return the number of Candidates to keep through a truncate
   */
  unsigned get_max_truncated_candidates () const {
    return params_.get_max_truncated_candidates();
  }

  /**
   * Set how many Candidates to keep through a truncate.
   * \return the number of Candidates to keep through a truncate
   */
  void set_max_truncated_candidates (unsigned max_value) {
    params_.set_max_truncated_candidates(max_value);
  }

  /**
   * Return the current find Candidates style
   * \return the current find-candidates style, one of
   * L2_parameters::tracker_default, L2_parameters::extend,
   * L2_parameters::prune_and_search or L2_parameters::find_fresh
   */
  Tracker::FC_style get_fc_style () const {
    if (tracker_ != 0) {
      // There is a Tracker; get the FC_style from it
      return tracker_->get_fc_style();
    } else {
      // Use the value in the L2_parameters object
      return params_.get_find_candidates_style();
    }
  }

  /**
   * Set the find-candidates style
   * \param fc_style one of L2_parameters::tracker_default,
   * L2_parameters::extend, L2_parameters::prune_and_search or
   * L2_parameters::find_fresh
   */
  void set_fc_style (Tracker::FC_style fc_style) {
    if (tracker_ != 0) {
      // There is a Tracker; set the FC_style for it
      tracker_->set_fc_style(fc_style);
    }
    // Unconditionally set the FC_style for the L2_parameters object
    params_.set_find_candidates_style(fc_style);
  }

  /**
   * Return the find Candidates style, one of "the tracker default fc",
   * "extend", "prune-and-search" or "find-fresh".
   * \return the current find-candidates style, one of
   * "the tracker default fc", "extend", "prune-and-search" or "find-fresh".
   */
  MBA_string get_fc_style_str () const {
    if (tracker_ != 0) {
      // There is a Tracker; get the FC style from it
      Tracker::FC_style fc_style = tracker_->get_fc_style();
      return params_.get_find_candidates_style(fc_style);
    } else {
      // There is no Tracker; get the value from the L2_parameters object
      return params_.get_find_candidates_style_str();
    }
  }

  /**
   * Set the find Candidates style, one of "the tracker default fc",
   * "extend", "prune-and-search" or "find-fresh". Ignored if the value is not
   * one of the above.
   * \param fc_style the find-candidates style, one of
   * "the tracker default fc", "extend", "prune-and-search" or "find-fresh".
   */
  void set_fc_style_str (const MBA_string& fc_style_s) {
    // Map the name string to an enum
    Tracker::FC_style fc_style = params_.get_find_candidates_style(fc_style_s);
    if (tracker_ != 0) {
      // There is a Tracker; set the FC style for it
      tracker_->set_fc_style(fc_style);
    }
    // Unconditionally set the FC_style for the L2_parmeters object
    set_fc_style(fc_style);
  }

  /**
   * Return whether the reader is flagged to be optimized upon reading.
   * This flag is informational only. It is the client's responsibility to call
   * Livingstone::optimize().
   * \return whether the model is flagged to be optimized upon reading
   */
  bool get_use_optimizer() const { return getReader().getUseOptimizer(); }

  /**
   * Set whether the reader is flagged to be optimized upon reading.
   * If progress style is L2_parameters::min, leave this flag set to true or
   * results will be incorrect. This flag is informational only. It is the
   * client's responsibility to call Livingstone::optimize().
   * \param b whether the model should be optimized upon reading
   */

  void set_use_optimizer(bool b) { getReader().setUseOptimizer(b); }

  /**
   * Return a read-only pointer to the Tracker (the Livingstone engine).
   * This method is deprecated; these interfaces are unsupported and may change
   * without warning from version to version.
   * \return a const pointer to the Tracker object
   */
  const Tracker *get_tracker() const { return tracker_; }

  /**
   * Return a pointer to the Tracker (the Livingstone engine).
   * This method is deprecated; these interfaces are unsupported and may change
   * without warning from version to version.
   * \return a pointer to the Tracker object
   */
  Tracker *get_tracker() { return tracker_; }

  /**
   * Return a reference to the Livingstone_reader data member.
   */
  Livingstone_reader& getReader() const {
    L2_assert(pLivingstone_reader != NULL,
	      L2_fatal_error,
	      ("Livingstone_reader deallocated prematurely"));
    return *pLivingstone_reader;
  }

  /**
   * Return the reason the last search terminated (if it did terminate).
   * \return the reason the last search terminated (if it did terminate)
   */
  SearchTermination get_search_termination_reason() const;

#ifdef ENABLE_RECOVERY
  /**
   * Neutral representation of a plan action. Essentially a Command assignment.
   * It is a bundle of three "properties": planStep, variableID and valueIndex.
   */

  class Action {
  public:
    /**
     * Save the arguments.
     * \param variableID the unique key for the Variable
     * \param valueIndex the index of a value in the Variable's domain
     * \param planStep the time step in the partially-ordered plan
     */
    Action(unsigned variableID, unsigned valueIndex, int planStep) :
      variableID(variableID), valueIndex(valueIndex), planStep(planStep) { }
    /// Return the Variable's ID
    unsigned getVariableID() { return variableID; }
    /// Return the Variable's value
    unsigned getValueIndex() { return valueIndex; }
    /// Return the time step in the partially-ordered plan
    int getPlanStep() { return planStep; }
  private:
    unsigned variableID;
    unsigned valueIndex;
    int planStep;
  };

  /**
   * Take a description of a planning problem, get the Tracker to solve it,
   * and return the result.
   * \param l2_file in-memory representation of the model file from some
   * Livingstone instance
   * \param planSteps the maximum number of plan steps allotted
   * \param initialModes the initial state expressed as mode assignments
   * \param goalObservations the goal expressed as a set of Observations for
   * the NOW time step
   * \param plan the plan is returned in this out parameter; the client must
   * free the elements
   * \return whether a plan was found
   * \internal This is here because lower levels should not know about L2_file
   * or Livingstone
   */
  bool reconfigure(L2_file* l2_file,
		   unsigned planSteps,
		   Array< MBApair<unsigned, unsigned>, true >& initialModes,
		   Array< MBApair<unsigned, unsigned>, true >& goalObservations,
		   Array<Action*, false>& plan);
#endif

  /** Return a reference to the T_system */
  T_system& get_t_system() { return t_system; }

 protected:

  /**
   * Template method pattern: initialize_tracker allows redefining this
   * function in order to link in a listener (for debuggers).
   * \param to_t_system the writer
   */
  virtual void init_tracker_created_writer(to_t_system* /* writer */) { }

 private:

  /// The Livingstone engine. Tracker is an abstract class.
  Tracker *tracker_;

  /// Is the writer verbose?
  bool verbose_writer_;

  /// Configuration parameters encapsulated in an object.
  L2_parameters params_;

  /// The Candidate the user installed.
  // Developer's note: must be cleared by anything that affects the list of
  // Candidates (progress, fc, ...)
  const Candidate* current_candidate_;

  /// Responsible for manipulating model files and L2_file instances
  Livingstone_reader* pLivingstone_reader;

  /// The Transition system
  T_system t_system;

#ifdef ENABLE_RECOVERY
  /*
   * Establish the time steps for the plan.
   * \param planSteps the maximum number of time steps the plan may take
   * \param t_system the Livingstone transition system
   */
  void reconfigureProgress(unsigned planSteps, T_system& t_system);

  /*
   * Assign the future observations.
   * \param goalObservations the Observations that are to be ensured by the
   * plan
   * \param t_system the Livingstone transition system
   */
  void reconfigureAssign(Array< MBApair<unsigned, unsigned>, true >&
			 goalObservations,
			 T_system& t_system);

  /*
   * Get Livingstone to create the plan.
   * \param pTracker a pointer to the Livingstone Tracker
   */
  void reconfigureFindCandidates(Tracker* pTracker);

  /*
   * Package the result for return.
   * \param plan the plan data structure
   * \param pTracker a pointer to the Livingstone Tracker
   */
  bool reconfigureResult(Array<Action*, false>& plan, Tracker* pTracker);

  /**
   * Set the initial modes in the L2_file, saving for later restoration.
   * \param l2_file the model file as read into memory
   * \param initialModes the starting state for planning
   */
  void setupInitialModes(L2_file* l2_file,
			 Array< MBApair<unsigned, unsigned>, true >&
			 initialModes);

  /**
   * Restore the L2_file object to its original state
   * \param l2_file the model file as read into memory
   * \param initialModes the starting state for planning
   */
  void cleanupInitialModes(L2_file* l2_file,
			   Array< MBApair<unsigned, unsigned>, true >&
			   initialModes);
#endif

};

/**
 * \mainpage Livingstone Reference Manual
 *
 * <!-- Note: The HTML links assume that the HTML page generated from this
 *      source file is in a subdirectory of "mba/cpp/doc/L2_refman/"
 * -->
 *
 * <p>Livingstone is a model-based reasoning system with discrete-valued
 * variables and a discrete representation of time. It provides discrete state
 * estimation (diagnosis) and is planned to provide reconfiguration
 * (recovery).</p>
 *
 * <p>The following brief manuals are the primary external documentation.
 * <ul>
 *   <li><a href="../../developer_man/l2dev.html>Developer Manual</a></li>
 *   <li><a href="../../user_man/l2.html>User Manual</a></li>
 * </ul></p>
 *
 * <p>The following documents describe the Livingstone design in more detail
 * but still in language-independent terms.
 * <ul>
 *   <li><a href="../design.html">Basic Functionality of L2</a></li>
 *   <li><a href="../progress.html">The Progress Algorithm</a></li>
 *   <li><a href="../truncate.html">The Truncation Algorithm</a></li>
 *   <li><a href="../recovery.html">Recovery (Reconfiguration)</a></li>
 *   <li><a href="../truncated-failures.html">Inclusion of Truncated
 *       Failures</a></li>
 *   <li><a href="../L2UnknownMode.html">Component "Unknown" Modes</a></li>
 * </ul></p>
 *
 * <p>The <a href="http://ic-www.arc.nasa.gov/ic/projects/mba">web site</a>
 * for the Model-Based Autonomy (MBA) research area has links to additional
 * resources.</p>
 *
 * <p>People with access to the Ames "PostDoc" pages will find a large amount
 * of information at the
 * <a href="http://ace.arc.nasa.gov/postdoc/livingstone">Livingstone PostDoc
 * page</a>.
 *
 * \section architecture Architecture
 *
 * <p>Livingstone's architecture is layered. Each layer is dependent upon the
 * layer below, but need not be used with the layers above.</p>
 *
 * \subsection executable Executable
 *
 * <p>The top layer is an executable that accepts a scenario (of observations,
 * commands, and requests for diagnoses) and passes these on to the reasoning
 * engine. There are two such executables:
 * <dl>
 *   <dt>l2test  </dt><dd>With an extensive command-line interface using
 *                        Variable and value names for development and
 *                        interactive use.</dd>
 *   <dt>l2flight</dt><dd>With a minimal command-line interface using Variable
 *                        and value indices for embedded use.</dd>
 * </dl></p>
 *
 * \subsection api The Livingstone API
 *
 * <p>The next layer is the application programmer's interface, responsible for
 * reading a model into the reasoning engine and performing the diagnosis. The
 * API is supplied as class Livingstone, for flight code, and class
 * Livingstone_debug, which provides a command-line interpreter. The API
 * contains a pointer to the Tracker, which is created at run time.</p>
 *
 * <p>The API enables the user to read in a model of a system to be tracked,
 * to set configuration <a href="../L2Parameters.html">parameters</a>,
 * allows the user to present commands and observations to Livingstone and
 * receive state estimates in response. Livingstone uses the commands issued
 * to the system and the observations that were generated as a result in order
 * to determine and track the likely trajectories of the system.</p>
 *
 * <p>For more information, there is a manually-created
 * <a href="../L2API.html">discussion of the Livingstone API</a>.
 *
 * \subsection tracker The Tracker
 *
 * <p>The next layer is the Tracker, responsible for managing Candidate
 * diagnoses. There are two types of Tracker, CBFS and cover. The API has
 * methods for selecting the type of Tracker and then creating a Tracker of
 * the selected type. The Tracker contains an instance of the T_system.</p>
 *
 * <p>The CBFS_tracker class implemente a conflict-directed, best-first search
 * that Livingstone can employ in order to choose the likely trajectories
 * to track. It performs a search over the Transition System
 * representation.</p>
 *
 * <p>The Cover_tracker class implements a conflict-coverage search that
 * Livingstone can employ in order to choose the likely trajectories to
 * track. It performs a search over the Transition System representation.</p>
 * 
 * \subsection t_system The Transition System
 *
 * <p>The next layer is the T_system (transition system), responsible for
 * managingthe transition from a time step to its successor time step. It is
 * the lowest level that reasons about time; although lower layers store the
 * time step, they don't reason about time. The T_system is a subclass of
 * Conflict_db.</p>
 *
 * <p>The Transition System represents the discrete state Variables of the
 * system being tracked and their evolution over time. The Transition System
 * represents recent history exactly and past history approximately. It is a
 * specialization of the Conflict Data Base, which is not temporal.</p>
 *
 * \subsection conflict_db The Conflict Data Base
 *
 * <p>The next layer is the Conflict_db (conflict data base), which manages the
 * values of Variables and mutually-inconsistent value assignments to those
 * Variables (called Conflicts). The Conflict_db contains an instance of the
 * LTMS, which represents assertions about Variable values as Proposition
 * objects and constraints on Variable values as Clause objects.</p>
 *
 * <p>The Conflict_db class provides a database of discrete-valued Variables
 * and constraints between Variable Assignments. It provides the notion of a
 * Conflict, or a set of Assignments that cannot all be made at once, as they
 * violate the Constraints.</p>
 *
 * \subsection ltms The Truth-Maintenance System
 *
 * <p>The lowest layer is the LTMS (logic-based truth-maintenance system),
 * which represents a logical theory in terms of Clause and Proposition
 * objects. It propagates changes to Propositions and determines whether the
 * theory is consistent.</p>
 *
 * <p>The TMS accepts a theory consisting of set of Propositions and
 * constraints between them. Given a truth assignment to a subset of the
 * Propositions, it determines if the theory is currently consistent. If not,
 * it returns a Conflict between Assignments that is causing an inconsistency.
 * This is a standard LTMS with some extensions to allow it to support the
 * creation and elimination of Propositions and constraints over time.</p>
 *
 * \subsection mba_utils Utilities
 *
 * <p>There are also utilitiy classes that span layers. In particular,
 * Livingstone does not use the Standard Template Library for reasons of
 * portability. It defines its own string (MBS_string), growable array (Array),
 * singly-linked list (Slist), and hash table (Hash_table) classes. The
 * interfaces conform to the C++ Standard Template Library except where
 * noted.</p>
 *
 * \section glossary Glossary
 *
 * <p>Model-based reasoning, integrated vehicle health management (IVHM),
 * diagnosis systems and Livingstone employ a terminology that may not be
 * familiar to everyone who is interested in Livingstone. For this purpose,
 * a <a href="../L2Glossary.html">glossary</a> has been created.</p>
 *
 * \section references References
 *
 * The major sources of information about the theory behind Livingstone are:
 *
 * Brian C. Williams and P. Pandurang Nayak,
 * <a href=
 * "http://ic-www.arc.nasa.gov/ic/projects/mba/abstracts/livingstone.html>
 * A model-based approach to reactive self-configuring systems.</a>
 * Proceedings of the Thirteenth National Conference on Artificial
 * Intelligence (AAAI-96),
 * Menlo Park, CA: AAAI Press, 1996, 971-978.
 *
 * James A. Kurien and P. Pandurang Nayak,
 * <a href=
 * "http://ic-www.arc.nasa.gov/ic/projects/mba/abstracts/aaai-2000.html">
 * Back to the future with consistency-based trajectory tracking.</a>
 * Proceedings of the Seventeenth National Conference on Artificial
 * Intelligence (AAAI-2000),
 * Menlo Park, CA: AAAI Press, 2000, 370-377.
 */

#endif

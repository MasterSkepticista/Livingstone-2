/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: parameters.h,v 1.16 2004/05/25 22:12:57 lbrown Exp $ */
#ifndef parameters_HEADER
#define parameters_HEADER

#include <mba_utils/MBA_string.h>
#include <mba_utils/array_dict.h>
#include <transition/tracker.h> // for FC_style

/**
  * A set of configuration parameters for Livingstone.
  * This file can read itself off disk with method
  * L2_parameters::read_from_file(), or one can use the copious get/set
  * methods, which are mirrored in class Livingstone.
  * The parameters are as follows.
  *
  * <table width="3" border="1" width="100%">
  *   <tr>
  *     <td><b>Parameter Print Name</b></td>
  *     <td><b>Domain</b></td>
  *     <td><b>Default Value</b></td>
  *   </tr>
  *   <tr>
  *     <td>L2SearchMethod</td>
  *     <td>{ "cbfs", "cover" }</td>
  *     <td>"cbfs"</td>
  *   </tr>
  *   <tr>
  *     <td>L2MaxCBFSCandidates</td>
  *     <td>unsigned; 0 signifies unbounded</td>
  *     <td>5</td>
  *   </tr>
  *   <tr>
  *     <td>L2MaxCBFSSearchSpace</td>
  *     <td>unsigned</td>
  *     <td>3500</td>
  *   </tr>
  *   <tr>
  *     <td>L2MaxCBFSCutoffWeight</td>
  *     <td>unsigned; 0 signifies unbounded</td>
  *     <td>100</td>
  *   </tr>
  *   <tr>
  *     <td>L2MaxCoverCandidateRank</td>
  *     <td>unsigned; 0 signifies unbounded</td>
  *     <td>8</td>
  *   </tr>
  *   <tr>
  *     <td>L2MaxHistorySteps</td>
  *     <td>unsigned; 0 signifies unbounded</td>
  *     <td>3</td>
  *   </tr>
  *   <tr>
  *     <td>L2ProgressCmdType</td>
  *     <td>{"min", "full"}</td>
  *     <td>"full"</td>
  *   </tr>
  *   <tr>
  *     <td>L2NumTrajectoriesTracked</td>
  *     <td>unsigned; 0 signifies unbounded</td>
  *     <td>5</td>
  *   </tr>
  *   <tr>
  *     <td>L2FindCandidatesCmdType</td>
  *     <td>{"the tracker default fc", "default", "none",
  *          "extend"
  *          "prune-and-search", "prune-search", "ps",
  *          "find-fresh", "ff"}</td>
  *     <td>"default"</td>
  *   </tr>
  * </table>
  */

class L2_parameters {
 public:
  /// Set the default values for the parameters.
  L2_parameters();

  /**
   * Read the parameter values from the file with the given pathname.
   * Each line in the file has the form &lt;parameter&gt; = &lt;value&gt;.
   * \param filename the name of the file without the extension
   * \return whether the file was read without a problem
   */
  bool read_from_file(const MBA_string& filename);
  
  /// Print the parameter values to the supplied ostream.
  void print(_STD_ ostream& outstream) const;

  /// Return the Tracker search style as a sybmolic constant.  
  Tracker::Search_style get_search_style() const { return search_style_; }

  /// Set the Tracker search style as a sybmolic constant. 
  void set_search_style(Tracker::Search_style style) { search_style_ = style; }

  /// Return the Tracker search style as a print name.
  MBA_string get_search_style_str() const {
    return get_search_style(get_search_style());
  }

  /// Map the Tracker search style symbolic constant onto its print name.
  static MBA_string get_search_style(Tracker::Search_style);

  /// Map the Tracker search style print name onto its symbolic constant.
  static Tracker::Search_style get_search_style(const MBA_string&);

  /// Return the CBFS_tracker maximum Candidate count.
  unsigned get_cbfs_max_candidates() const { return cbfs_max_candidates_; }

  /// Set the CBFS_tracker maximum Candidate count.
  void set_cbfs_max_candidates(unsigned maxcand) {
    cbfs_max_candidates_ = maxcand;
  }

  /**
   * Return the maximum number of Candidate equivalence classes returned by the
   * CBFS_tracker.
   */
  unsigned get_cbfs_max_candidate_classes() const {
    return cbfs_max_candidate_classes_;
  }

  /**
   * Set the maximum number of Candidate equivalence classes returned by the
   * CBFS_tracker.
   * \param maxClassCount the maximum number of equivalence classes; 0 means
   * unlimited.
   */
  void set_cbfs_max_candidate_classes(unsigned maxClassCount) {
    cbfs_max_candidate_classes_ = maxClassCount;
  }

  /// Return the CBFS_tracker search space size.
  unsigned get_cbfs_search_space() const { return cbfs_search_space_; }

  /// Set the CBFS_tracker search space size.
  void set_cbfs_search_space(unsigned nsearched) {
    cbfs_search_space_ = nsearched;
  }

  /// Return the CBFS_tracker maximum cost/rank/weight.
  unsigned get_cbfs_max_rank() const { return cbfs_max_rank_; }

  /// Set the CBFS_tracker maximum cost/rank/weight.
  void set_cbfs_max_rank(unsigned weight) { cbfs_max_rank_ = weight; }

  /// Return the Cover_tracker maximum cost/rank/weight.
  unsigned get_cover_max_rank() const { return cover_max_rank_; }

  /// Set the Cover_tracker maximum cost/rank/weight.
  void set_cover_max_rank(unsigned weight) { cover_max_rank_ = weight; }

  /// Return the maximum number of past time steps retained by the T_system.
  unsigned get_history() const { return history_; }

  /// Set the maximum number of past time steps retained by the T_system.
  void set_history(unsigned h) { history_ = h; }

  /// Return the T_system progress style as a sybolic constant.
  T_system::Progress_style get_progress_style() const {
    return progress_style_;
  }

  /// Set the T_system progress style as a symbolic constant.
  void set_progress_style(T_system::Progress_style style) {
    progress_style_ = style;
  }

  /// Return the T_system progress style as a print name.
  MBA_string get_progress_style_str() const {
    return get_progress_style(get_progress_style());
  }

  /// Map the T_system progress style symbolic constant onto its print name.
  static MBA_string get_progress_style(T_system::Progress_style);

  /// Map the T_system progress style print name onto its symbolic constant.
  static T_system::Progress_style get_progress_style(const MBA_string&);
  
  /**
   * Return the maximum number of Candidates retained when the T_system
   * truncates.
   */
  unsigned get_max_truncated_candidates() const {
    return max_truncated_candidates_;
  }

  /**
   * Set the maximum number of Candidates retained when the T_system truncates.
   */
  void set_max_truncated_candidates(unsigned n) {
    max_truncated_candidates_ = n;
  }

  /// Return the Tracker find Candidates style as a symbolic constant.
  Tracker::FC_style get_find_candidates_style() const { return fc_style_; }

  /// Return the Tracker find Candidates style as a print name.
  MBA_string get_find_candidates_style_str() const {
    return get_find_candidates_style(get_find_candidates_style());
  }

  /// Return the Tracker find Candidates style as a print name.
  void set_find_candidates_style(Tracker::FC_style style) { fc_style_ = style; }

  /**
   * Map the Tracker find Candidates style symbolic constant onto its print
   * name.
   */
  static MBA_string get_find_candidates_style(Tracker::FC_style);

  /**
   * Map the Tracker find Candidates style print name onto its symbolic
   * constant.
   */
  static Tracker::FC_style get_find_candidates_style(const MBA_string&);

 private:
  /// The Tracker search style.
  Tracker::Search_style search_style_;

  /// The CBFS_tracker maximum Candidate count.
  unsigned cbfs_max_candidates_;

  /// The CBFS_tracker maximum equivalence class count
  unsigned cbfs_max_candidate_classes_;

  /// The CBFS_tracker maximum search space.
  unsigned cbfs_search_space_;

  // The following 2 are separated so that we can set both and use only one.
  /// The CBFS_tracker maximum Candidate cost/rank/weight
  unsigned cbfs_max_rank_;   

  /// The Cover_tracker maximum Candidate cost/rank/weight.
  unsigned cover_max_rank_;  

  /// The maximum number of past time steps retained; 0 means unbounded.
  unsigned history_;

  /// The T_system progress style, minimal or full.
  T_system::Progress_style progress_style_;

  /// The maximum Candidates count to retain at truncation; 0 means unbounded.
  unsigned max_truncated_candidates_;

  /// The Tracker find Candidates style.
  Tracker::FC_style fc_style_;
  
  // The follwing "dictionaries" map sybolic constants onto print names.
  // Search is stopped by the last item having an invalid style.

  /// Dictionary for Tracker search style.
  typedef String_key_dict_item<Tracker::Search_style> Search_item;
  static Search_item      search_dict  [];

  /// Dictionary for T_system progress style.
  typedef String_key_dict_item<T_system::Progress_style> Progress_item;
  static Progress_item    progress_dict[];

  /// Dictionary for Tracker find Candidates style.
  typedef String_key_dict_item<Tracker::FC_style> FC_item;
  static FC_item          fc_dict      [];  
};

#endif

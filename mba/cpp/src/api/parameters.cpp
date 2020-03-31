/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: parameters.cpp,v 1.27 2004/05/27 23:06:53 lbrown Exp $ */
#include <api/parameters.h>
#include <stdio.h> // fopen()
#include <stdlib.h> // atoi
#include <ctype.h> // isspace
#include <cover/cover_tracker.h>
#include <livingstone/L2_assert.h>
#include <livingstone/L2_fstream.h>
#include <mba_utils/stringops.h>
#include <opsat/cbfs_tracker.h>

/***************************************************************************
  Dictionary functions.
 ***************************************************************************/

L2_parameters::Search_item L2_parameters::search_dict[] = {
  { "cbfs"   , Tracker::cbfs },
  { "cover"  , Tracker::cover },
  { "invalid", Tracker::invalid_search }
};


L2_parameters::Progress_item L2_parameters::progress_dict[] = {
  { "full"   , T_system::full },
  { "min"    , T_system::min },
  { "invalid", T_system::invalid_progress }
};


L2_parameters::FC_item L2_parameters::fc_dict[] = {
  // this first one is just for printing really
  { "the tracker default fc", Tracker::tracker_default_fc },
  { "default",                Tracker::tracker_default_fc },
  { "none",                   Tracker::tracker_default_fc },

  { "extend",                 Tracker::extend_fc },

  { "prune-and-search",       Tracker::prune_and_search_fc },
  { "prune-search",           Tracker::prune_and_search_fc },
  { "ps",                     Tracker::prune_and_search_fc },

  { "find-fresh",             Tracker::find_fresh_fc },
  { "ff",                     Tracker::find_fresh_fc },
  { "findfresh",              Tracker::find_fresh_fc },

  // this one must be last ; name is ignored
  { "invalid",                Tracker::invalid_fc }
};


MBA_string L2_parameters::get_search_style(Tracker::Search_style value) {
  return search_dict_by_elt(search_dict, Tracker::invalid_search, value);
}

Tracker::Search_style
L2_parameters::get_search_style(const MBA_string& name) {
  return search_dict_by_key(search_dict, Tracker::invalid_search, name.c_str());
}

MBA_string L2_parameters::get_progress_style(T_system::Progress_style value) {
  return search_dict_by_elt(progress_dict, T_system::invalid_progress, value);
}

T_system::Progress_style
L2_parameters::get_progress_style(const MBA_string& name) {
  return search_dict_by_key(progress_dict, T_system::invalid_progress, name.c_str());
}

MBA_string L2_parameters::get_find_candidates_style(Tracker::FC_style value) {
  return search_dict_by_elt(fc_dict, Tracker::invalid_fc, value);
}

Tracker::FC_style
L2_parameters::get_find_candidates_style(const MBA_string& name) {
  return search_dict_by_key(fc_dict, Tracker::invalid_fc, name.c_str());
}


/***************************************************************************
  Starting up.
 ***************************************************************************/

L2_parameters::L2_parameters() : 
  search_style_(Tracker::DEFAULT_SEARCH_STYLE),
  cbfs_max_candidates_(CBFS_tracker::DEFAULT_MAXIMUM_CANDIDATE_COUNT),
  cbfs_max_candidate_classes_(CBFS_tracker::DEFAULT_MAXIMUM_CANDIDATE_CLASSES),
  cbfs_search_space_(CBFS_tracker::DEFAULT_MAXIMUM_SEARCH),
  cbfs_max_rank_(CBFS_tracker::DEFAULT_CBFS_MAX_CUTOFF_WEIGHT),
  cover_max_rank_(Cover_tracker::DEFAULT_COVER_MAX_RANK),
  history_(T_system::DEFAULT_HISTORY_LENGTH),
  progress_style_(T_system::DEFAULT_PROGRESS_STYLE),
  max_truncated_candidates_(T_system::DEFAULT_NUM_HISTORIES),
  fc_style_(Tracker::tracker_default_fc) { }


/***************************************************************************
  Parsing in the params file.
 ***************************************************************************/

bool L2_parameters::read_from_file(const MBA_string& filename) {
  // Class Parameters really shouldn't be doing I/O; this is for readers.
  // In the meantime, to accommodate older compilers, use C stdio file input.
  FILE* input = fopen((filename + ".params").c_str(), "r");
  if (!input) { input = fopen((filename + ".par").c_str(), "r"); }
  if (!input) {
    // If the model file is not XMPL, filename will have an extension.
    // Try stripping off the extension and appending ".par"
    MBA_string::size_type pi = filename.find(".");
    if (pi != MBA_string::npos) {
      // There is a period in the pathname. Assume there is only one.
      assert(filename.size() < 256);
      char s[256];
      // Copy the entire pathname to the buffer
      strcpy(s, filename.c_str());
      // Overwrite the old extension with the new extension
      strcpy(s + pi, ".par");
      input = fopen(s, "r");
    }
  }
  // Ran out of things to try
  if (!input) return false;

  char line[MBA_MAX_LINE_SIZE];
  unsigned linenum = 0;
  while ( ++linenum, fgets(line, MBA_MAX_LINE_SIZE, input)) {
    MBA_string var, value;
    if (!split_equality(line, var, value)) {
      if (var=="") {
	// empty line, that's ok
	continue;
      }
      // varname[1] may be the null character, but it's certainly in
      // bounds because the string isn't empty.  Get c_str because the
      // operator[] bounds-checks to keep from accessing the null char.
      if (var.c_str()[0] != 'L' || var.c_str()[1] != '2') {
	// comment or irrelevant line, also ok
	continue;
      }

      // if here, we actually found something but failed to parse
      L2_throw(L2_parse_params_error,
	       (filename + ":" + MBA_string(linenum)+
		":   couldn't parse `" + line + "'"));
    }

    // Used except in a few cases.  We can hope the optimizer will fix that;
    // or not care since this is non-flight.
    int intval = atoi(value.c_str());

    if (var == "L2SearchMethod") {
      set_search_style(get_search_style(value));
    } else if (var == "L2MaxCBFSCandidates") {
      set_cbfs_max_candidates(intval);
    } else if (var == "L2MaxCBFSCandidateClasses") {
      set_cbfs_max_candidate_classes(intval);
    } else if (var == "L2MaxCBFSSearchSpace") {
      set_cbfs_search_space(intval);
    } else if (var == "L2MaxCBFSCutoffWeight") {
      set_cbfs_max_rank(intval);
    } else if (var == "L2MaxCoverCandidateRank") {
      set_cover_max_rank(intval);
    } else if (var == "L2MaxHistorySteps") {
      set_history(intval);
    } else if (var == "L2ProgressCmdType") {
      set_progress_style(get_progress_style(value));
    } else if (var == "L2NumTrajectoriesTracked") {
      set_max_truncated_candidates(intval);
    } else if (var == "L2FindCandidatesCmdType") {
      set_find_candidates_style(get_find_candidates_style(value));
    } else {
      L2_throw(L2_parse_params_error, ("unknown parameter `"+var+"'"));
    }
  }
  fclose(input);

  // If the file is empty, linenum will equal one.
  if (1 == linenum) {
    return false; // Return that we didn't read anything.
  }

  return true;
}

/***************************************************************************
  Print the parameters.
 ***************************************************************************/
void L2_parameters::print (_STD_ ostream& stream) const {
  stream
    << "Search settings:" << _STD_ endl
    << "   Search type               = " << get_search_style_str()
    << _STD_ endl;

#ifndef DISABLE_CBFS
    if (get_search_style() == Tracker::cbfs) {
      stream
	<< "   Max candidates returned   = " << get_cbfs_max_candidates()
	<< _STD_ endl
	<< "   Max candidates searched   = " << get_cbfs_search_space()
	<< _STD_ endl
	<< "   Max candidate classes     = " << get_cbfs_max_candidate_classes()
	<< _STD_ endl
	<< "   Max rank                  = " << get_cbfs_max_rank()
	<< _STD_ endl;
    }
#endif

#ifndef DISABLE_COVER
    if (get_search_style()==Tracker::cover) {
        stream
	  << "   Max rank                  = " << get_cover_max_rank()
	  << _STD_ endl;
    }
#endif

    stream << "   History length            = ";
    if (get_history()) {
      stream << get_history() << _STD_ endl;
    } else {
      stream << "unlimited" << _STD_ endl;
    }

    stream
      << _STD_ endl
      << "   Max candidates tracked    = " << get_max_truncated_candidates()
      << _STD_ endl
      << "   Progress style            = " << get_progress_style_str()
      << _STD_ endl
      << "   Find Candidates style     = " << get_find_candidates_style_str()
      << _STD_ endl;

    stream << "---------------\n";
}

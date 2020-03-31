#include <debuggers/L2_string_map.h>
#include <tms/ltms.h>

/**
 * Export a map from Proposition numbers to Proposition names
 */
class PropositionNameMapExporter {

 public:

  /**
   * Supply the LTMS, the ostream to which the map is written, and the
   * debugger's L2_string_map.
   */
  PropositionNameMapExporter(LTMS& ltms, ostream& os,
			     L2_string_map& l2_string_map) :
    ltms(ltms), os(os), l2_string_map(l2_string_map) { }

  /**
   * Export the index-Proposition name map.
   * \internal This can't be called 'export' because that's a C++ keyword.
   */
  void exportNameMap() {
    unsigned index = 1;
    for (Ptheory::prop_list::iterator it = ltms.begin_propositions();
	 it != ltms.end_propositions(); ++it, ++index) {
      Proposition *pProposition = *it;
      os << index << " " << l2_string_map.get_name_of_prop(pProposition)
	 << endl;
    }
  }

 private:

  /** The LTMS the name map of which is exported. */
  LTMS& ltms;
  /** The ostream to which to export the name map. */
  ostream& os;
  /** Maps objects to their print names */
  L2_string_map& l2_string_map;
};

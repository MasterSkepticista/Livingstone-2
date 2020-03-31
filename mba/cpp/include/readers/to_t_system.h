/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: to_t_system.h,v 1.21 2004/01/12 23:55:35 lbrown Exp $ */
#ifndef to_t_system_HEADER
#define to_t_system_HEADER

/**
  * Interface from memory to Livingstone.
  */

#include <readers/common.h>
#include <mba_utils/pooled.h>
#include <mba_utils/slist.h>
#include <readers/L2_file_writer.h>
#include <transition/T_system.h>

class T_system;
class Transitioned;
class Variable;
class Clause;
class L2_file;
class L2rVariable;
class L2rClause;
class L2rTransition;
class L2rPropVarVar;

#ifndef DISABLE_TO_T_SYS_LISTEN
class to_t_system_listener;
#endif

class to_t_system : public L2_file_writer {
    public:
        to_t_system(const L2_file *f, T_system *tsys);

        virtual ~to_t_system();

        bool write();

        // arguably this shouldn't be here
        void createInitialState();

#ifndef DISABLE_TO_T_SYS_LISTEN
        void add_listener(to_t_system_listener *listen) {
            listeners_.push_front(listen);
        }
        void remove_listener(to_t_system_listener *listen) {
            listeners_.remove_first(listen);
        }
#endif

 private:

	/**
	 * This inner class is used to make an iterative algorithm easier in
	 * expandClauseEquality(). A stack of these is maintained.
	 */
	class Partition : public Pooled {
	public:
	  /**
	   * Copy all three lists locally.
	   * \param pos Array of Proto_assignment for positive Propositions
	   * \param neg Array of Proto_assignment for negative Propositions
	   * \param equality Slist of L2rPropVarVar for Variable1=Variable2
	   * propositions
	   * \todo initializers for Proto_assignment_list caused problems, but
	   * explicit assignment seem to be OK. Copy constructor?
	   */
	  Partition(const T_system::Proto_assignment_list& pos,
		    const T_system::Proto_assignment_list& neg,
		    const Slist<const L2rPropVarVar*>& equality) :
	    duals(equality)       // copy
	    { 
	      for (unsigned i = 0; i < pos.size(); i++) {
		const T_system::Proto_assignment& proto_assignment = pos[i];
		positiveSingles.push(proto_assignment);
	      }
	      for (unsigned j = 0; j < neg.size(); j++) {
		const T_system::Proto_assignment& proto_assignment = neg[j];
		negativeSingles.push(proto_assignment);
	      }
	    }
	  T_system::Proto_assignment_list& getPositiveSingles() {
	    return positiveSingles;
	  }
	  T_system::Proto_assignment_list& getNegativeSingles() {
	    return negativeSingles;
	  }
	  const Slist<const L2rPropVarVar*>& getDuals() { return duals; }
	private:
	  T_system::Proto_assignment_list positiveSingles;
	  T_system::Proto_assignment_list negativeSingles;
	  Slist<const L2rPropVarVar*> duals;
	};

    private:
        void createVariable(const L2rVariable*);
        void createBackground(const L2rClause*);
        void createTransitions(const L2rVariable*);
        void createTransition(Transitioned*,const L2rTransition*,
                unsigned);

        void expandClause(const L2rClause*, bool);

        Variable *findVar(const L2rVariable*) const;

	void expandClauseEquality(const Slist<const L2rPropVarVar*>& equality,
				  T_system::Proto_assignment_list& pos,
				  T_system::Proto_assignment_list& neg,
				  bool state_constr);
    private:
        T_system *t_system;

#ifndef DISABLE_TO_T_SYS_LISTEN
        Slist<to_t_system_listener*> listeners_;

        // needed for the implementation of some of the callbacks
        class tms_listener;
        tms_listener *internal_tms_listen;
#endif

};


#endif

/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: L2_string_map.cpp,v 1.49 2004/02/06 18:53:27 lbrown Exp $ */
#include <stdio.h> // sprintf
#include <conflict_db/variable.h>
#include <debuggers/domain.h>
#include <debuggers/L2_string_map.h>
#include <debuggers/trajectory.h>
#include <readers/L2_file.h>
#include <readers/variable.h>    // get_time_step()
#include <transition/transitioned.h> // class Transitioned

/**
  * Used by everything that reads from a hash table.
  * We do the error-checking here; it's illegal not to find.
  * If you want to allow not finding the key, write it yourself.
  */

template <class Key, class Elt>
static Elt get_element_for_key(const Key k,
			       Hash_table<Key, Elt>& table,
			       const MBA_string& forwhat,
			       const MBA_string& bywhat) {
  typename Hash_table<Key,Elt>::iterator it = table.find(k);
  if (it == table.end()) {
    unsigned sz;
    sz = table.size(); // for debugging ; avoid a warning
    L2_assert(it != table.end(),
	      L2_not_found_error,
	      ("failed search for " + forwhat + " by " + bywhat));
  }
  return *it;
}


/***************************************************************************
  Constructor ; mostly just initialize the hashtables with the right
  hash functions.
 ***************************************************************************/

L2_string_map::L2_string_map() :
  name2id(hashpjw),
  // The L2_file hasn't been filled in yet -- or even created -- so we
  // initialize these to zero and require the user to call create_domains
  // when the file has been read
  varid2domain(0),
  xvarid2domain(0),
  prop2varval(hash_integral_key)
{
  state_domain = new Domain("state domain", 1);;
}


/***************************************************************************
  Destructor ; deallocate the values.
 ***************************************************************************/

L2_string_map::~L2_string_map() {
  // delete the domains
  for (Slist<const Domain*>::iterator it = begin_domains();
       it != end_domains(); ++it) {
    const Domain* pDomain = *it;
    delete const_cast<Domain*>(pDomain);
  }
  L2_free_array(varid2domain, nvars);
  L2_free_array(xvarid2domain, nvars);
  delete state_domain;
  // the rest is automatic
}


/***************************************************************************
  Keeping track of the variable/name/model mappings.
 ***************************************************************************/
static const char * str_pound = "#";
static const char * str_Transition = "Transition";

MBA_string L2_string_map::get_variable_name(const Variable& variable) const {
  unsigned timeStep = variable.get_time_step();
  if (variable.is_state_variable()) {
    return MBA_string(timeStep) + "#state";
  } else {
    const unsigned variableID = variable.get_id();
    MBA_string name = name_by_id[variableID];
    if (variable.is_transition()) {
      name = name + str_Transition; // convert "mode" to "modeTransition"
      if (timeStep == T_system::BEFORE_HORIZON) {
	return "-#" + name;         // '-' represents "before horizon"
      }
    }
    // Not a horizon Transition
    return form_name_at_time(name, timeStep);
  }
}

MBA_string L2_string_map::get_variable_name(const Variable& variable,
					    unsigned timeStep) const {
  if (variable.is_state_variable()) {
    return MBA_string(timeStep) + "#state";
  } else {
    const unsigned variableID = variable.get_id();
    MBA_string name = name_by_id[variableID];
    if (variable.is_transition()) {
      name = name + str_Transition; // convert "mode" to "modeTransition"
      if (timeStep == T_system::BEFORE_HORIZON) {
	return "-#" + name;         // '-' represents "before horizon"
      }
    }
    // Not a horizon Transition
    return form_name_at_time(name, timeStep);
  }
}


/***************************************************************************
  Finding variables by name.
 ***************************************************************************/

Variable& L2_string_map::get_variable_by_name(const MBA_string& variableName) {
  Variable *const pVariable = get_var_or_null(variableName);
  L2_assert(pVariable,
	    L2_not_found_error,
	    ("no variable by the name of " + variableName));
  return *pVariable;
}


bool L2_string_map::has_variable_named(const MBA_string& variableName) {
  return get_var_or_null(variableName) != NULL;
}


Variable* L2_string_map::get_var_or_null(const MBA_string& n) {
  // Copy the name so the copy can be modified
  MBA_string name = n;

  // Figure out the time. '-' means BEFORE_HORIZON, empty time string means
  // NOW; otherwise, use atoi
  unsigned time;
  // The index of the '#' character in the name string
  int pound_index = name.find(str_pound);
  if (pound_index < 0) {
    // '#' absent; assume NOW
    time = T_system::NOW;
  } else {
    // '#' present
    // The string up to the '#' character
    MBA_string timestr(name.c_str(), pound_index);
    if (timestr == "-") {
      // Prefix is "-#"; this means BEFORE_HORIZON
      time = T_system::BEFORE_HORIZON;
    } else {
      // Prefix is "<TimeStep>#", where <TimeStep> is assumed to be an int
      time = atoi(timestr.c_str());
    }
    // skip the '#'; the remainder is assumed to be the variable name
    name = name.c_str() + pound_index + 1;
  }

  if (name == "state") {
    if (state_var && time == state_var->get_time_step()) {
      // The cached State Variable is for the requested time step; return it
      return state_var;
    } else {
      // else fall through, which won't find it, because there's only one
    }
  }

  // If it ends in "Transition", remove it to find the ID; but remember the
  // fact.

  bool isTransition = false;
  const int nameLength       = name.size();
  const int transitionLength = strlen(str_Transition);
  const int difference       = (nameLength - transitionLength);
  if (nameLength >= transitionLength && 
      0 == strcmp(name.c_str() + difference, str_Transition)) {
    isTransition = true;
    name = MBA_string(name.c_str(), difference);
  }

  // Find the ID
  Hash_table<MBA_string, unsigned>::iterator it = name2id.find(name);
  if (it == name2id.end()) {
    // The Variable wasn't in the name-to-ID map
    return NULL;
  } else {
    // The Variable was in the name-to-ID map
    unsigned id = *it;
    // Find the Variable.  We do this differently depending on whether it's a
    // mode transition or a normal Variable.
    Variable *pVariable;
    if (isTransition) {
      Transitioned *mode = static_cast<Transitioned*>(nowvar_by_id[id]);
      pVariable = mode->get_zenith();
    } else {
      pVariable = nowvar_by_id[id];
    }

    // Follow links back to the requested time step
    while (pVariable && pVariable->get_time_step() != time) {
      pVariable = pVariable->get_previous();
    }

    // finally!
    return pVariable;
  }
}


const L2rVariable* 
L2_string_map::get_model_variable(const Variable& variable) const {
  if (variable.is_state_variable()) {
    // There is no L2rVariable that corresponds to the State Variable
    return NULL;
  } else {
    // All other Variables should be in the (ID, pL2rVariable) mapping
    return readvar_by_id[variable.get_id()];
  }
}


const MBA_string *
L2_string_map::get_variable_values(const L2rVariable *pL2rVariable) const {
  return get_variable_domain(pL2rVariable)->get_values();
}


const MBA_string *
L2_string_map::get_variable_values(const Variable& variable) const {
  return get_variable_domain(variable)->get_values();
}


const Domain *
L2_string_map::get_variable_domain(const L2rVariable *pL2rVariable) const {
  return varid2domain[pL2rVariable->id()];
}


const Domain *
L2_string_map::get_variable_domain(const Variable& variable) const {
  if (variable.is_state_variable()) {
    // The State Variable has its own domain
    return state_domain;
  } else {
    const unsigned id = variable.get_id();
    /// If we are doing recovery, then there are no modeTransitions
    /// and command variables are assumptions.
    // discover if it's a modeTransition
    if (variable.is_transition()) {
      // modeTransition variable
      return xvarid2domain[id];
    } else {
      // normal variable
      return varid2domain[id];
    }
  }
}


Variable&
L2_string_map::get_now_variable(const L2rVariable* pL2rVariable) {
  return *nowvar_by_id[pL2rVariable->id()];
}


void L2_string_map::create_domains(const L2_file& f) {
  nvars = f.nvars();
  varid2domain  = L2_alloc_array(const Domain* , nvars);
  xvarid2domain = L2_alloc_array(const Domain* , nvars);

  unsigned i;
  Hash_table<const L2rEnumeration*, const Domain*>
    en2domain(hash_integral_key);
  for (i=0; i<f.nenums(); ++i) {
    const L2rEnumeration *en = f.getEnum(i);
    Domain *d = new Domain(en);
    domains.push_front(d);
    en2domain.insert(en, d);
  }

  for (i = 0; i < nvars; ++i) {
    const L2rVariable *var = f.getVar(i);
    varid2domain[i] = *en2domain.find(var->type());

    if (var->kind() != vk_mode) {
      xvarid2domain[i] = NULL;
    } else {
      const L2rMode *m = var->mode();
      Domain *d = new Domain(m);
      domains.push_front(d);
      xvarid2domain[i] = d;
    }
  }
}


void L2_string_map::add_variable(const L2rVariable* read, Variable& var) {
  MBA_string name;
  unsigned id = read->id();

  if (read->hasDebug()) {
    name = static_cast<const dbg_L2rVariable*>(read)->name();
  } else {
    name = MBA_string(id);
  }
  // Set name to ID mapping
  name2id.insert(name, id);
  // Set ID to name, index to pVariable, index to pL2rVariable mappings
  name_by_id   .grow_and_set(id, name);
  nowvar_by_id .grow_and_set(id, &var);
  readvar_by_id.grow_and_set(id, read);
  add_variable(var);
}


// Note: the regression tests passed even when i was not incremented, so this
// value may be unused

void L2_string_map::add_variable(Variable& variable) {
  // The index of the Proposition in the Variable
  unsigned i = 0;
  for (unsigned j = 0; j < variable.get_nvalues(); j++, i++) {
    // Map the Proposition onto a (pVariable, index) pair
    prop2varval.insert_duplicate(variable.get_prop(j),
				 MBApair<Variable*, unsigned>(&variable,i));
  }
  // The the Variable is the State Variable, cache it
  if (variable.is_state_variable()) {
    state_var = &variable;
  }
}


void L2_string_map::remove_variable(const Variable& variable) {
  if (variable.get_time_step() == T_system::NOW &&
     !variable.is_assumption()) {
    const unsigned id = variable.get_id();
    name2id.erase(name_by_id[id]);
    name_by_id[id]   = "";
    nowvar_by_id[id] = NULL;
    readvar_by_id[id]= NULL;
  }

  if (&variable == state_var) {
    state_var = NULL;
  }

  // affected clauses and props will be removed separately
}


// This function should not be called if the name is that of a Transition.

MBA_string L2_string_map::form_name_at_time(const MBA_string& name,
					    unsigned timestep) {
  if (timestep == T_system::NOW) {
    return name;
  } else {
    char result[MBA_MAX_LINE_SIZE];
    // skip over a previous time specification
    const char *namep = strchr(name.c_str(), '#');
    if (namep) {
        ++namep;
    } else {
        namep = name.c_str();
    }
    sprintf(result, "%u#%s", timestep, namep);
    return result;
  }
}


/***************************************************************************
  Keeping track of the clause/component mapping
 ***************************************************************************/

#if 0
MBA_string L2_string_map::get_component_of_clause(Clause *pClause) {
  return get_element_for_key(pClause, clause2whence, "component", "clause");
}

void L2_string_map::add_clause(Clause* pClause, MBA_string component) {
  clause2whence.insert(pClause, component);
}

void L2_string_map::remove_clause(Clause* pClause) {
  clause2whence.erase(pClause);
}
#endif

/***************************************************************************
  Keeping track of the prop/assignment and name/prop mappings
 ***************************************************************************/

MBApair<Variable*, unsigned>
L2_string_map::get_assignment_of_prop(Proposition* pProposition) {
  return get_element_for_key(pProposition, prop2varval,
			     "assignment", "proposition");
}


MBA_string L2_string_map::get_name_of_prop(Proposition* pProposition) {
  MBApair<Variable*, unsigned> assignment =
    get_assignment_of_prop(pProposition);
  const Variable * const pVariable = assignment.first;
  const unsigned valueIndex = assignment.second;
  return get_variable_name(*pVariable) + "=" +
    get_variable_values(*pVariable)[valueIndex];
}


// The "propositionName" string is assumed to be "<variableName>=<valueName>"

Proposition* L2_string_map::get_prop_by_name(const MBA_string&
					     propositionName) {
  // The index of the '=' character in the name string
  const int eqIndex = propositionName.find("=");
  L2_assert(eqIndex >= 0,
	    L2_argument_error,
	    ("no '=' found in `" + propositionName + "'"));
  const MBA_string variableName(propositionName.c_str(), eqIndex);
  const MBA_string valueName(propositionName.c_str() + eqIndex + 1); // skip the '='

  // Map the variable name onto the its Variable
  const Variable& variable = get_variable_by_name(variableName);
  const int valueIndex = get_variable_domain(variable)->value(valueName);
  L2_assert(valueIndex != Domain::NOT_FOUND,
	    L2_not_found_error,
            ("no value `" + valueName +
	     "' found for variable `" + variableName + "'"));
  return variable.get_prop(valueIndex);
}


void L2_string_map::remove_prop(Proposition *pProposition) {
  prop2varval.erase(pProposition);
}


void L2_string_map::revise_state_domain(unsigned size) {
  if (size != state_domain->size()) {
    delete state_domain;
    state_domain = new Domain("state", size);
  }
}

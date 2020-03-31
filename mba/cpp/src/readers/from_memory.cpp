/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: from_memory.cpp,v 1.7 2004/01/15 19:04:40 lbrown Exp $ */

#include <fcntl.h>                 // For macro O_RDONLY
#include <readers/from_memory.h>
#include <readers/L2_file.h>
#include <readers/transition.h>

/***************************************************************************
  Constructor/destructor.
 ***************************************************************************/

/* Everything is done in the initializers */

from_memory::from_memory(L2_file * f,
			 const unsigned char * const theModel,
			 size_t modelSize) :
  L2_file_reader(f),
  modelSize(modelSize),
  theModel(theModel),
  nextChar(0) {
}

/* No clean-up. from_bin owns theModel */

from_memory::~from_memory() {
}


/***************************************************************************
  reading the file
 ***************************************************************************/
bool from_memory::read() {
  if (dest() == 0) {
    return false;    // NULL destination ptr
  } else if (theModel == 0) {
    return false;    // NULL model ptr
  } else if (static_cast<int>(modelSize) <= 0) {
    return false;    // Bad model size
  } else if (!read_header()) {
    return false;    // Bad cookie, version or checksum
  } else {
    read_enums();
    read_variables();
    read_propositions();
    read_clauses();
    read_transitions();
    return true;
  }
}


/***************************************************************************
  reading the file header
 ***************************************************************************/

bool from_memory::read_header() {
  if (read_cookie() && read_version() && read_checksum()) {
    read_weights();
    read_counts();
    return true;
  } else {
    return false;
  }
}

bool from_memory::read_cookie() {
  // cookie; 4 bytes
  static unsigned char expected_cookie[] = "L2bi";
  for(unsigned i=0; i<4; ++i) {
    if(get1() != expected_cookie[i])
      // magic cookie mismatch error
      return false;
  }
  return true;
}

bool from_memory::read_version() {
  // version ; 2 bytes
  unsigned version = get(2);
  return (version == 3);
}

bool from_memory::read_checksum() {
  // checksum ; 4 bytes ; TODO: check it
  unsigned checksum;
  checksum = get(4);
  return true;
}

// weights ; 2 bytes,
// first byte is We,Wv,Wp,Wc
// second is Wch, Wx, Wxc
// Two bits per weight, read it out and add one for the number
// of bytes (0 means 1 byte, 3 means 4 bytes, etc).

void from_memory::read_weights() {
  unsigned pack;
  pack = get1();
  Wc = 1 + pack & 0x3;
  pack >>= 2;
  Wp = 1 + pack & 0x3;
  pack >>= 2;
  Wv = 1 + pack & 0x3;
  pack >>= 2;
  We = 1 + pack & 0x3;
  pack >>= 2;
  assert(pack==0);

  pack = get1();
  Wxc = 1 + pack & 0x3;
  pack >>= 2;
  Wx = 1 + pack & 0x3;
  pack >>= 2;
  Wch = 1 + pack & 0x3;
  assert(pack==0);
}

void from_memory::read_counts() {
  // number of each kind of thing
  nenums_     = get(We);
  nvars_      = get(Wv);
  nposprops_  = get(Wp);
  nnegprops_  = get(Wp);
  nsameprops_ = get(Wp);
  ndiffprops_ = get(Wp);
  nclauses_   = get(Wc);
}


/***************************************************************************
  Parsing enumerations
 ***************************************************************************/
void from_memory::read_one_enum(unsigned id) {
    unsigned nvalues = get(We);
    L2rEnumeration *en = new L2rEnumeration(id, nvalues);
    dest()->setEnum(id, en);
}

void from_memory::read_enums() {
    dest()->allocEnums(nenums_, false);
    for(unsigned i=0; i<nenums_; ++i) {
        read_one_enum(i);
    }
}

/***************************************************************************
  Parsing variables
 ***************************************************************************/
void from_memory::read_one_variable(unsigned id) {
  const unsigned enumID = get(We); // index of the domain Enumeration
  const unsigned kindint = get1(); // Variable subclass
  VarKind kind = vk_unknown;       // initialize to suppress compiler warning
  switch (kindint) {
  case 100: kind = vk_unknown;   break;
  case 101: kind = vk_observed;  break;
  case 102: kind = vk_commanded; break;
  case 103: kind = vk_mode;      break;
  default: L2_throw(L2_reader_error, ("unknown kind " + MBA_string(kindint)));
  }

  L2rVariable* const var = new L2rVariable(id, get_dest()->getEnum(enumID));
  var->setKind(kind);
  if (kind == vk_mode) {
    const unsigned initial = get(We); // value index of the initial value
    var->setInitial(initial);
  }
  
  dest()->setVar(id, var); // map the VariableID to the new L2rVariable object
}


void from_memory::read_variables() {
    dest()->allocVars(nvars_, false);
    for(unsigned i=0; i<nvars_; ++i) {
        read_one_variable(i);
    }
}

/***************************************************************************
  Parsing propositions
 ***************************************************************************/
void from_memory::read_one_varval(unsigned id, bool pos) {
    unsigned varID = get(Wv);
    unsigned value = get(We);
    L2rPropVarValue *p = new L2rPropVarValue(id,
            dest()->getVar(varID),
            pos,
            value);
    dest()->setProp(id, p);
}

void from_memory::read_one_varvar(unsigned id, bool pos) {
    unsigned var1 = get(Wv);
    unsigned var2 = get(Wv);
    L2rPropVarVar *p = new L2rPropVarVar(id,
            dest()->getVar(var1),
            pos,
            dest()->getVar(var2));
    dest()->setProp(id, p);
}

void from_memory::read_propositions() {
    unsigned prop_id = 0;
    unsigned i;
    dest()->allocProps(nposprops_ + nnegprops_ +
            nsameprops_ + ndiffprops_);

    for(i=0; i<nposprops_; ++i, ++prop_id)
        read_one_varval(prop_id, true);
    for(i=0; i<nnegprops_; ++i, ++prop_id)
        read_one_varval(prop_id, false);
    for(i=0; i<nsameprops_; ++i, ++prop_id)
        read_one_varvar(prop_id, true);
    for(i=0; i<ndiffprops_; ++i, ++prop_id)
        read_one_varvar(prop_id, false);
}

/***************************************************************************
  Parsing clauses
 ***************************************************************************/

void from_memory::read_one_clause(unsigned id) {
  // The first word is 2*PropositionCount + IsBackground
  unsigned pack = get(Wch);
  bool bg = bool(pack & 0x1);
  unsigned cls_nprops = pack >> 1;
  const L2rProposition **props =
    L2_alloc_array(const L2rProposition*, cls_nprops);
  
  // Read the Clause's proposition indices
  for(unsigned i=0; i<cls_nprops; ++i) {
    unsigned pid = get(Wp);
    props[i] = dest()->getProp(pid);
  }

  L2rClause *cls = new L2rClause(id, cls_nprops, props, false);
  if(bg) cls->putInBackground();
  
  dest()->setClause(id, cls);
}

void from_memory::read_clauses() {
  dest()->allocClauses(nclauses_, false);
  for(unsigned i=0; i<nclauses_; ++i) {
    read_one_clause(i);
  }
}


/***************************************************************************
  Parsing transitions
 ***************************************************************************/

/* A Transition has the form [rank|from] ToIndex ClauseCount ClauseID* */

void from_memory::read_one_transition(bool isnominal, const L2rVariable *m) {
  unsigned rank;
  int from;

  // Read "from" if nominal mode or "rank" if failure mode
  if(isnominal) {
    rank = 0;       // Nominal modes have rank 0
    from = get(We); // Read antecedent of nominal mode
  } else {
    rank = get1();  // Read rank of failure mode
    from = L2rTransition::ANY_MODE;// Failure modes have any mode as antecedent
  }

  unsigned to   = get(We);  // Destination
  unsigned ncls = get(Wxc); // Clause count
  const L2rClause **cls = L2_alloc_array(const L2rClause*, ncls);
  for(unsigned i=0; i<ncls; ++i) {
    unsigned cid = get(Wc); // Read index of clause
    cls[i] = get_dest()->getClause(cid);
  }

  L2rTransition *x =
    new L2rTransition(m, from, to, ncls, cls, false, isnominal, rank);
  dest()->addTransition(x);
}


/* For each Mode Variable, read its transitions */

void from_memory::read_transitions() {
  for(unsigned i=0; i<get_dest()->nvars(); ++i) {
    const L2rVariable *var = get_dest()->getVar(i);
    if(var->kind() != vk_mode) continue;

    unsigned nnominal = get(Wx);
    unsigned nfailure = get(Wx);
    unsigned j;

    for(j=0; j<nnominal; ++j)
      read_one_transition(true, var);
    for(j=0; j<nfailure; ++j)
      read_one_transition(false, var);
  }
}

/***************************************************************************
  Reading a byte or several bytes at a time.
 ***************************************************************************/

/* Get one byte. Used by get() */

unsigned char from_memory::get1() {
  L2_assert(nextChar < static_cast<unsigned int>(modelSize),
	    L2_reader_error,
            ("reached EOF prematurely"));
  int c = theModel[nextChar++];
  return c;
}

/* Get from 1 to 4 bytes, packed into the return value */

unsigned from_memory::get(unsigned width) {
    unsigned acc = 0;
    for(unsigned i=0; i<width; ++i) {
        acc <<= 8;
        acc += get1();
    }
    return acc;
}


/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: from_xmpl.cpp,v 1.38 2004/02/09 23:20:58 lbrown Exp $ */

#include <errno.h>
#include <math.h>
#include <stdlib.h>

#include <livingstone/L2_fstream.h>
#include <readers/from_xmpl.h>
#include <readers/transition.h>

#include <sax/HandlerBase.hpp>
#include <sax/AttributeList.hpp>
#include <parsers/SAXParser.hpp>
#include <util/PlatformUtils.hpp>

// The do-while(0) is the only portable way to block.
#ifdef ENABLE_L2_VERBOSE
#define verbose(expr) do { if (isVerbose()) { expr; } } while(0)
#else
#define verbose(expr)
#endif

template <class T>
T *list_to_array(Slist<T>& list) {
    T* ret = L2_alloc_array(T,list.size());
    unsigned i;
    typename Slist<T>::iterator it;
    for(i=0, it=list.begin(); it!=list.end(); ++i, ++it)
        ret[i] = *it;
    return ret;
}



/***************************************************************************
        The class that handles the parsing callbacks.
 ***************************************************************************/
class from_xmpl::HandlerBase_subclass : public HandlerBase {
    private:
        from_xmpl& owner_;
        AttributeList *attr_;

    public:

        MBA_string filename;

        HandlerBase_subclass(from_xmpl& owner)
            : owner_(owner)
        {
        }

        MBA_string xcode(const XMLCh* xmlstr) {
            return XMLString::transcode(xmlstr);
        }
        MBA_string get_attribute(const char *str) {
            const XMLCh* attr = attr_->getValue(str);
            if(attr)
                return xcode(attr);
            else
                return "";
        }

        /// These are callbacks declared in HandlerBase
        virtual void startElement(const XMLCh* const tagname,
                AttributeList& attributes) {
            attr_ = &attributes;
            MBA_string localTag(xcode(tagname));

            if(localTag == "ci:attribute") {
                MBA_string name = get_attribute("name");
                MBA_string type = get_attribute("type");

                owner_.start_attribute(name, type);
            }
            else if(localTag == "cmd") {
                MBA_string varname = get_attribute("name");
                owner_.start_cmd(varname);
            }
            else if(localTag == "obs") {
                MBA_string varname = get_attribute("name");
                owner_.start_obs(varname);
            }
            else if(localTag == "assign") {
                MBA_string prop = get_attribute("eq");
                owner_.start_assign(prop);
            }
            else if(localTag == "ci:attributetype") {
                MBA_string name = get_attribute("name");
                MBA_string values(get_attribute("members"));
                owner_.start_attributetype(name, values);
            }
            else if(localTag == "ci:component") {
                MBA_string name = get_attribute("name");
                owner_.start_component(name);
            }
            else if(localTag == "ci:clause") {
                owner_.start_clause();
            }
            else if(localTag == "ci:term") {
                owner_.start_prop();
            }
            else if(localTag == "ci:statevector") {
                MBA_string name = get_attribute("vars");
                owner_.start_statevector(name);
            }
            else if(localTag == "ci:transition") {
                MBA_string from, to, name, prob;

                from = get_attribute("from");
                to   = get_attribute("to");
                prob = get_attribute("probability");

                const XMLCh *xname = attributes.getValue("name");

                if(xname)
                    name = xcode(xname);
                else
                    name = from + "->" + to;
                owner_.start_transition(from, to, name, prob);
            }
        }

        virtual void characters(const XMLCh* const chars, const unsigned int) {
            owner_.characters(xcode(chars));
        }

        virtual void endElement(const XMLCh* const tagname) {
            MBA_string localTag(xcode(tagname));
            if(localTag == "ci:component")
                owner_.end_component();
            else if(localTag == "ci:clause")
                owner_.end_clause();
            else if(localTag == "ci:term")
                owner_.end_prop();
            else if(localTag == "ci:transition")
                owner_.end_transition();
        }



        virtual void warning(const SAXParseException& e) {
            _STD_ cerr<< filename << ":" << e.getLineNumber()
		     << " : " << xcode(e.getMessage()) << _STD_ endl;
        }

        virtual void error(const SAXParseException& e) {
            L2_throw( L2_reader_error,
                    ("Error at " + filename
                     + " line " + MBA_string(e.getLineNumber())
                     + " position " + MBA_string(e.getColumnNumber())
                     + ": " + xcode(e.getMessage())));
        }

        virtual void fatalError(const SAXParseException& e) {
            L2_throw( L2_reader_error,
                    ("Fatal Error at " + filename
                     + " line " + MBA_string(e.getLineNumber())
                     + " position " + MBA_string(e.getColumnNumber())
                     + ": " + xcode(e.getMessage())));
        }
};


/***************************************************************************
        Parsing probabilities into the ranks that Livingstone uses.
 ***************************************************************************/

// Take the negative log, base 10, of the probability to yield a rank.

static unsigned convert_to_rank(double prob) {
  L2_assert(prob >  0, L2_reader_error, ("probability <= 0"));
  L2_assert(prob <= 1, L2_reader_error, ("probability >  1"));
  int rank = (int)(0.5 - log10(prob));
  return rank;
}


// If the probability is one of { likely, lessLikely, unlikely, rare },
// map to a rank. Otherwise, try to interpret as a probability.

static unsigned convert_to_rank(const MBA_string& prob_string) {
  if      (prob_string == "likely")     { return 1; }
  else if (prob_string == "lessLikely") { return 2; }
  else if (prob_string == "unlikely")   { return 3; }
  else if (prob_string == "rare")       { return 4; }
  // Any Candidate containing an Assignment with this rank will have a rank
  // higher than any double-fault Candidate with only the previous four ranks
  // used. That way, unknown-fault Candidates can be segregated.
  else if (prob_string == "unknownFaultRank") { return 10; }
  else {
    double probability = atof(prob_string.c_str());
    L2_assert(probability != 0 || errno != EINVAL,
	      L2_reader_error,
	      ("probability unparseable"));
    return convert_to_rank(probability);
  }
}


/***************************************************************************
        Dictionary functions
 ***************************************************************************/
dbg_L2rEnumeration *from_xmpl::find_enum(const MBA_string& name) {
    Hash_table<MBA_string, dbg_L2rEnumeration*>::iterator it = enums_.find(name);
    L2_assert(it != enums_.end(), L2_not_found_error,
            ("enumeration `"+name+"' not found"));
    return *it;
}

static const char* noCommand = "noCommand";

dbg_L2rEnumeration *from_xmpl::create_enum(
        const MBA_string& name, Slist<MBA_string>& values) {

    // first, force noCommand to the start of the list
    Slist<MBA_string>::iterator it = values.begin();
    bool hasNoCommand = false;
    while(it != values.end()) {
        if(*it == noCommand) {
            hasNoCommand = true;
            values.erase(it);
        } else
            ++it;
    }
    if(hasNoCommand) values.push_front(noCommand);


    // create the new enumeration, give it its ID, add to the list
    unsigned nvalues = values.size();
    MBA_string *names = list_to_array(values);
    dbg_L2rEnumeration *newenum = new dbg_L2rEnumeration(nenums_++,
            nvalues, name, names);
    L2_free_array(names, nvalues);

#ifdef VXWORKS531
    const bool ok = enums_.insert(name, newenum);        // returns bool
#else
    const bool ok = enums_.insert(name, newenum).second; // returns pair
#endif
    L2_assert(ok, L2_reader_error, ("enumeration `"+name+"' already exists"));
    verbose(newenum->toOStream_long(_STD_ cout));
    return newenum;
}


int from_xmpl::find_enum_member(const L2rEnumeration *t,
				const MBA_string& member) {
    assert(t->hasDebug());
    const dbg_L2rEnumeration *type = (dbg_L2rEnumeration*)t;

    if(member == "*") // required for transitions
        return L2rTransition::ANY_MODE;
    for(unsigned i=0; i<type->nmembers(); i++) {
        if(member==type->name(i))
            return i;
    }
    return NOT_FOUND;
}



L2rVariable *from_xmpl::find_variable(const MBA_string& name, bool fatal) {
    varsIT it = vars_.find(name);
    if(it == vars_.end()) {
        if(fatal) {
            L2_assert(it != vars_.end(), L2_not_found_error,
                    ("variable `"+name+"' not found"));
        } else
            return 0;
    }
    return *it;
}

L2rVariable *from_xmpl::create_variable(
        const dbg_L2rEnumeration *type, const MBA_string& name) {

    dbg_L2rVariable *v = new dbg_L2rVariable(nvars_++, type, name);
#ifdef VXWORKS531
    const bool ok = vars_.insert(name, v);        // returns bool
#else
    const bool ok = vars_.insert(name, v).second; // returns pair
#endif
    L2_assert(ok, L2_reader_error, ("variable `"+name+"' already exists"));
    verbose(v->toOStream_long(_STD_ cout));
    return v;
}



L2rProposition *from_xmpl::find_or_create_prop(
        const L2rVariable *var, bool isPositive, const MBA_string& equalTo) {
    // first, find out what `equalTo' refers to -- a variable or a value
    // Search for a value; if not found, search for a variable.

    // For ease-of-implementation, we create the prop and then use
    // operator == ; this is inefficient but I don't really care since
    // this is only used on the ground.
    int value = find_enum_member(var->type(), equalTo);
    L2rProposition *prop;
    if(value>=0) {
        // found a value of the given name, so use it
        prop = new L2rPropVarValue(nprops_, var, isPositive, value);
    } else {
        // no such value; look for a variable.
        MBA_string varname = component_name + "." + equalTo;
        L2rVariable *var2 = find_variable(varname, false);
        L2_assert(var2, L2_not_found_error,
                ("no value or variable `"+equalTo+"' found"));
        prop = new L2rPropVarVar(nprops_, var, isPositive, var2);
    }

    // linear search; surely I could do better!
    for(propsIT it = props_.begin() ; it != props_.end() ; ++it ) {
        if(*prop == **it) {
            // found a previous identical prop; delete the new one and
            // use the old.
            delete prop;
            return *it;
        }
    }

    // not found; add the new prop we just created to the list.
    props_.push_front(prop);
    nprops_++;
    verbose(prop->toOStream_long(_STD_ cout));
    return prop;
}

/***************************************************************************
  Building up a clause.
  We store the props in sorted order, which takes O(n^2) where n is the
  number of props.
  This allows us to check clause==list in O(n) time.
  We build c lists ; we check each against c clauses.  So we spend
  O(c*n^2) building, and O(c^2*n) searching.

  If we built the list in O(n) time, we'd spend O(n^2) time searching each,
  for a total of O(cn) building and O(c^2*n^2) searching, so it's faster this
  way.

  If we used a hash table, hash function is sum the pointers, then search
  would be O(n+T(eq)) per lookup, c lookups.  So we'd spend

  Note: n << c
                sorted                unsorted
  building      O(c*n^2)              O(cn)

  list          O(c^2*n)              O(c^2*n^2)
  total(list)   O(c^2*n)              O(c^2*n^2)

  hash          O(cn)                 O(cn^2)
  total(hash)   O(cn^2)               O(cn^2)

  In other words, we have a speedup of O(c) possible if we use a hash table.
 ***************************************************************************/

void from_xmpl::add_clause_prop(L2rProposition *prop) {
    // Insert in sorted order.  Insertion-sort is expensive but the
    // clauses are small.
    Slist<L2rProposition*>::iterator it = clause_props.begin();
    Slist<L2rProposition*>::iterator pit= it; // prev iterator
    while(it != clause_props.end()) {
        if(prop == *it) {
            return; // duplicate; skip this one
        }
        if(prop < *it) {
            break; // pit points to the last iterator smaller than prop
        }
        pit = it;
        ++it;
    }

    // if we reached it==end, pit is last(), so this works in either case
    clause_props.insert(pit, prop);
    nclause_props++;
}

bool from_xmpl::clause_eq_clause_props(const L2rClause *cls) {
    // See if the clause matches.
    // Clauses are in increasing order, so we can check in linear time.
    unsigned cp_size = nclause_props;
    if(cp_size != cls->nprops()) return false;

    // for each element in clause_props, make sure it's in cls->props_
    unsigned i=0;
    Slist<L2rProposition*>::iterator pit=clause_props.begin();
    for( ; i < cp_size ; ++pit, ++i) {
        if(*pit != cls->prop(i))
            return false;
    }

    return true;
}


L2rClause *from_xmpl::find_or_create_clause() {
    // in case we get <clause/> which we actually do
    if(nclause_props==0) return 0;

    // foreach clause
    for(clausesIT it = clauses_.begin(); it != clauses_.end(); ++it) {
        L2rClause *cls = *it;
        // if we find a match, use it
        if(clause_eq_clause_props(cls))
            return cls;
    }

    // if we get here, then the clause represented by clause_props
    // doesn't match any clause in clauses_
    L2rClause *cls = new dbg_L2rClause(component_name, nclauses_++,
            nclause_props,
            list_to_array(clause_props),
            false);
    clauses_.push_front(cls);
    verbose(cls->toOStream_long(_STD_ cout));
    return cls;
}


/***************************************************************************
        Callback functions
 ***************************************************************************/

// All variables are attributes.
void from_xmpl::start_attribute(const MBA_string& name, const MBA_string& type) {
    MBA_string full_name = component_name + "." + name;
    dbg_L2rEnumeration *en = find_enum(type);
    L2rVariable *newvar = create_variable(en, full_name);

    if(name == "mode") {
        modevar_ = newvar;
        modevar_->setKind(vk_mode);
    }
}

// Note a variable is in fact commandable.
// Commandable variables must have the noCommand value in their domain.
// In fact, it must be index 0 (which is ensured by the code that makes the
// enumerations).
void from_xmpl::start_cmd(const MBA_string& varname) {
    L2rVariable *var = find_variable(varname);
    L2_assert(var->type()->hasDebug(), L2_fatal_error,
            ("Enumeration of variable "+varname+
             " has no debugging information"));
    const dbg_L2rEnumeration *en =
        static_cast<const dbg_L2rEnumeration*>(var->type());
    L2_assert(en->name(0) == noCommand, L2_parse_model_error,
            ("Enumeration "+ en->name() + " of commandable variable "+varname+
             "\nshould have the `noCommand' value"));
    var->setKind(vk_commanded);
}

// Note a variable is in fact observable.
void from_xmpl::start_obs(const MBA_string& varname) {
    L2rVariable *var = find_variable(varname);
    var->setKind(vk_observed);
}


// Initial value for a variable.
void from_xmpl::start_assign(const MBA_string& prop) {
    // split the proposition
    int eq_index = prop.find("=");
    L2_assert(eq_index != MBA_string::npos, L2_parse_model_error,
            ("no '=' parsing assign "));
    MBA_string varname(prop.c_str(), eq_index);
    MBA_string valuename(prop.c_str()+eq_index+1); // skip the =

    // find the internal representation
    L2rVariable *var = find_variable(varname);
    int value = find_enum_member(var->type(), valuename);
    var->setInitial(value);
}

// An enumeration; the values are space-separated.
void from_xmpl::start_attributetype(const MBA_string& name, const MBA_string& values) {
    // extract a list of strings from the single values string
    Slist<MBA_string> memnames;
    for(unsigned i=0; i<values.size(); /* iteration is done inside */ ) {
        // skip a word
        unsigned j;
        for(j=i; j<values.size() && values[j]!=' '; j++) ;
        memnames.push_back(MBA_string(values.c_str()+i, j-i));
        i=j;

        // skip whitespace
        for( ; i<values.size() && values[i]==' '; i++) ;
    }
    create_enum(name, memnames);
}

// Start a new component; we just store the name to prepend it.
void from_xmpl::start_component(const MBA_string& name) {
    component_name = name;
}

// We need to know we're parsing a clause.
void from_xmpl::start_clause() {
    parsing_clause = true;
}

// We need to know we're parsing a prop.
void from_xmpl::start_prop() {
    parsing_prop = true;
}

// Currently we only handle a single state (eg mode) variable and it
// must be called mode.
void from_xmpl::start_statevector(const MBA_string& vector) {
    L2_assert(vector=="mode", L2_reader_error,
            ("mode variable must be called `mode', not `"+vector+"'"));
}

void from_xmpl::start_transition(const MBA_string& from, const MBA_string& to,
        const MBA_string& name, const MBA_string& prob) {
    parsing_transition = true;
    transition_name = name;

    transition_from = find_enum_member(modevar_->type(), from);
    transition_to   = find_enum_member(modevar_->type(), to);

    if(prob=="") {
        // probability is only specified for failures,
        // so this must be a nominal transition
        transition_isNominal = true;
        transition_rank = 0;
    }
    else {
        transition_isNominal = false;
        transition_rank = convert_to_rank(prob);
    }
}

void from_xmpl::characters(const MBA_string& prop_str) {
    // ignore whitespace and other junk outside of the <term> tags
    if (!parsing_prop) return;

    // the string comes in the format:
    //     var=value
    // or
    //     !var=value
    // where value may be a member of var->type() or another variable
    bool isPositive;
    L2rVariable *var;

    unsigned i;
    if(prop_str[0] == '!') {
        isPositive = false;
        i = 1;
    }
    else {
        isPositive = true ;
        i = 0;
    }

    for(/*i already set*/ ; i<prop_str.size(); i++) {
        if(prop_str[i]=='=') break;
    }
    MBA_string varname;
    varname = component_name + ".";
    // read up to but not including the '=' ; also, skip '!' (which of course
    // changes the count)
    if(isPositive) varname = varname + MBA_string(prop_str.c_str(), i);
    else           varname = varname + MBA_string(prop_str.c_str()+1, i-1);

    // read starting from right after the '='
    MBA_string valuename(prop_str.c_str()+i+1);

    var = find_variable(varname);

    add_clause_prop(find_or_create_prop(var, isPositive, valuename));
}

void from_xmpl::end_component() {
    // fully-parsed already; unlike in the L2_writer,
    // we really don't care about components.
    component_name = "";

    // just for debugging, really
    modevar_ = 0;
    return;
}

void from_xmpl::end_clause() {
    // we may be creating a new clause here, or maybe not
    L2rClause *cls = find_or_create_clause();

    // this happens in a nominal, idle transition; we get a:
    // <clause/> tag, which has no propositions.  Looks like
    // a dirty hack to me, and I don't need it.
    if(!cls) return;

    // if we're in the background model, stuff the clause there;
    // if we're in a transition, put it there instead
    if(parsing_transition)
        transition_clauses.push_back(cls);
    else
        cls->putInBackground();

    parsing_clause = false;
    clause_props.erase();
    nclause_props = 0;
}

// Propositions are stored in clause_props, which
// is cleaned up in </ci:clause> so nothing left to do here
void from_xmpl::end_prop() {
    parsing_prop = false;
}

// Now we have the transition_clauses, so we can create the transition
void from_xmpl::end_transition() {
    L2rTransition *newtran = new dbg_L2rTransition(
            transition_name,
            modevar_, transition_from, transition_to,
            transition_clauses.size(),
            list_to_array(transition_clauses),false,
            transition_isNominal, transition_rank);
    X_.push_back(newtran);
    verbose(newtran->toOStream_long(_STD_ cout));

    // clear the variables that accumulated the info
    parsing_transition = false;
    transition_clauses.erase();
    // others don't matter; and we shouldn't clear modevar_
}

/***************************************************************************
        The 'mainline'
 ***************************************************************************/

bool try_to_open(const MBA_string& name) {
    _STD_ ifstream try_file(name.c_str());
    if(!try_file) {
        _STD_ cerr << "Error opening `" << name << "'";
#ifndef WIN32
        _STD_ cerr << ": " << strerror(errno);
#endif
        _STD_ cerr << _STD_ endl;
        return false;
    }
    else
        return true;
}

bool from_xmpl::read() {

    // parse the files.  Really easy now!
    XMLPlatformUtils::Initialize();

    HandlerBase_subclass handler (*this);
    SAXParser parser;
    parser.setDocumentHandler(&handler);
    parser.setErrorHandler(&handler);

    L2_TRY {

        // the main model file
        handler.filename = filename_ + ".xmpl";
        verbose(_STD_ cout << "Reading " << handler.filename << _STD_ endl);
        if(!try_to_open(handler.filename)) return false;
        parser.parse( (handler.filename).c_str());

        // the harness file
        handler.filename = filename_ + ".hrn";
        verbose(_STD_ cout << "Reading " << handler.filename << _STD_ endl);
        if(!try_to_open(handler.filename)) return false;
        parser.parse( (handler.filename).c_str());

        // the initial mode file
        handler.filename = filename_ + ".ini";
        verbose(_STD_ cout << "Reading " << handler.filename << _STD_ endl);
        if(!try_to_open(handler.filename)) return false;
        parser.parse( (handler.filename).c_str());
    }
#ifdef ENABLE_EXCEPTIONS
    catch(const XMLException& xcp) {
        L2_throw( L2_reader_error,
                (MBA_string("Error reading `") + handler.filename +"': "
                 + XMLString::transcode(xcp.getMessage()) + "\n"));
    }
#endif

    // Now, the bulk of this function's work: putting it all into
    // the L2_file structure.

    // We set the prop and transition IDs here only, since we have
    // to put them in a particular order.

    {
        // enumerations
        dest()->allocEnums(nenums_, true);
        for(enumsIT eit = enums_.begin(); eit != enums_.end(); ++eit) {
            dbg_L2rEnumeration *en = *eit;
            dest()->setEnum(en->id(), en);
        }
    }

    {
        // variables
        dest()->allocVars(vars_.size(), true);
        for(varsIT vit = vars_.begin(); vit != vars_.end(); ++vit) {
            dbg_L2rVariable *var = *vit;
            dest()->setVar(var->id(), var);
        }
    }

    {
        dest()->allocProps(nprops_);
        for(propsIT pit = props_.begin(); pit != props_.end(); ++pit) {
            L2rProposition *p = *pit;
            dest()->setProp(p->id(), p);
        }
    }


    {
        // clauses
        dest()->allocClauses(nclauses_, true);
        for(clausesIT cit = clauses_.begin(); cit != clauses_.end(); ++cit) {
            L2rClause *cls = *cit;
            dest()->setClause(cls->id(), cls);
        }
    }

    {
        // transitions ; these are just added in, don't even need to allocate
        transitionsIT xit = X_.begin();
        for(xit = X_.begin(); xit != X_.end(); ++xit) {
            dest()->addTransition(*xit);
        }
    }

    // didn't fail so we succeded!
    return true;
}

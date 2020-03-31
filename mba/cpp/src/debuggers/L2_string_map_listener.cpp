/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: L2_string_map_listener.cpp,v 1.26 2002/05/13 18:08:16 lbrown Exp $ */
#include <debuggers/L2_string_map_listener.h>

#include <conflict_db/assignable.h>
#include <conflict_db/variable.h>
#include <debuggers/L2_string_map.h>
#include <readers/clause.h>
#include <readers/L2_file.h>
#include <readers/to_t_system.h>
#include <transition/tracker.h>

L2_string_map_listener::L2_string_map_listener(L2_string_map& themap,
        L2_file& thefile,
        to_t_system& writer,
        Tracker& thetracker)
: map(themap), file(thefile), tracker(thetracker)
{
    // make absolutely sure we invoke the right method with each
    // set_listener call.  Without all the casts, we get an error
    // in setting them for T_system/Conflict_db.  Use static_cast
    // to be compile-time safe.

    writer.add_listener(static_cast<to_t_system_listener*>(this));

    tracker.add_listener(static_cast<Tracker_listener*>(this));

    T_system& t_sys = tracker.get_t_system();
    static_cast<T_system&>(t_sys).add_listener(
            static_cast<T_system_listener*>(this));
    static_cast<Conflict_db&>(t_sys).add_listener(
            static_cast<Conflict_db_listener*>(this));

    Ptheory *tms = t_sys.get_solver();
    static_cast<Ptheory*>(tms)->add_listener(
            static_cast<Ptheory_listener*>(this));


    // also, get the map to initialize its domains
    map.create_domains(file);
}

L2_string_map_listener::~L2_string_map_listener() {
    T_system& t_sys = tracker.get_t_system();

    static_cast<T_system&>(t_sys).remove_listener(
            static_cast<T_system_listener*>(this));
    static_cast<Conflict_db&>(t_sys).remove_listener(
            static_cast<Conflict_db_listener*>(this));
    static_cast<Ptheory*>(t_sys.get_solver())->remove_listener(
            static_cast<Ptheory_listener*>(this));
}


/***************************************************************************
  to_t_system_listener functions
 ***************************************************************************/
void L2_string_map_listener::created_variable(const L2rVariable* read, Variable& var)
{
    map.add_variable(read, var);
}

void L2_string_map_listener::created_clause(const L2rClause*, Clause&)
{
    return;
    /*
    if(!read->has_debug()) return;

    const dbg_L2rClause *dbg_read = (const dbg_L2rClause*)read;
    map.add_clause(&cls, dbg_read->get_component());
    */
}

void L2_string_map_listener::created_clause(const L2rVariable*, Clause&)
{
    return;
    /*
    strstream name_stream;
    read->toOStream(name_stream);
    name_stream << '\0';
    const char *dot_ptr = strrchr(name_stream.str(), '.');
    MBA_string component;
    if(dot_ptr)
        component = MBA_string(name_stream.str(),
                (unsigned)(dot_ptr-name_stream.str()));
    else
        component = name_stream.str();
    map.add_clause(&cls, component);

    name_stream.rdbuf()->freeze(0);
    */
}


void L2_string_map_listener::destroying_container(to_t_system&) {
    // ignore ; we might assert in our dtor that we're gone, but
    // we know we will be
}

/***************************************************************************
  Tracker_listener functions
 ***************************************************************************/

void L2_string_map_listener::destroying_container(Tracker&) {
    L2_throw(L2_fatal_error, ("Listener should be destroyed before the rest"));
}

/***************************************************************************
  T_system_listener functions
 ***************************************************************************/
void L2_string_map_listener::progressed_variable(Variable& /*now_var*/,
        Variable& newly_created_var) {
    map.add_variable(newly_created_var);
}

void L2_string_map_listener::created_state_variable(Assignable& state_var) {
    map.add_variable(state_var);
    map.revise_state_domain(state_var.get_nvalues());
}

void L2_string_map_listener::created_mode_transition(
        Dependent& /*mode*/,
        Assumption& modeTransition) {
    map.add_variable(modeTransition);
}

void L2_string_map_listener::destroying_container(T_system&) {
    L2_throw(L2_fatal_error, ("Listener should be destroyed before the rest"));
}

/***************************************************************************
  Conflict_db_listener functions
 ***************************************************************************/
void L2_string_map_listener::created_variable(Variable&) {
    // ignore ; some other callback will take care of it
}

void L2_string_map_listener::destroying_variable(Variable& var) {
    map.remove_variable(var);
}

void L2_string_map_listener::destroying_container(Conflict_db&) {
    L2_throw(L2_fatal_error, ("Listener should be destroyed before the rest"));
}

/***************************************************************************
  Ptheory_listener functions
 ***************************************************************************/
void L2_string_map_listener::created_proposition(Proposition&) {
    // ignore
}
void L2_string_map_listener::created_clause(Clause&) {
    // ignore
}

void L2_string_map_listener::destroying_proposition(Proposition& prop) {
    map.remove_prop(&prop);
}
void L2_string_map_listener::destroying_clause(Clause&) {
    //map.remove_clause(&cls);
}

void L2_string_map_listener::destroying_container(Ptheory&) {
    L2_throw(L2_fatal_error, ("Listener should be destroyed before the rest"));
}

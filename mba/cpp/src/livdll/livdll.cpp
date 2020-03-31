/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

// * Case split on engine type
// * Wrap all entries with catches
// * Check for exceptions after all callbacks
// * Throw java Errors from each
// * Catch java Errors in the commandline main loops (already done, I think)
// * Put a switch into this s.t. engines which are corrupt can't be used.
// * do view state command

//
// livdll.cpp : Defines the entry point for the DLL application.
//

#ifdef _MSC_VER
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>

#endif

#include <livdll/LivingstoneEngineJNI.h>

#include <debuggers/L2_string_map.h>
#include <debuggers/L2_string_map_listener.h>
#include <debuggers/livingstone_debug.h>
#include <debuggers/tracker_debug.h>
#include <debuggers/trajectory.h>
#include <debuggers/conflict_db_debug.h>
#include <livingstone/L2_assert.h>
#include <mba_utils/slist.h>
#include <readers/L2_file.h>
#include <transition/termination.h>
#include <transition/tracker.h>
#include <api/livingstone.h>


#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <livingstone/L2_fstream.h>

#define CBFS 1
#define COVER 2

int JNI_CBFS_COVER_search = -1;

//
// Helper functions
//

bool jni_prop_contains_string( Conflict_db_debug* debugger,
                               Proposition* prop, const char* substring) {
  return (debugger->get_prop_name( *prop).find( substring) != MBA_string::npos);

}

void jni_print_proposition( Conflict_db_debug* debugger, Proposition* prop) {
  _STD_ cerr << debugger->get_prop_name( *prop) << " ";
  if      (prop->isFalse()) _STD_ cerr << ":FALSE";
  else if (prop->isTrue())  _STD_ cerr << ":TRUE";
  else                      _STD_ cerr << ":UNKNOWN";
  _STD_ cerr << _STD_ endl;
}



/***************************************************************************
        A package for the reader, mapping, debuggers, and engine, all
        in one.
 ***************************************************************************/
static
Livingstone_debug *fetchLivPointer(JNIEnv* env, jobject self) {
    jclass clazz = env->GetObjectClass(self);
    jfieldID fid = env->GetFieldID(clazz, "pointer", "I");
    return (Livingstone_debug*)env->GetIntField(self,fid);
}

static
Tracker *fetchTracker(JNIEnv* env, jobject self) {
    return fetchLivPointer(env,self)->get_tracker();
}
static
Tracker_debug *fetchDebugger(JNIEnv* env, jobject self) {
    return fetchLivPointer(env,self)->get_debugger();
}

static
jint livType(JNIEnv* env, jobject self) {
  jclass clazz = env->GetObjectClass(self);
  jfieldID fid = env->GetFieldID(clazz, "search_tracker_type", "I");
  jint retval = env->GetIntField(self,fid);
  L2_assert(retval==COVER || retval==CBFS, L2_argument_error,
          ("invalid search_tracker_type"));
  return retval;
}


/***************************************************************************
        Throwing a java exception
 ***************************************************************************/

#include <stdarg.h>

#ifdef WIN32
/* Two things: Win32 calls it _vsnprintf; and the 'count' argument
   is not the buffer length, but buffer length -1 leaving room for
   the null terminator */
static int vsnprintf(char *buffer, size_t len,
        const char *format, va_list args) {
    return _vsnprintf(buffer, len-1, format, args);
}
#endif


// declare that gcc should warn about bad or missing arguments
#ifdef __GNUC__
static void throw_error(JNIEnv *env, jclass aclazz, const char *source, const char *format, ...)
__attribute__((format (printf, 4, 5)));
#endif

static void throw_error(JNIEnv *env, jclass aclazz, const char *source, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    char err_str[MBA_MAX_LINE_SIZE];
    vsnprintf(err_str, MBA_MAX_LINE_SIZE, format, args);
    err_str[MBA_MAX_LINE_SIZE-1] = '\0'; // just in case

    // print the error msg to the Skunkworks cmd line
    _STD_ cerr << err_str << _STD_ endl;

    // throw the error msg with a prefix of the source method
    char source_err_str[MBA_MAX_LINE_SIZE];
    char source_format[MBA_MAX_LINE_SIZE];
    sprintf(source_format, source);
    strcat(source_format, format);
    vsnprintf(source_err_str, MBA_MAX_LINE_SIZE, source_format, args);
    source_err_str[MBA_MAX_LINE_SIZE-1] = '\0'; // just in case

    if (aclazz==NULL) {
        _STD_ cerr << err_str << _STD_ endl
		  << "Couldn't find livdll.LivingstoneEngineJNI\n" ;
    } else {
      jmethodID mid = env->GetMethodID(aclazz, "createNativeException",
                                       "(Ljava/lang/String;)Ljava/lang/Throwable;");
      jobject throwable = env->CallStaticObjectMethod(aclazz, mid,
                                                      env->NewStringUTF(source_err_str));
      env->Throw((jthrowable)throwable);
    }
}


//
// Main entry
//

#ifdef WIN32

BOOL APIENTRY DllMain( HANDLE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved) {
  switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
      break;
    }
  return TRUE;
}

#endif


//
// Status
//

JNIEXPORT void JNICALL Java_livdll_LivingstoneEngineJNI_helloWorldJNI
(JNIEnv *, jclass) {
  _STD_ cerr << "Hello JNI World\n";
  _STD_ cerr << "Version w/ try-catch\n";
}

//
// Engine allocation / deallocation
//

JNIEXPORT jint JNICALL Java_livdll_LivingstoneEngineJNI_setCBFSEngineParamsJNI
  (JNIEnv *env,
   jclass cls,
   jint max_candidate_classes,
   jint max_candidates,
   jint max_search,
   jint max_cutoff_weight,
   jint max_history,
   jint max_truncated_candidates,
   jstring progress_cmd_type,
   jstring fc_cmd_type)
{
    L2_TRY {
        jboolean b1, b2;
        MBA_string progress_cmd_type_str, fc_cmd_type_str;
        const char *str1 = env->GetStringUTFChars(progress_cmd_type, &b1);
        progress_cmd_type_str = str1;
        if (b1 == JNI_TRUE)
            env->ReleaseStringUTFChars( progress_cmd_type, str1);
        const char *str2 = env->GetStringUTFChars(fc_cmd_type , &b2);
        fc_cmd_type_str = str2;
        if (b2 == JNI_TRUE)
            env->ReleaseStringUTFChars( fc_cmd_type, str2);

        _STD_ cerr << "Instantiating Livingstone $Revision: 1.133 $"
		  << _STD_ endl;

        Livingstone_debug *pack = new Livingstone_debug;

        // we must store args into Livingstone_debug slots for
        // create_cbfs_tracker to access
        pack->set_search_style(Tracker::cbfs);
        pack->set_cbfs_max_candidate_classes( (unsigned) max_candidate_classes);
        pack->set_cbfs_maxcand( (unsigned) max_candidates);
        pack->set_cbfs_search( (unsigned) max_search);
        pack->set_cbfs_max_cutoff_weight( (unsigned) max_cutoff_weight);
        pack->set_history( (unsigned) max_history);
        pack->set_max_truncated_candidates((unsigned) max_truncated_candidates);
        pack->set_progress_style_str(progress_cmd_type_str);
        pack->set_fc_style_str( fc_cmd_type_str);
        // debugger must exist for set_fc_style

        _STD_ cerr << "set CBFS engine params" << _STD_ endl;
        return (jint) pack;

    }
#ifdef ENABLE_EXCEPTIONS
    catch (L2_error lobj) {
        throw_error(env, cls, "setCBFSEngineParamsJNI: ", "%s", lobj.error_string.c_str());
        return (jint) 0;
    } catch (...) {
        _STD_ cerr << "setCBFSEngineParamsJNI: uncaught error\n";
        return (jint) 0;
    }
#endif
}


JNIEXPORT jint JNICALL Java_livdll_LivingstoneEngineJNI_allocateCBFSEngineJNI
  (JNIEnv *env, jclass cls, jint engine_ptr)
{
    L2_TRY {
        Livingstone_debug *pack = (Livingstone_debug*)engine_ptr;

        if(pack->get_parameters()->get_search_style() != Tracker::cbfs) {
            throw_error(env, cls, "allocateCBFSEngineJNI: ",
                    "params disagree with request to create CBFS engine");
        }

        pack->create_tracker();

        if (! pack->initialize_tracker()) {
          throw_error(env, cls, "allocateCBFSEngineJNI: ", "error initializing tracker from %s",
                      pack->get_filename().c_str());
          return (jint) 0;
        }

        _STD_ cerr << "allocated a CBFS engine: " << (jint) pack << _STD_ endl;

        return (jint) pack;

    }
#ifdef ENABLE_EXCEPTIONS
    catch (L2_error lobj) {
        throw_error(env, cls, "allocateCBFSEngineJNI: ", "%s", lobj.error_string.c_str());
        return (jint) 0;
    } catch (...) {
        _STD_ cerr << "allocateCBFSEngineJNI: uncaught error\n";
        return (jint) 0;
    }
#endif
}

JNIEXPORT jint JNICALL Java_livdll_LivingstoneEngineJNI_allocateCBFSDebuggerJNI
  (JNIEnv *env, jclass cls, jint engine_ptr)
{
    L2_TRY {
        Livingstone_debug *pack = (Livingstone_debug*)engine_ptr;
        pack->create_debugger();
        _STD_ cerr << "allocated a CBFS debugger: " <<
          (jint)pack->get_debugger() << _STD_ endl << _STD_ endl;

        // get search param
        MBA_string fc_cmd_type_str = pack->get_fc_style_str();
        // debugger must exist for set_fc_style
        Tracker_debug *debug = pack->get_debugger();
        Tracker::FC_style fc_style = Tracker_debug::find_style( fc_cmd_type_str);
        debug->get_tracker()->set_fc_style( fc_style);

        // do this here rather than in allocateCBFSTracker, so that
        // fc_style will be correct
        pack->print_search_parameters();

           return (jint) pack->get_debugger();
    }
#ifdef ENABLE_EXCEPTIONS
    catch (L2_error lobj) {
        throw_error(env, cls, "allocateCBFSDebuggerJNI: ", "%s", lobj.error_string.c_str());
        return (jint) 0;
    } catch (...) {
        _STD_ cerr << "allocateCBFSDebuggerJNI: uncaught error\n";
        return (jint) 0;
    }
#endif
}


JNIEXPORT jint JNICALL Java_livdll_LivingstoneEngineJNI_setCoverEngineParamsJNI
  (JNIEnv *env,
   jclass cls,
   jint max_rank,
   jint max_history,
   jint max_truncated_candidates,
   jstring progress_cmd_type,
   jstring fc_cmd_type
   )
{
    L2_TRY {
        jboolean b1, b2;
        MBA_string progress_cmd_type_str, fc_cmd_type_str;
        const char *str1 = env->GetStringUTFChars(progress_cmd_type, &b1);
        progress_cmd_type_str = str1;
        if (b1 == JNI_TRUE)
            env->ReleaseStringUTFChars( progress_cmd_type, str1);
        const char *str2 = env->GetStringUTFChars(fc_cmd_type , &b2);
        fc_cmd_type_str = str2;
        if (b2 == JNI_TRUE)
            env->ReleaseStringUTFChars( fc_cmd_type, str2);

        _STD_ cerr<< "Instantiating Livingstone $Revision: 1.133 $"
		 << _STD_ endl;

        Livingstone_debug *pack = new Livingstone_debug;

        // we must store args into Livingstone_debug slots for
        // create_cbfs_tracker to access
        pack->set_search_style(Tracker::cover);
        pack->set_cover_maxrank( (unsigned) max_rank);
        pack->set_history( (unsigned) max_history);
        pack->set_max_truncated_candidates((unsigned) max_truncated_candidates);
        pack->set_progress_style(L2_parameters
                ::get_progress_style(progress_cmd_type_str));
        pack->set_fc_style_str( fc_cmd_type_str);
        // debugger must exist for set_fc_style

        _STD_ cerr << "set Cover engine params" << _STD_ endl;
        return (jint) pack;
    }
#ifdef ENABLE_EXCEPTIONS
    catch (L2_error lobj) {
        throw_error(env, cls, "setCoverEngineParamsJNI: ", "%s", lobj.error_string.c_str());
        return (jint) 0;
    } catch (...) {
        _STD_ cerr << "setCoverEngineParamsJNI: uncaught error\n";
        return (jint) 0;
    }
#endif
}


JNIEXPORT jint JNICALL Java_livdll_LivingstoneEngineJNI_allocateCoverEngineJNI
  (JNIEnv *env, jclass cls, jint engine_ptr)
{
    L2_TRY {
        Livingstone_debug *pack = (Livingstone_debug*)engine_ptr;

        if(pack->get_parameters()->get_search_style() != Tracker::cover) {
            throw_error(env, cls, "allocateCoverEngineJNI: ",
                    "params disagree with request to create Cover engine");
        }
        pack->create_tracker();

        if (! pack->initialize_tracker()) {
            throw_error(env, cls, "allocateCoverEngineJNI: ",
                    "error initializing tracker from %s",
                    pack->get_filename().c_str());
            return (jint) 0;
        }

        _STD_ cerr << "allocated a Cover engine: " << (jint) pack << _STD_ endl;
        return (jint) pack;
    }
#ifdef ENABLE_EXCEPTIONS
    catch (L2_error lobj) {
        throw_error(env, cls, "allocateCoverEngineJNI: ", "%s", lobj.error_string.c_str());
        return (jint) 0;
    } catch (...) {
        _STD_ cerr << "allocateCoverEngineJNI: uncaught error\n";
        return (jint) 0;
    }
#endif
}



JNIEXPORT jint JNICALL Java_livdll_LivingstoneEngineJNI_allocateCoverDebuggerJNI
  (JNIEnv *env, jclass cls, jint engine_ptr)
{
     L2_TRY {
        Livingstone_debug *pack = (Livingstone_debug*)engine_ptr;
        pack->create_debugger();
        _STD_ cerr << "allocated a Cover debugger: " <<
          (jint)pack->get_debugger() << _STD_ endl << _STD_ endl;

        // get search param
        MBA_string fc_cmd_type_str = pack->get_fc_style_str();
        // debugger must exist for set_fc_style
        Tracker_debug *debug = pack->get_debugger();
        Tracker::FC_style fc_style = Tracker_debug::find_style( fc_cmd_type_str);
        debug->get_tracker()->set_fc_style( fc_style);

        // do this here rather than in allocateCBFSTracker, so that
        // fc_style will be correct
        pack->print_search_parameters();

        return (jint) pack->get_debugger();
    }
#ifdef ENABLE_EXCEPTIONS
     catch (L2_error lobj) {
        throw_error(env, cls, "allocateCoverDebuggerJNI: ", "%s", lobj.error_string.c_str());
        return 0;
    } catch (...) {
        _STD_ cerr << "allocateCoverDebuggerJNI: uncaught error\n";
        return 0;
    }
#endif
}


JNIEXPORT jint JNICALL Java_livdll_LivingstoneEngineJNI_deallocateEngineJNI
  (JNIEnv *env, jobject self, jint tracker_addr, jint debugger_addr)
{
    // use MonitorEnter/MonitorExit to require synchronized execution
    env->MonitorEnter( self);
    L2_TRY {
        Livingstone_debug *pack = (Livingstone_debug*)tracker_addr;
        delete pack;
    }
#ifdef ENABLE_EXCEPTIONS
    catch (...) {
        _STD_ cerr << "Java_livdll_LivingstoneEngineJNI_deallocateEngineJNI: uncaught error\n";
    }
#endif
    env->MonitorExit( self);
    return (jint)0;
}

//
// Engine operations
//


// returns
//         "cbfs": allocate new cbfs engine, using L2 search params
//         "cover": allocate new cover engine, using L2 search params
JNIEXPORT jstring JNICALL Java_livdll_LivingstoneEngineJNI_loadModelJNI
(JNIEnv* env, jclass cls, jstring model_name, jint engine_ptr)
{
  L2_TRY {
    MBA_string model_name_str;

    // _STD_ cerr << "In Java_livdll_LivingstoneEngineJNI_loadModelJNI" << _STD_ endl;

    // Get the string, store it in our ref-counted string.
    jboolean b;
    const char *str = env->GetStringUTFChars(model_name, &b);
    model_name_str = str;
    if(b == JNI_TRUE)
      env->ReleaseStringUTFChars(model_name, str);

    Livingstone_debug *pack = (Livingstone_debug*)engine_ptr;
    pack->set_filename(model_name_str);
    pack->set_filetype("xmpl");

    _STD_ cerr << "Reading model file(s): "
	 <<  model_name_str + "(.xmpl .ini .hrn)"
	 << _STD_ endl;

    if (!pack->read_file()) {
        throw_error(env, cls, "loadModelJNI: ", "error reading %s",
		    model_name_str.c_str());
        return env->NewStringUTF( "error");
    }

#ifndef DISABLE_OPTIMIZER
    if (pack->get_use_optimizer()) {
        Livingstone_reader& reader = pack->getReader();
        bool old_verbosity = reader.verbose_optimizer();
        reader.set_verbose_optimizer(true);
        _STD_ ostream& old_stream = reader.get_output();
        MBA_string output = model_name_str + ".xmpl-opt";
        _STD_ ofstream outfile(output.c_str());
        reader.set_output(outfile);

	_STD_ cerr << "Optimizing model ..." << _STD_ endl;
        if (!reader.optimize()) {
            throw_error(env, cls, "loadModelJNI: ", "error optimizing %s",
                    model_name_str.c_str());
            return env->NewStringUTF("error");
        }

        reader.set_output(old_stream);
        reader.set_verbose_optimizer(old_verbosity);
    }
#endif

    // _STD_ cerr << "Model `" << model_name_str << "' successfully loaded\n";

    return env->NewStringUTF( pack->get_search_style_str().c_str());

  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
    throw_error(env, cls, "loadModelJNI: ", "%s", lobj.error_string.c_str());
    return env->NewStringUTF( "error");
  } catch (...) {
    _STD_ cerr << "loadModelJNI: uncaught error\n" << _STD_ endl;
    return env->NewStringUTF( "error");
  }
#endif
}

JNIEXPORT jint JNICALL Java_livdll_LivingstoneEngineJNI_propagateJNI
(JNIEnv *env, jobject self) {
  L2_TRY {
    return (jint) fetchTracker(env,self)->
        get_t_system().get_solver()->propagate();
  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
    throw_error(env, env->GetObjectClass(self), "propagateJNI: ", "%s",
                lobj.error_string.c_str());
    return 0;
  } catch (...) {
    _STD_ cerr << "propagateJNI: uncaught error\n";
    return 0;
  }
#endif
}

JNIEXPORT void JNICALL Java_livdll_LivingstoneEngineJNI_minProgressJNI
  (JNIEnv *env, jobject self, jstring str) {
  const char* argstring;
  jboolean isCopy;

  L2_TRY {
    argstring = env->GetStringUTFChars(str, &isCopy);
    char arg[MBA_MAX_LINE_SIZE];
    strncpy(arg, argstring, MBA_MAX_LINE_SIZE);
    Tracker_debug * debugger = fetchDebugger(env,self);
    if (fetchTracker(env,self)->get_t_system().get_progress_uses_full())
      debugger->parse_cmd( "progress-style", "min");
    debugger->parse_cmd("progress", arg);
    if (isCopy == JNI_TRUE) {
      env->ReleaseStringUTFChars(str, argstring);
    }
    return;
  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
    throw_error(env, env->GetObjectClass(self), "minProgressJNI: ", "%s",
                lobj.error_string.c_str());
    return;
  } catch (...) {
    _STD_ cerr << "minProgressJNI: uncaught error\n";
    return;
  }
#endif
}

JNIEXPORT void JNICALL Java_livdll_LivingstoneEngineJNI_fullProgressJNI
  (JNIEnv *env, jobject self, jstring str) {
    const char* argstring;
    jboolean isCopy;

  L2_TRY {
    argstring = env->GetStringUTFChars(str, &isCopy);
    char arg[MBA_MAX_LINE_SIZE];
    strncpy(arg, argstring, MBA_MAX_LINE_SIZE);
    Tracker_debug * debugger = fetchDebugger(env,self);
    if (! fetchTracker(env,self)->get_t_system().get_progress_uses_full())
      debugger->parse_cmd( "progress-style", "full");
    debugger->parse_cmd("progress", arg);
    if (isCopy == JNI_TRUE) {
      env->ReleaseStringUTFChars(str, argstring);
    }
    return;
  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
    throw_error(env, env->GetObjectClass(self), "fullProgressJNI: ", "%s",
                lobj.error_string.c_str());
    return;
  } catch (...) {
    _STD_ cerr << "fullProgressJNI: uncaught error\n";
    return;
  }
#endif
}

JNIEXPORT void JNICALL Java_livdll_LivingstoneEngineJNI_assignMultipleJNI
(JNIEnv *env, jobject self, jstring monitor_jstring) {
  L2_TRY {
    jboolean monitor_isCopy;
    const char* monitor_str = env->GetStringUTFChars(monitor_jstring, &monitor_isCopy);
    // debugger->assign mangles its arg
    // make sure theString is large enough
    int size_alloc = MBA_MAX_LINE_SIZE;
    char *theString = (char*)malloc(size_alloc);
    int monitor_str_len = strlen( monitor_str) + 1;
    if (monitor_str_len > size_alloc ) {
      size_alloc *= 1 + monitor_str_len / MBA_MAX_LINE_SIZE;
      theString = (char*)realloc(theString, size_alloc);
    }
    strncpy(theString, monitor_str, size_alloc);
    fetchDebugger(env,self)->assign( theString);
    free( theString);
    if (monitor_isCopy == JNI_TRUE) env->ReleaseStringUTFChars(monitor_jstring,
                                                               monitor_str);
  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
      throw_error(env, env->GetObjectClass(self), "assignMultipleJNI: ", "%s",
                  lobj.error_string.c_str());
  } catch (...) {
    _STD_ cerr << "assignMultipleJNI: uncaught error\n";
    return;
  }
#endif
}

JNIEXPORT void JNICALL Java_livdll_LivingstoneEngineJNI_unassignJNI
(JNIEnv *env, jobject self, jstring var_jstring) {
  L2_TRY {
    jboolean var_isCopy;
    const char* var_str = env->GetStringUTFChars(var_jstring, &var_isCopy);
    Variable *var = fetchDebugger(env,self)->find_variable(var_str);
    char err_str[MBA_MAX_LINE_SIZE];

    if (var==NULL) {
        sprintf(err_str, "No such variable `%s' in assignment", var_str);
        throw_error(env, env->GetObjectClass(self), "unassignJNI: ", err_str);
    } else if (!var->is_assignable()) {
        sprintf(err_str, "Variable `%s' is not assignable", var_str);
        throw_error(env, env->GetObjectClass(self), "unassignJNI: ", err_str);
    } else {
        ((Assignable*) var)->unassign();
    }

    if (var_isCopy == JNI_TRUE) env->ReleaseStringUTFChars(var_jstring,var_str);
  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
      throw_error(env, env->GetObjectClass(self), "unassignJNI: ", "%s",
                  lobj.error_string.c_str());
  } catch (...) {
    _STD_ cerr << "unassignJNI: uncaught error\n";
    return;
  }
#endif
}

JNIEXPORT jint JNICALL Java_livdll_LivingstoneEngineJNI_getPropValueJNI
(JNIEnv *env, jobject self, jstring propname) {
  L2_TRY {
    int result = 0;
    jboolean isCopy;
    const char* propname_str = env->GetStringUTFChars(propname, &isCopy);
    char err_str[MBA_MAX_LINE_SIZE];

    MBA_string propname_mba(propname_str);
    Proposition *prop = fetchDebugger(env,self)->find_proposition(propname_mba);

    if (prop==NULL) {
        sprintf(err_str, "No such proposition `%s'",  propname_str);
        throw_error(env, env->GetObjectClass(self), "getPropValueJNI: ", err_str);
        result = 0;
    }
    else if (prop->isTrue())  { result = 1; }
    else if (prop->isFalse()) { result = 2; }
    else                      { result = 3; }

    // Clean up
    if (isCopy == JNI_TRUE) env->ReleaseStringUTFChars(propname, propname_str);

    return result;
  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
      throw_error(env, env->GetObjectClass(self), "getPropValueJNI: ", "%s",
                  lobj.error_string.c_str());
    return 0;
  } catch (...) {
    _STD_ cerr << "getPropValueJNI: uncaught error\n";
    return 0;
  }
#endif
}

//
// Candidates and Assignments
//

JNIEXPORT void JNICALL Java_livdll_LivingstoneEngineJNI_installCandidateJNI
(JNIEnv *env, jobject self, jint candidate_num) {
  L2_TRY {
    fetchTracker(env,self)->get_t_system().install(candidate_num);
  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
    throw_error(env, env->GetObjectClass(self), "installCandidateJNI: ", "%s",
                lobj.error_string.c_str());
    return;
  } catch (...) {
    _STD_ cerr << "installCandidateJNI: uncaught error\n";
    return;
  }
#endif
}

JNIEXPORT jstring JNICALL Java_livdll_LivingstoneEngineJNI_getFcStyleJNI
(JNIEnv *env, jobject self) {
  L2_TRY {
    Livingstone_debug* pack = fetchLivPointer(env, self);
    return env->NewStringUTF( pack->get_fc_style_str().c_str());
  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
    throw_error(env, env->GetObjectClass(self), "getFcStyleJNI: ", "%s",
                lobj.error_string.c_str());
    return env->NewStringUTF( "error");
  } catch (...) {
    _STD_ cerr << "getFcStyleJNI: uncaught error\n" << _STD_ endl;
    return env->NewStringUTF( "error");
  }
#endif
}

JNIEXPORT void JNICALL Java_livdll_LivingstoneEngineJNI_pruneAndSearchJNI
(JNIEnv *env, jobject self) {
  L2_TRY {
    Tracker* tracker = fetchTracker(env,self);
    Livingstone_debug* pack = fetchLivPointer(env, self);
    Tracker_debug* debug = pack->get_debugger();

    // tracker->prune_and_search();
    // since src/transition/tracker.cpp does not handle this properly
    // do it here
    tracker->get_t_system().prune_inconsistent_candidates();
    if (tracker->get_t_system().getCandidates().empty()) {
      tracker->find_fresh_candidates();
      JNI_CBFS_COVER_search = 1;
      // _STD_ cerr << "pruneAndSearchJNI: find_fresh_candidates \n";
    } else {
      JNI_CBFS_COVER_search = 0;
    }

    if (tracker->get_t_system().getCandidates().empty())
      _STD_ cerr << "No candidates can be found.\n";
    else {
      _STD_ cerr << "Consistent candidates are:\n" ;
      debug->print_candidates();
    }

  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
    throw_error(env, env->GetObjectClass(self), "pruneAndSearchJNI: ", "%s",
                lobj.error_string.c_str());
    return;
  } catch (...) {
    _STD_ cerr << "pruneAndSearchJNI: uncaught error\n";
    return;
  }
#endif
}

JNIEXPORT void JNICALL Java_livdll_LivingstoneEngineJNI_findFreshJNI
(JNIEnv *env, jobject self) {
  L2_TRY {
    Tracker* tracker = fetchTracker(env,self);
    Livingstone_debug* pack = fetchLivPointer(env, self);
    Tracker_debug* debug = pack->get_debugger();

    tracker->find_fresh_candidates();
    JNI_CBFS_COVER_search = 1;

    if (tracker->get_t_system().getCandidates().empty())
      _STD_ cerr << "No candidates can be found.\n";
    else {
      _STD_ cerr << "Consistent candidates are:\n" ;
      debug->print_candidates();
    }

  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
    throw_error(env, env->GetObjectClass(self), "findFreshJNI: ", "%s",
                lobj.error_string.c_str());
    return;
  } catch (...) {
    _STD_ cerr << "findFreshJNI: uncaught error\n";
    return;
  }
#endif
}

JNIEXPORT void JNICALL Java_livdll_LivingstoneEngineJNI_extendJNI
(JNIEnv *env, jobject self) {
  L2_TRY {
    Tracker* tracker = fetchTracker(env,self);
    Livingstone_debug* pack = fetchLivPointer(env, self);
    Tracker_debug* debug = pack->get_debugger();
    tracker->extend_candidates();
    JNI_CBFS_COVER_search = 1;

    if (tracker->get_t_system().getCandidates().empty())
      _STD_ cerr << "No candidates can be found.\n";
    else {
      _STD_ cerr << "Consistent candidates are:\n" ;
      debug->print_candidates();
    }

  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
    throw_error(env, env->GetObjectClass(self), "extendJNI: ", "%s",
                lobj.error_string.c_str());
    return;
  } catch (...) {
    _STD_ cerr << "extendJNI: uncaught error\n";
    return;
  }
#endif
}

JNIEXPORT jstring JNICALL Java_livdll_LivingstoneEngineJNI_getSearchTerminationJNI
  (JNIEnv *env, jobject self) {
  L2_TRY {
      Livingstone_debug *pack = fetchLivPointer(env, self);

      if (CBFS==livType(env,self)) {
#ifdef DISABLE_CBFS
          throw L2_error("cbfs was disabled at livdll compile-time");
#else
          MBA_string reason;
          if (JNI_CBFS_COVER_search != 1) {
              // no search actually happened
              reason = "CBFS: no search, remaining consistent candidates";
          } else {
              Tracker_debug *debug = pack->get_debugger();
              reason = debug->search_termination();
          }
          _STD_ cerr << reason << _STD_ endl;
          return env->NewStringUTF(reason.c_str());
#endif
      }

    else {
#ifdef DISABLE_COVER
        throw L2_error("cover was disabled at livdll compile-time");
#else
      // COVER
      char error_string[100];
      Tracker* tracker = pack->get_tracker();
      error_string[0] = '\0';
      if (JNI_CBFS_COVER_search == 1) {
        // not defined yet
        if (tracker->get_t_system().getCandidates().empty())
          strcat( error_string, "COVER: search found no candidates");
        else
          strcat( error_string, "COVER: search candidates complete");
      } else {
        strcpy( error_string, "COVER: no search, remaining consistent candidates");
      }
      _STD_ cerr << error_string << _STD_ endl;
      return env->NewStringUTF(error_string);
#endif
    }

  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
    throw_error(env, env->GetObjectClass(self), "getSearchTerminationJNI: ", "%s",
                lobj.error_string.c_str());
    return env->NewStringUTF("");
  } catch (...) {
    _STD_ cerr << "getSearchTerminationJNI: uncaught error\n";
    return env->NewStringUTF("");
  }
#endif
}

JNIEXPORT jint JNICALL Java_livdll_LivingstoneEngineJNI_getCandidateCountJNI
(JNIEnv *env, jobject self) {
  L2_TRY {
      return fetchTracker(env,self)->get_t_system().getCandidates().size();
  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
    throw_error(env, env->GetObjectClass(self), "getCandidateCountJNI: ", "%s",
                lobj.error_string.c_str());
    return 0;
  } catch (...) {
    _STD_ cerr << "getCandidateCountJNI: uncaught error\n";
    return 0;
  }
#endif
}

JNIEXPORT jint JNICALL Java_livdll_LivingstoneEngineJNI_getCandidatePointerJNI
(JNIEnv *env, jobject self, jint index) {
  L2_TRY {
      Tracker *tracker = fetchTracker(env, self);
      Tracker::iterator it = tracker->get_t_system().getCandidates().begin();
      Tracker::iterator end = tracker->get_t_system().getCandidates().end();

      for(int count=0; it!=end; ++it) {
          if (count==index) return (jint)*it;
          count++;
      }
      return 0;
  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
    throw_error(env, env->GetObjectClass(self), "getCandidatePointerJNI: ", "%s",
                lobj.error_string.c_str());
    return 0;
  } catch (...) {
    _STD_ cerr << "getCandidatePointerJNI: uncaught error\n";
    return 0;
  }
#endif
}

JNIEXPORT void JNICALL Java_livdll_LivingstoneEngineJNI_mapOverCandidateAssignmentsJNI
  (JNIEnv *env, jobject self, jint cpointer, jobject callback) {
  // _STD_ cerr << "In mapOverCandidateAssignmentsJNI" << _STD_ endl;
  L2_TRY {
    jclass aclazz = env->GetObjectClass(self);
    if (aclazz==NULL) {
      _STD_ cerr << "Couldn't find livdll.LivingstoneEngineJNI\n";
      return;
    }
    jmethodID aconstructor = env->GetMethodID(aclazz, "createCandidateAssignment",
                                              "(Ljava/lang/String;Ljava/lang/String;I)Ljava/lang/Object;");
    if (aconstructor==NULL) {
      _STD_ cerr << "Couldn't find livdll.LivingstoneEngineJNI.createCandidateAssignment()\n";
      return;
    }

    jclass cclazz = env->GetObjectClass(callback);
    jmethodID mid = env->GetMethodID(cclazz, "update",
                                     "(Ljava/util/Observable;Ljava/lang/Object;)V");
    if (mid==NULL) {
      _STD_ cerr << "Couldn't find the callback's update method\n";
      return;
    }

    jthrowable exc;
    Livingstone_debug* pack = fetchLivPointer(env, self);
    T_system_debug* debugger = pack->get_debugger();
    Tracker *tracker = fetchTracker(env, self);

    Candidate* cand = ((Candidate*)cpointer);
    // int size = cand->size();
    // _STD_ cerr << "In mapOverCandidateAssignmentsJNI: " << size << _STD_ endl;
    Slist<Trajectory::Parts> candidate_parts_ptr;
    Array<MBA_string, true> variableNames;
    Array<MBA_string, true> valueNames;
    debugger->candidate_details( *cand, candidate_parts_ptr,
				 variableNames, valueNames);
    Slist_iterator<Trajectory::Parts> it = candidate_parts_ptr.begin();
    bool zero_cost_candidate_p = true;
    for(unsigned i = candidate_parts_ptr.size() - 1;
	it != candidate_parts_ptr.end(); ++it, --i) {
      zero_cost_candidate_p = false;
      Trajectory::Parts c_part = *it;
      const char* name = variableNames[i].c_str();
      const char* val  = valueNames[i].c_str();
      Assignment::weight_t weight = c_part.get_rank();
      // _STD_ cerr << "val " << val << " name "   << name << " weight " << weight << _STD_ endl;
      jobject newobj = env->CallStaticObjectMethod(aclazz, aconstructor, env->NewStringUTF(name),
		       env->NewStringUTF(val), weight);
      // The actual callback
      env->CallVoidMethod(callback, mid, NULL, newobj);
      // Error handling
      exc = env->ExceptionOccurred();
      if (exc) {
	env->ExceptionDescribe();
	env->ExceptionClear();
	break;
      }
      // I don't think I have to deallocate anything.
    }
    if (zero_cost_candidate_p) {
      unsigned current_time = tracker->get_t_system().get_time_step();
      char time_string[10];
      sprintf( time_string, "%d", current_time);
      strcat( time_string, "#");
      jobject newobj = env->CallStaticObjectMethod(aclazz, aconstructor,
                                      env->NewStringUTF( time_string ),
                                      env->NewStringUTF(""),
				      static_cast<Assignment::weight_t>(0));
      // The actual callback
      env->CallVoidMethod(callback, mid, NULL, newobj);
      // Error handling
      exc = env->ExceptionOccurred();
      if (exc) {
	env->ExceptionDescribe();
	env->ExceptionClear();
      }
    }
    return;

  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
    throw_error(env, env->GetObjectClass(self), "mapOverCandidateAssignmentsJNI: ",
                "%s", lobj.error_string.c_str());
    return;
  } catch (...) {
    _STD_ cerr << "mapOverCandidateAssignmentsJNI: uncaught error\n";
    return;
  }
#endif
}

JNIEXPORT jint JNICALL Java_livdll_LivingstoneEngineJNI_getCandidateClassCountJNI
(JNIEnv *env, jobject self) {
  L2_TRY {
    Tracker *tracker = fetchTracker(env, self);
    // Force Partition of the current Candidates
    tracker->partition();
    // _STD_ cerr << " getCandidateClassCountJNI " << tracker->getCandidatePartition().size() << _STD_ endl;
    return tracker->getCandidatePartition().size();
  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
    throw_error(env, env->GetObjectClass(self), "getCandidateClassCountJNI: ",
                "%s", lobj.error_string.c_str());
    return 0;
  } catch (...) {
    _STD_ cerr << "getCandidateClassCountJNI: uncaught error\n";
    return 0;
  }
#endif
}

JNIEXPORT jint JNICALL Java_livdll_LivingstoneEngineJNI_getCandidateClassPointerJNI
(JNIEnv *env, jobject self, jint index) {
  L2_TRY {
    const Array<CandidateEqClass*, false>& candidatePartition =
      fetchTracker(env,self)->getCandidatePartition();
    for (unsigned i = 0; i < candidatePartition.size(); i++) {
      const CandidateEqClass* const candidateEqClass = candidatePartition[i];
      // _STD_ cerr << " getCandidateClassPointerJNI Equivalence Class " << i << _STD_ endl;
      if (i == (unsigned)index) {
        return (jint) candidateEqClass->get(0);
      }
    }
    return 0;
  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
    throw_error(env, env->GetObjectClass(self), "getCandidateClassPointerJNI: ",
                "%s", lobj.error_string.c_str());
    return 0;
  } catch (...) {
    _STD_ cerr << "getCandidateClassPointerJNI: uncaught error\n";
    return 0;
  }
#endif
}

JNIEXPORT void JNICALL Java_livdll_LivingstoneEngineJNI_mapOverAttributeAssignmentsJNI
(JNIEnv *env, jobject self, jobject callback) {
  L2_TRY {
      Livingstone_debug *pack = fetchLivPointer(env,self);
      Tracker* tracker = pack->get_tracker();
      Tracker_debug *debugger = pack->get_debugger();

    jclass aclazz = env->GetObjectClass(self);
    if (aclazz==NULL) {
      _STD_ cerr << "Couldn't find livdll.LivingstoneEngineJNI\n";
      return;
    }
    jmethodID aconstructor = env->GetMethodID(aclazz, "createAttributeAssignment",
                                              "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/Object;");
    jmethodID aconstructor2 = env->GetMethodID(aclazz, "createAttributeAssignment",
					       "(Ljava/lang/String;Ljava/lang/String;I)Ljava/lang/Object;");
    if (aconstructor==NULL) {
      _STD_ cerr << "Couldn't find livdll.LivingstoneEngineJNI.createAttributeAssignment()\n";
      return;
    }
    if (aconstructor2==NULL) {
      _STD_ cerr << "Couldn't find livdll.LivingstoneEngineJNI.createAttributeAssignment()\n";
      return;
    }

    jclass cclazz = env->GetObjectClass(callback);
    jmethodID mid = env->GetMethodID(cclazz, "update",
                                     "(Ljava/util/Observable;Ljava/lang/Object;)V");
    if (mid==NULL) {
      _STD_ cerr << "Couldn't find the callback's update method\n";
      return;
    }

    jthrowable exc;

    // Conflict_db& db = tracker->get_t_system(); // LSB 18 SEP 2001

    unsigned current_time = tracker->get_t_system().get_time_step();

    pack->collectAllVariableNames(); // LSB 18 SEP 2001
    // for (Variable::ListOfVariable::iterator it = db.begin_variables();
    //      it != db.end_variables(); ++it) {
    for (Array<const MBA_string, true>::iterator it =
	   pack->beginAllVariableNames();
	 it != pack->endAllVariableNames(); ++it) {
      // Variable *var = *it;
      // MBA_string name = debugger->get_var_name(*var);
      const MBA_string name = *it;
      const Variable *const var = pack->getRepresentative(name);
        const MBA_string val = debugger->get_variable_value(*var);

        jobject newobj;
        if(var->get_time_step() > current_time) {
            // for NOW and Start.State, use current_time
            newobj = env->CallStaticObjectMethod(aclazz,aconstructor2,
                    env->NewStringUTF(name.c_str()),
                    env->NewStringUTF(val.c_str()),
                    current_time);
        } else {
            // for everything else, use the time of the variable ?
            newobj = env->CallStaticObjectMethod(aclazz,aconstructor,
                    env->NewStringUTF(name.c_str()),
                    env->NewStringUTF(val.c_str()));
        }

        // The actual callback
        env->CallVoidMethod(callback, mid, NULL, newobj);

        // Error handling
        exc = env->ExceptionOccurred();
        if (exc) {
            env->ExceptionDescribe();
            env->ExceptionClear();
            break;
        }
    }

    return;
  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
    throw_error(env, env->GetObjectClass(self), "mapOverAttributeAssignmentsJNI: ",
                "%s", lobj.error_string.c_str());
    return;
  } catch (...) {
    _STD_ cerr << "mapOverAttributeAssignmentsJNI: uncaught error\n";
    return;
  }
#endif
}

JNIEXPORT void JNICALL Java_livdll_LivingstoneEngineJNI_mapOverAttributeNamesJNI
(JNIEnv *env, jobject self, jobject callback) {
  L2_TRY {
      Livingstone_debug *pack = fetchLivPointer(env,self);
      Tracker* tracker = pack->get_tracker();
      Tracker_debug *debugger = pack->get_debugger();

    //jclass aclazz = env->FindClass("livdll.LivingstoneEngineJNI");
    //if (aclazz==NULL) {
    //  _STD_ cerr << "Couldn't find livdll.LivingstoneEngineJNI\n";
    //  return;
    //}
    jclass cclazz = env->GetObjectClass(callback);
    jmethodID mid = env->GetMethodID(cclazz, "update",
                                     "(Ljava/util/Observable;Ljava/lang/Object;)V");
    if (mid==NULL) {
      _STD_ cerr << "Couldn't find the callback's update method\n";
      return;
    }

    jthrowable exc;

    Conflict_db& db = tracker->get_t_system();
    Variable::ListOfVariable::iterator it = db.begin_variables();

    bool stateVarSeen = false;
    char c_name[MBA_MAX_LINE_SIZE];
    for (;it != db.end_variables(); ++it) {
      MBA_string name = debugger->get_var_name(**it);
      c_name[0] = '0';
      strcpy(c_name, name.c_str());
      // _STD_ cerr << "mapOverAttributeNamesJNI: name " << name << _STD_ endl;

      // discard all variables with # in the name
      char* delimiter_addr = strchr( c_name, '#');
      // but keep state var which always have a #, e.g. 13#state
      // since it does not have current values, only truncated history values
      if ((stateVarSeen == false) && (delimiter_addr != NULL) &&
          (strncmp( "#state", delimiter_addr, 6) == 0)) {
        stateVarSeen = true;
        c_name[0] = '0';
        strcpy(c_name, "state");
      }
      if ((delimiter_addr == NULL) ||
          ((delimiter_addr != NULL) && (strcmp( c_name, "state") == 0))) {
        env->CallVoidMethod(callback, mid, NULL, env->NewStringUTF(c_name));

        // Error handling
        exc = env->ExceptionOccurred();
        if (exc) {
          env->ExceptionDescribe();
          env->ExceptionClear();
          break;
        }
      }
      // I don't think I have to deallocate anything.
    }

    return;
  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
    throw_error(env, env->GetObjectClass(self), "mapOverAttributeNamesJNI: ",
                "%s", lobj.error_string.c_str());
    return;
  } catch (...) {
    _STD_ cerr << "mapOverAttributeNamesJNI: uncaught error\n";
    return;
  }
#endif
}

JNIEXPORT void JNICALL Java_livdll_LivingstoneEngineJNI_mapOverCommandAttributeNamesJNI
(JNIEnv *env, jobject self, jobject callback) {
  L2_TRY {
    Livingstone_debug *pack = fetchLivPointer(env,self);
    Tracker* tracker = pack->get_tracker();
    Tracker_debug *debugger = pack->get_debugger();

    jclass cclazz = env->GetObjectClass(callback);
    jmethodID mid = env->GetMethodID(cclazz, "update",
                                     "(Ljava/util/Observable;Ljava/lang/Object;)V");
    if (mid==NULL) {
      _STD_ cerr << "Couldn't find the callback's update method\n";
      return;
    }

    jthrowable exc;

    T_system& t_system = tracker->get_t_system();
    for (unsigned i = 0; i < t_system.npresent_variables(); i++) {
      Variable *pVariable = t_system.get_present_variable(i);
      if (!pVariable->is_command()) continue;
      Command *pCommand = static_cast<Command *>(pVariable);
      const char* name = debugger->get_var_name(*pCommand).c_str();
      // _STD_ cerr << "mapOverCommandAttributeNamesJNI: name " << name << _STD_ endl;

      // discard all variables with # in the name
      char* delimiter_addr = strchr( name, '#');
      if (delimiter_addr == NULL) {
        env->CallVoidMethod(callback, mid, NULL, env->NewStringUTF(name));

        // Error handling
        exc = env->ExceptionOccurred();
        if (exc) {
          env->ExceptionDescribe();
          env->ExceptionClear();
          break;
        }
      }
      // I don't think I have to deallocate anything.
    }

    return;
  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
    throw_error(env, env->GetObjectClass(self), "mapOverCommandAttributeNamesJNI: ",
                "%s", lobj.error_string.c_str());
    return;
  } catch (...) {
    _STD_ cerr << "mapOverCommandAttributeNamesJNI: uncaught error\n";
    return;
  }
#endif
}

JNIEXPORT void JNICALL Java_livdll_LivingstoneEngineJNI_mapOverObservableAttributeNamesJNI
(JNIEnv *env, jobject self, jobject callback) {
  L2_TRY {
      Livingstone_debug *pack = fetchLivPointer(env,self);
      Tracker* tracker = pack->get_tracker();
      Tracker_debug *debugger = pack->get_debugger();

    jclass cclazz = env->GetObjectClass(callback);
    jmethodID mid = env->GetMethodID(cclazz, "update",
                                     "(Ljava/util/Observable;Ljava/lang/Object;)V");
    if (mid==NULL) {
      _STD_ cerr << "Couldn't find the callback's update method\n";
      return;
    }

    jthrowable exc;

    T_system& t_system = tracker->get_t_system();
    for (unsigned i = 0; i < t_system.npresent_variables(); i++) {
      Variable *pVariable = t_system.get_present_variable(i);
      if (!pVariable->is_observable()) continue;
      Observable *pObservable = static_cast<Observable *>(pVariable);
      const char* name = debugger->get_var_name(*pObservable).c_str();

      // _STD_ cerr << "mapOverObservableAttributeNamesJNI: name " << name << _STD_ endl;

      // discard all variables with # in the name
      char* delimiter_addr = strchr( name, '#');
      if (delimiter_addr == NULL) {
        env->CallVoidMethod(callback, mid, NULL, env->NewStringUTF(name));

        // Error handling
        exc = env->ExceptionOccurred();
        if (exc) {
          env->ExceptionDescribe();
          env->ExceptionClear();
          break;
        }
      }
      // I don't think I have to deallocate anything.
    }

    return;
  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
    throw_error(env, env->GetObjectClass(self), "mapOverObervableAttributeNamesJNI: ",
                "%s", lobj.error_string.c_str());
    return;
  } catch (...) {
    _STD_ cerr << "mapOverObervableAttributeNamesJNI: uncaught error\n";
    return;
  }
#endif
}


JNIEXPORT jboolean JNICALL Java_livdll_LivingstoneEngineJNI_setAttributeModeJNI
(JNIEnv *env, jobject self, jstring variable, jstring mode) {

  L2_TRY {
    jboolean b1, b2;
    MBA_string variable_str, mode_str;
    const char *str1 = env->GetStringUTFChars(variable, &b1);
    variable_str = str1;
    if (b1 == JNI_TRUE)
      env->ReleaseStringUTFChars( variable, str1);
    const char *str2 = env->GetStringUTFChars(mode , &b2);
    mode_str = str2;
    if (b2 == JNI_TRUE)
      env->ReleaseStringUTFChars( mode, str2);

    Livingstone_debug *pack = fetchLivPointer(env,self);
    T_system_debug* debugger = pack->get_debugger();

    bool consistent_p = debugger->assign_mode( variable_str +
                                               MBA_string("=") + mode_str);
//     if (! consistent_p) {
//       char error_str[MBA_MAX_LINE_SIZE];
//       sprintf(error_str, "`%s=%s' inconsistent: unassign or change conflicting observations",
//               variable_str.c_str(), mode_str.c_str());
//       throw_error(env, env->GetObjectClass(self), "setAttributeModeJNI: ", error_str);
//     }
    // return true/false for valid mode variable, but inconsistent mode value
    return (jboolean) consistent_p;
  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
    // throw error for invalid mode variable
    throw_error(env, env->GetObjectClass(self), "setAttributeModeJNI: ", "%s",
                lobj.error_string.c_str());
  } catch (...) {
      _STD_ cerr << "setAttributeModeJNI: uncaught error\n";
  }
#endif
  return (jboolean) true;
}


#ifdef ENABLE_CHECKPOINT

JNIEXPORT void JNICALL Java_livdll_LivingstoneEngineJNI_createCheckpointJNI
(JNIEnv *env, jobject self, jstring name) {

  L2_TRY {
    jboolean b;
    MBA_string name_str;
    const char *str = env->GetStringUTFChars(name, &b);
    name_str = str;
    if (b == JNI_TRUE)
      env->ReleaseStringUTFChars( name, str);

    Livingstone_debug *pack = fetchLivPointer(env,self);
    Tracker_debug* debugger = pack->get_debugger();

    debugger->create_checkpoint( name_str);
  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
    throw_error(env, env->GetObjectClass(self), "createCheckpointJNI : ", "%s",
                lobj.error_string.c_str());
  } catch (...) {
      _STD_ cerr << "createCheckpointJNI: uncaught error\n";
  }
#endif
}


JNIEXPORT jboolean JNICALL Java_livdll_LivingstoneEngineJNI_restoreCheckpointJNI
(JNIEnv *env, jobject self, jstring name) {

  L2_TRY {
    jboolean b;
    MBA_string name_str;
    const char *str = env->GetStringUTFChars(name, &b);
    name_str = str;
    if (b == JNI_TRUE)
      env->ReleaseStringUTFChars( name, str);

    Livingstone_debug *pack = fetchLivPointer(env,self);
    Tracker_debug* debugger = pack->get_debugger();

    return (jboolean) debugger->restore_checkpoint( name_str);

  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
    throw_error(env, env->GetObjectClass(self), "restoreCheckpointJNI : ", "%s",
                lobj.error_string.c_str());
  } catch (...) {
      _STD_ cerr << "restoreCheckpointJNI: uncaught error\n";
  }
#endif
  return (jboolean) true;
}


JNIEXPORT void JNICALL Java_livdll_LivingstoneEngineJNI_deleteCheckpointJNI
(JNIEnv *env, jobject self, jstring name) {

  L2_TRY {
    jboolean b;
    MBA_string name_str;
    const char *str = env->GetStringUTFChars(name, &b);
    name_str = str;
    if (b == JNI_TRUE)
      env->ReleaseStringUTFChars( name, str);

    Livingstone_debug *pack = fetchLivPointer(env,self);
    Tracker_debug* debugger = pack->get_debugger();

    debugger->delete_checkpoint( name_str);
  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
    throw_error(env, env->GetObjectClass(self), "deleteCheckpointJNI : ", "%s",
                lobj.error_string.c_str());
  } catch (...) {
      _STD_ cerr << "deleteCheckpointJNI: uncaught error\n";
  }
#endif
}

#endif


JNIEXPORT jint JNICALL Java_livdll_LivingstoneEngineJNI_getTimeStepJNI
(JNIEnv *env, jobject self) {
  L2_TRY {
      return (jint)fetchTracker(env,self)->get_t_system().get_time_step();
  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
      throw_error(env, env->GetObjectClass(self), "getTimeStepJNI: ", "%s",
                  lobj.error_string.c_str());
    return 0;
  } catch (...) {
    _STD_ cerr << "getTimeStepJNI: uncaught error\n";
    return 0;
  }
#endif
}

JNIEXPORT void JNICALL Java_livdll_LivingstoneEngineJNI_setTransitionVariableAtTimeJNI
(JNIEnv *env, jobject self, jstring var_jstring, jstring val_jstring, jint time) {
  L2_TRY {
    jboolean var_isCopy, val_isCopy;
    const char* var_str = env->GetStringUTFChars(var_jstring, &var_isCopy);
    const char* val_str = env->GetStringUTFChars(val_jstring, &val_isCopy);
    char err_str[MBA_MAX_LINE_SIZE];

    T_system_debug* debugger = fetchDebugger(env,self);

    Variable* var = debugger->find_at_timestep(var_str,time);
    if (var==NULL) {
        sprintf(err_str, "No such variable `%s' in assignment",
           var_str);
        throw_error(env, env->GetObjectClass(self), "setTransitionVariableAtTimeJNI: ",
                    err_str);
    } else if (!var->is_assignable()) {
        sprintf(err_str, "Variable `%s' is not assignable",
          var_str);
        throw_error(env, env->GetObjectClass(self), "setTransitionVariableAtTimeJNI: ",
                    err_str);
    } else {
        char var_str_arg[MBA_MAX_LINE_SIZE];
        strncpy(var_str_arg , var_str , MBA_MAX_LINE_SIZE);
        debugger->parse_cmd("assign", var_str_arg);
    }

    if (var_isCopy == JNI_TRUE) env->ReleaseStringUTFChars(var_jstring, var_str);
    if (val_isCopy == JNI_TRUE) env->ReleaseStringUTFChars(val_jstring, val_str);
  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
      throw_error(env, env->GetObjectClass(self), "setTransitionVariableAtTimeJNI: ",
                  "%s", lobj.error_string.c_str());
  } catch (...) {
    _STD_ cerr << "setTransitionVariableAtTimeJNI: uncaught error\n";
    return;
  }
#endif
}

JNIEXPORT jintArray JNICALL Java_livdll_LivingstoneEngineJNI_getHistoryStartStopJNI
(JNIEnv *env, jobject self) {
  long sz = 2;
  jint historyStartStop[2] = { 0, 0};
  jintArray jintHistoryStartStop = env->NewIntArray( sz);
  env->SetIntArrayRegion( jintHistoryStartStop, 0, sz, historyStartStop);

  L2_TRY {
    Tracker* tracker = fetchTracker(env,self);
    unsigned int startHistory = tracker->get_t_system().get_oldest_timestep();
    unsigned int stopHistory = tracker->get_t_system().get_time_step();
    // _STD_ cerr << "getHistoryStartStopJNI: start " << startHistory << " stop " << stopHistory << _STD_ endl;
    historyStartStop[0] = (jint) startHistory;
    historyStartStop[1] = (jint) stopHistory;
    env->SetIntArrayRegion( jintHistoryStartStop, 0, sz, historyStartStop);
    return jintHistoryStartStop;

  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
    throw_error(env, env->GetObjectClass(self), "getHistoryStartStopJNI: ", "%s",
                lobj.error_string.c_str());
    return jintHistoryStartStop;
  } catch (...) {
    _STD_ cerr << "getHistoryStartStopJNI: uncaught error\n";
    return jintHistoryStartStop;
  }
#endif
}



JNIEXPORT void JNICALL Java_livdll_LivingstoneEngineJNI_listenerUsageJNI
(JNIEnv *env, jobject self) {
  L2_TRY {
      fetchDebugger(env,self)->listener_usage();
  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
    throw_error(env, env->GetObjectClass(self), "listenerUsageJNI: ", "%s",
                lobj.error_string.c_str());
    return;
  } catch (...) {
    _STD_ cerr << "listenerUsageJNI: uncaught error\n";
    return;
  }
#endif
}


JNIEXPORT jboolean JNICALL Java_livdll_LivingstoneEngineJNI_parseCmdJNI
(JNIEnv *env, jobject self, jstring cmd_jstring, jstring args_jstring) {
  L2_TRY {
    jboolean var_isCopy, val_isCopy;
    const char* cmd_str = env->GetStringUTFChars(cmd_jstring, &var_isCopy);
    const char* args_str = env->GetStringUTFChars(args_jstring, &val_isCopy);

    // Currently, parse_cmd may modify the argument string while parsing it.
    // That should be fixed.
    char cmd[MBA_MAX_LINE_SIZE];
    char args[MBA_MAX_LINE_SIZE];
    strncpy(cmd,cmd_str,MBA_MAX_LINE_SIZE);
    strncpy(args,args_str,MBA_MAX_LINE_SIZE);

    return (jboolean)fetchDebugger(env, self)->parse_cmd(cmd, args);
  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
    throw_error(env, env->GetObjectClass(self), "parse_cmdJNI: ", "%s",
                lobj.error_string.c_str());
    return (jboolean) false;
  } catch (...) {
    _STD_ cerr << "parse_cmdJNI: uncaught error\n";
    return (jboolean) false;
  }
#endif
}

JNIEXPORT jint JNICALL Java_livdll_LivingstoneEngineJNI_justifyPropositionJNI
(JNIEnv *env, jobject self, jstring propname) {
  L2_TRY {
    int result = 0;
    jboolean isCopy;
    const char* propname_str = env->GetStringUTFChars(propname, &isCopy);

    MBA_string propname_mba(propname_str);

      Tracker_debug* debugger = fetchDebugger(env,self);

      Proposition* prop = debugger->find_proposition(propname_mba);

      if (prop==NULL)
          return 0;			// couldn't find the proposition

      Clause* support = prop->get_supported_by();
      if (!support)
          return 1;			// proposition is unsupported

      // Set up the callback
      jclass clazz = env->GetObjectClass(self);
      jmethodID midp = env->GetMethodID(clazz,"mapOverJustificationPositive","(Ljava/lang/String;)V");
      jmethodID midn = env->GetMethodID(clazz,"mapOverJustificationNegative","(Ljava/lang/String;)V");

      Clause::iterator it = support->begin_positive();
      Clause::iterator it_end = support->end_positive();

      jthrowable exc;

      for (; it != it_end; ++it) {
          if (prop != *(it)) {
              MBA_string str = debugger->get_prop_name(**it);
              env->CallVoidMethod(self,midp,env->NewStringUTF(str.c_str()));
              exc = env->ExceptionOccurred();
              if (exc) {
                  result = 2;
                  env->ExceptionDescribe();
                  env->ExceptionClear();
                  break;
              }
          }
      }

      it = support->begin_negative();
      it_end =support->end_negative();

      for (; it != it_end; ++it) {
          if (prop != *(it)) {
              MBA_string str = debugger->get_prop_name(**it);
              env->CallVoidMethod(self,midn,env->NewStringUTF(str.c_str()));
              exc = env->ExceptionOccurred();
              if (exc) {
                  result = 2;
                  env->ExceptionDescribe();
                  env->ExceptionClear();
                  break;
              }
          }
      }

      // Clean up
      if (isCopy == JNI_TRUE) env->ReleaseStringUTFChars(propname, propname_str);

      return 3;
  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
      throw_error(env, env->GetObjectClass(self), "justifyPropositionJNI: ", "%s",
                  lobj.error_string.c_str());
      return 0;
  } catch (...) {
      _STD_ cerr << "justifyPropositionJNI: uncaught error\n";
      return 0;
  }
#endif
}

JNIEXPORT jstring JNICALL Java_livdll_LivingstoneEngineJNI_variableValueAtTimeJNI
(JNIEnv *env, jobject self, jstring var_jstring, jint time) {
    jboolean var_isCopy;
    const char *jni_var_str = env->GetStringUTFChars(var_jstring, &var_isCopy);
    MBA_string var_str = jni_var_str;
    MBA_string val;
    char err_str[MBA_MAX_LINE_SIZE];
    // _STD_ cerr << "variableValueAtTimeJNI: var_str " << var_str << _STD_ endl;
    L2_TRY {
        Livingstone_debug *pack = fetchLivPointer(env,self);
        Tracker* tracker = pack->get_tracker();
        Tracker_debug *debugger = pack->get_debugger();

        Variable* var;
        if ((unsigned)time==tracker->get_t_system().get_time_step()) {
            var = debugger->find_variable(var_str);
        } else {
            var = debugger->find_at_timestep(var_str.c_str(),time);
        }
        if (var==NULL) {
            sprintf(err_str, "No such variable `%s'", var_str.c_str());
            // do not output error to cmd line, so do not call throw_error
            // which does print it
            // throw_error(env, env->GetObjectClass(self), "variableValueAtTimeJNI: ", err_str);
            // the ThrowNew is used by HistoryTable to display <undefined>
            // in value slots
            jclass aclazz = env->GetObjectClass(self);
            if (aclazz==NULL) {
                _STD_ cerr<< err_str << _STD_ endl
                    << "Couldn't find livdll.LivingstoneEngineJNI\n" ;
            } else {
              jmethodID mid = env->GetMethodID(aclazz, "createNativeException",
                                               "(Ljava/lang/String;)Ljava/lang/Throwable;");
              jobject throwable =
                env->CallStaticObjectMethod(aclazz, mid, env->NewStringUTF(err_str));
              env->Throw((jthrowable)throwable);
            }
            return NULL;
        } else {
            Variable& var1 = *var;
            val = debugger->get_variable_value(var1);
        }

        if (var_isCopy == JNI_TRUE)
            env->ReleaseStringUTFChars(var_jstring, jni_var_str);
        return env->NewStringUTF(val.c_str());
    }
#ifdef ENABLE_EXCEPTIONS
    catch (L2_error lobj) {
        throw_error(env, env->GetObjectClass(self), "variableValueAtTimeJNI: ", "%s",
                    lobj.error_string.c_str());
        return NULL;
    } catch (...) {
        _STD_ cerr << "variableValueAtTimeJNI: uncaught error\n";
        return NULL;
    }
#endif
}

JNIEXPORT jboolean JNICALL Java_livdll_LivingstoneEngineJNI_variableExogenousAtTimeJNI
(JNIEnv *env, jobject self, jstring var_jstring, jint time) {
    jboolean var_isCopy;
    const char *jni_var_str = env->GetStringUTFChars(var_jstring, &var_isCopy);
    MBA_string var_str = jni_var_str;
    MBA_string val;
    bool isExogenous = false;
    char err_str[MBA_MAX_LINE_SIZE];
    // _STD_ cerr << "variableExogenousAtTimeJNI: var_str " << var_str << _STD_ endl;
    L2_TRY {
        Livingstone_debug *pack = fetchLivPointer(env,self);
        Tracker* tracker = pack->get_tracker();
        Tracker_debug *debugger = pack->get_debugger();

        Variable* var;
        if ((unsigned)time==tracker->get_t_system().get_time_step()) {
            var = debugger->find_variable(var_str);
        } else {
            var = debugger->find_at_timestep(var_str.c_str(),time);
        }
        if (var==NULL) {
            sprintf(err_str, "No such variable `%s'", var_str.c_str());
            // do not output error to cmd line, so do not call throw_error
            // which does print it
            // throw_error(env, env->GetObjectClass(self), "variableExogenousAtTimeJNI: ", err_str);
            // the ThrowNew is used by HistoryTable to display <undefined>
            // in value slots
            jclass aclazz = env->GetObjectClass(self);
            if (aclazz==NULL) {
                _STD_ cerr<< err_str << _STD_ endl
                    << "Couldn't find L2_error_Exception\n" ;
            } else {
              jmethodID mid = env->GetMethodID(aclazz, "createNativeException",
                                               "(Ljava/lang/String;)Ljava/lang/Throwable;");
              jobject throwable =
                env->CallStaticObjectMethod(aclazz, mid, env->NewStringUTF(err_str));
              env->Throw((jthrowable)throwable);
            }
            return (jboolean)false;
        } else {
            isExogenous = debugger->is_exogenously_assigned(var);
        }
        if (var_isCopy == JNI_TRUE)
            env->ReleaseStringUTFChars(var_jstring, jni_var_str);
        return (jboolean) isExogenous;
    }
#ifdef ENABLE_EXCEPTIONS
    catch (L2_error lobj) {
        throw_error(env, env->GetObjectClass(self), "variableExogenousAtTimeJNI: ",
                    "%s", lobj.error_string.c_str());
        return (jboolean)false;
    } catch (...) {
        _STD_ cerr << "variableExogenousAtTimeJNI: uncaught error\n";
        return (jboolean)false;
    }
#endif
}

JNIEXPORT void JNICALL Java_livdll_LivingstoneEngineJNI_printPropClausesJNI
(JNIEnv *env, jobject self, jstring propname) {
  L2_TRY {
    jboolean isCopy;
    const char* propname_str = env->GetStringUTFChars(propname, &isCopy);
    MBA_string propname_mba(propname_str);
    Tracker_debug *debugger = fetchDebugger(env,self);

    Proposition* prop = debugger->find_proposition( propname_mba);
    if (prop==NULL) {
      _STD_ cerr << ":prop_clauses <couldn't find proposition> " << propname_mba << _STD_ endl;
      return;
    }

    Clause* clause_p;

    _STD_ cerr << ":prop_clauses " << debugger->get_prop_name(*prop) << _STD_ endl;

    int count = 1;
    Proposition::iterator pit = prop->begin_positive();
    Proposition::iterator pit_end = prop->end_positive();
    for (; pit != pit_end; ++pit) {
      clause_p = *pit;
      if (!clause_p->is_deleted()) {
	// _STD_ cerr << "  " << count++ << ".  " << clause_p << "\n";
	_STD_ cerr << "  " << count++ << ".  ";
        debugger->pretty_print_clause(* clause_p);
        _STD_ cerr << "\n";
      }
    }

    Proposition::iterator nit = prop->begin_negative();
    Proposition::iterator nit_end = prop->end_negative();
    for (; nit != nit_end; ++nit) {
      clause_p = *nit;
      if (!clause_p->is_deleted()) {
	// _STD_ cerr << "  " << count++ << ".  " << clause_p << "\n";
 	_STD_ cerr << "  " << count++ << ".  ";
        debugger->pretty_print_clause(* clause_p);
        _STD_ cerr << "\n";
     }
    }

    _STD_ cerr << "======> ";
    // Clean up
    if (isCopy == JNI_TRUE) env->ReleaseStringUTFChars(propname, propname_str);

  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
      throw_error(env, env->GetObjectClass(self), "printPropClausesJNI: ", "%s",
                  lobj.error_string.c_str());
  } catch (...) {
    _STD_ cerr << "printPropClausesJNI: uncaught error\n";
  }
#endif
}

JNIEXPORT void JNICALL Java_livdll_LivingstoneEngineJNI_executeWarpCommandsJNI
(JNIEnv *env, jobject self, jstring commands) {
  L2_TRY {
    // execute L2 cmds without Skunkworks/Stanley updates or interaction
    jboolean isCopy;
    const char* commands_str = env->GetStringUTFChars(commands, &isCopy);
    // _STD_ cerr << "executeWarpCommandsJNI: commands_str " << commands_str << _STD_ endl;
    Tracker_debug *debugger = fetchDebugger(env,self);
    char cmd[MBA_MAX_LINE_SIZE];
    char args[MBA_MAX_LINE_SIZE];
    char substring[MBA_MAX_LINE_SIZE * 2];
    int commands_str_len = strlen( commands_str);
    int cmds_start_indx = 0, cmds_stop_indx, args_indx;
    bool completedP, firstP = true;
    char *cmds_stop_addr, *args_addr, *substring_addr = &substring[0];
    while (cmds_start_indx < commands_str_len - 1) {
      cmd[0] = '\0';
      args[0] = '\0';
      cmds_stop_addr = strchr((char *) (commands_str + cmds_start_indx), '\n');
      cmds_stop_indx = (int) (cmds_stop_addr - commands_str);
      // _STD_ cerr << "cmds_stop_indx " << cmds_stop_indx << _STD_ endl;
      substring[0] = '\0';
      strncpy( substring, (char *) (commands_str + cmds_start_indx),
              cmds_stop_indx - cmds_start_indx);
      substring[cmds_stop_indx - cmds_start_indx] = '\0';
      // _STD_ cerr << "substring " << substring << _STD_ endl;
      // get cmd and args
      args_addr = strstr(substring_addr, " ");
      if (args_addr == NULL) {
        strcpy( cmd, substring_addr);
      } else {
        args_indx = (int) (args_addr - substring_addr);
        strncpy( cmd, substring_addr, args_indx);
        cmd[args_indx]  = '\0';
        strcpy( args, (char *) (substring_addr + args_indx +1));
      }
      // _STD_ cerr << "cmd " << cmd << " args " << args << _STD_ endl;
      if (firstP)
        firstP = false;
      else
        _STD_ cerr << "======> ";
      _STD_ cerr << cmd << " " << args << _STD_ endl;

      completedP = debugger->parse_cmd(cmd, args);
      if (completedP == false)
        break;
      cmds_start_indx = cmds_stop_indx + 1;
    }
    _STD_ cerr << "======> ";

    // Clean up
    if (isCopy == JNI_TRUE) env->ReleaseStringUTFChars(commands, commands_str);

  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
      throw_error(env, env->GetObjectClass(self), "executeWarpCommandsJNI: ",
                  "%s", lobj.error_string.c_str());
  } catch (...) {
    _STD_ cerr << "executeWarpCommandsJNI: uncaught error\n";
  }
#endif
}

// this is kept since the GPU, actually the RTI still uses this
// gov/nasa/arc/skunkworks/livingstone/data/L2Publisher.java
// and
// Used by Charles Pecheur in Livingstone Pathfinder
//
JNIEXPORT void JNICALL Java_livdll_LivingstoneEngineJNI_findCandidatesJNI
(JNIEnv *env, jobject self) {
  L2_TRY {
    Tracker* tracker = fetchTracker(env,self);
    Livingstone_debug* pack = fetchLivPointer(env, self);
    Tracker_debug* debug = pack->get_debugger();

    // tracker->prune_and_search();
    // since src/transition/tracker.cpp does not handle this properly
    // do it here
    tracker->get_t_system().prune_inconsistent_candidates();
    if (tracker->get_t_system().getCandidates().empty()) {
      tracker->find_fresh_candidates();
      JNI_CBFS_COVER_search = 1;
    } else {
      JNI_CBFS_COVER_search = 0;
    }

    if (tracker->get_t_system().getCandidates().empty())
      _STD_ cerr << "No candidates can be found.\n";
    else {
      _STD_ cerr << "Consistent candidates are:\n" ;
      debug->print_candidates();
    }

  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
    throw_error(env, env->GetObjectClass(self), "findCandidatesJNI: ", "%s",
                lobj.error_string.c_str());
    return;
  } catch (...) {
    _STD_ cerr << "findCandidatesJNI: uncaught error\n";
    return;
  }
#endif
}

// this is kept since the GPU, actually the RTI still uses this
// gov/nasa/arc/skunkworks/livingstone/data/L2Publisher.java
// and
// Used by Charles Pecheur in Livingstone Pathfinder
//
JNIEXPORT void JNICALL Java_livdll_LivingstoneEngineJNI_assignJNI
(JNIEnv *env, jobject self, jstring var_jstring, jstring val_jstring) {
  L2_TRY {
    jboolean var_isCopy, val_isCopy;
    const char* var_str = env->GetStringUTFChars(var_jstring, &var_isCopy);
    const char* val_str = env->GetStringUTFChars(val_jstring, &val_isCopy);
    // _STD_ cerr << "assign var_str=" << var_str << "  val_str=" << val_str << _STD_ endl;
    Variable *var = fetchDebugger(env,self)->find_variable(var_str);
    char err_str[MBA_MAX_LINE_SIZE];

    if (var==NULL) {
        sprintf(err_str, "No such variable `%s' in assignment", var_str);
        throw_error(env, env->GetObjectClass(self), "assignJNI: ", err_str);
    } else if (!var->is_assignable()) {
        sprintf(err_str, "Variable `%s' is not assignable", var_str);
        throw_error(env, env->GetObjectClass(self), "assignJNI: ", err_str);
    } else {
      sprintf(err_str, "%s=%s", var_str, val_str);
      fetchDebugger(env,self)->assign( err_str);
    }
    if (var_isCopy == JNI_TRUE) env->ReleaseStringUTFChars(var_jstring,var_str);
    if (val_isCopy == JNI_TRUE) env->ReleaseStringUTFChars(val_jstring,val_str);
  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
      throw_error(env, env->GetObjectClass(self), "assignJNI: ", "%s",
                  lobj.error_string.c_str());
  } catch (...) {
    _STD_ cerr << "assignJNI: uncaught error\n";
    return;
  }
#endif
}

// Used by Charles Pecheur in Livingstone Pathfinder
//
JNIEXPORT void JNICALL Java_livdll_LivingstoneEngineJNI_addAssumptionJNI
(JNIEnv *env, jobject self, jstring var_jstring, jstring val_jstring) {
  L2_TRY {
    jboolean var_isCopy, val_isCopy;
    const char* var_str = env->GetStringUTFChars(var_jstring, &var_isCopy);
    const char* val_str = env->GetStringUTFChars(val_jstring, &val_isCopy);
    // _STD_ cerr << "assign var_str=" << var_str << "  val_str=" << val_str << _STD_ endl;
    Variable *var = fetchDebugger(env,self)->find_variable(var_str);
    char err_str[MBA_MAX_LINE_SIZE];

    if (var==NULL) {
        sprintf(err_str, "No such variable `%s' in assignment", var_str);
        throw_error(env, env->GetObjectClass(self), "addAssumptionJNI: ", err_str);
    } else if (!var->is_assignable()) {
        sprintf(err_str, "Variable `%s' is not assignable", var_str);
        throw_error(env, env->GetObjectClass(self), "addAssumptionJNI: ", err_str);
    } else {
      sprintf(err_str, "%s=%s", var_str, val_str);
      MBA_string args = err_str;
      Assignment *pAssignment = fetchDebugger(env,self)->find_assignment(args, true);
      if (pAssignment != NULL) {
        fetchTracker(env,self)->get_t_system().addAssumption(pAssignment);
      }
    }
    if (var_isCopy == JNI_TRUE) env->ReleaseStringUTFChars(var_jstring,var_str);
    if (val_isCopy == JNI_TRUE) env->ReleaseStringUTFChars(val_jstring,val_str);
  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
      throw_error(env, env->GetObjectClass(self), "addAssumptionJNINI: ", "%s",
                  lobj.error_string.c_str());
  } catch (...) {
    _STD_ cerr << "addAssumptionJNI: uncaught error\n";
    return;
  }
#endif
}

// For Skunkworks: superseded by mapOverAttributeAssignmentsJNI
// Used by Charles Pecheur in Livingstone Pathfinder
//
JNIEXPORT void JNICALL Java_livdll_LivingstoneEngineJNI_mapOverPropositionStringsJNI
(JNIEnv *env, jobject self) {
  L2_TRY {
      Livingstone_debug *pack = fetchLivPointer(env,self);
      Tracker* tracker = pack->get_tracker();
      Conflict_db_debug* debugger = pack->get_debugger();

      // Set up the iterator
      Ptheory::prop_list::iterator it =
          tracker->get_t_system().get_solver()->begin_propositions();
      Ptheory::prop_list::iterator it_end =
          tracker->get_t_system().get_solver()->end_propositions();

    // Set up the callback
    jclass clazz = env->GetObjectClass(self);
    jmethodID mid = env->GetMethodID(clazz,"mapOverPropositionStringsCallback","(Ljava/lang/String;)V");

    jthrowable exc;

    for (; it != it_end; ++it) {
        MBA_string prop_name = debugger->get_prop_name(**it);

      //_STD_ cerr << "str=" << str << _STD_ endl;
      env->CallVoidMethod(self,mid,env->NewStringUTF(prop_name.c_str()));
      exc = env->ExceptionOccurred();
      if (exc) {
	env->ExceptionDescribe();
	env->ExceptionClear();
	break;
      }
    }
  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
    throw_error(env, env->GetObjectClass(self), "mapOverPropositionStringsJNI: ",
                "%s", lobj.error_string.c_str());
    return;
  } catch (...) {
    _STD_ cerr << "mapOverPropositionStringsJNI: uncaught error\n";
    return;
  }
#endif
}


//Added By RG

JNIEXPORT void JNICALL Java_livdll_LivingstoneEngineJNI_findDiscrepancysJNI
(JNIEnv *env, jobject self, jobject callback) {

  L2_TRY{

     Assignment::ListOfAssignment assignments;
     const Variable *var;
     MBA_string name, value;
     Assignment::weight_t weight;
     jthrowable exc;

     jclass dclazz = env->GetObjectClass(self);
     if (dclazz==NULL) {
       _STD_ cerr << "Couldn't find livdll.LivingstoneEngineJNI\n";
       return;
     }

     jmethodID dconstructor =
       env->GetMethodID(dclazz, "createDiscrepancy",
                        "(Ljava/lang/String;Ljava/lang/String;I)Ljava/lang/Object;");
     if (dconstructor==NULL) {
       _STD_ cerr << "Couldn't find livdll.LivingstoneEngineJNI.createDiscrepancy()\n";
       return;
     }

     jclass cclazz = env->GetObjectClass(callback);
     jmethodID mid = env->GetMethodID(cclazz, "update",
                                      "(Ljava/util/Observable;Ljava/lang/Object;)V");
     if (mid==NULL) {
       _STD_ cerr << "Couldn't find the callback's update method\n";
       return;
     }

     fetchTracker(env,self)->get_t_system().get_solver()->propagate();
     fetchDebugger(env,self)->find_discrepant_observations(assignments);
     fetchDebugger(env,self)->find_discrepant_commands(assignments);

     for (Assignment::ListOfAssignment::iterator it = assignments.begin();
          it != assignments.end();  ++it) {
       Assignment *assignment = *it;
       var = assignment->get_variable();
       name = fetchDebugger(env,self)->get_var_name(*var);
       value = fetchDebugger(env,self)->get_variable_value(*var, assignment->get_index());
       weight = assignment->get_weight();
       jobject newobj = env->CallStaticObjectMethod(dclazz, dconstructor,
                                                    env->NewStringUTF(name.c_str()),
                                                    env->NewStringUTF(value.c_str()),
                                                    weight);
       env->CallVoidMethod(callback,mid, NULL, newobj);
       exc = env->ExceptionOccurred();

       if (exc) {
           env->ExceptionDescribe();
           env->ExceptionClear();
           break;
       }
     }
  }
#ifdef ENABLE_EXCEPTIONS
  catch (L2_error lobj) {
    throw_error(env, env->GetObjectClass(self), "findDiscrepancysJNI: ", "%s",
                lobj.error_string.c_str());
    return ;
  } catch (...) {
    _STD_ cerr << "findDiscrepancysJNI: uncaught error\n";
  }
#endif
}




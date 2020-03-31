//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.tools.historyTable;

import java.util.Enumeration;
import java.util.Hashtable;

/* A transcript is an output stream intended to hold a record of program
   output.  This class provides a central place for tools to find
   transcripts by name or type. */

public class TranscriptRegistry {

  /* Maps names to transcript streams */
  static Hashtable registry = new Hashtable();

  /* Register a transcript for use */
  public static void addTranscript(String name, Object transcript) {
    registry.put(name,transcript);
  }

  /* Remove a transcript by name */
  public static void removeTranscript(String name) {
    registry.remove(name);
  }

  /* Remove a given transcript */
  public static void removeTranscript(Object transcript) {
    for (Enumeration e=registry.keys();e.hasMoreElements();) {
      String key = (String)e.nextElement();
      Object t = registry.get(key);
      if (transcript==t) {
	registry.remove(key);
	return;
      }
    }
  }

  /* Return the transcript with the given name */
  public static Object getTranscript(String name) {
    return registry.get(name);
  }

  /* Return any transcript of the given type */
  public static Object getTranscriptByType(Class type) {
    for (Enumeration e=registry.elements();e.hasMoreElements();) {
      Object t = e.nextElement();
      if (type.isInstance(t)) return t;
    }
    return null;
  }

}

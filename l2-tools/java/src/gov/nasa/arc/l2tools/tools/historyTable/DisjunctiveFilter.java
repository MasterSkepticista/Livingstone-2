//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.tools.historyTable;

import java.util.ArrayList;

/* A filter that accepts strings matching a disjunctive pattern */

public class DisjunctiveFilter {

  /* */
  protected String pattern = "";

  /* This has a value when the pattern is disjunctive.  Otherwise, it's
     null and we use the pattern ivar. */
  protected ArrayList disjunction = null;

  public DisjunctiveFilter() {
  }

  public DisjunctiveFilter(String pattern) {
    setPattern(pattern);
  }

  public synchronized void setPattern(String pattern) {
    this.pattern = pattern;
    compilePattern(pattern);
  }

  protected void compilePattern(String p) {
    ArrayList d = new ArrayList(3);
    int i;
    int length;
    // System.err.println("compilePattern: p `" + p + "'");
    while ((length = p.length()) > 0) {
      i = p.indexOf('|');
      if (i == -1) {
        i = length;
      }
      d.add(p.substring(0,i).trim());
      // System.err.println("    `" + p.substring(0,i).trim() + "'");
      if (i == length) {
        break;
      }
      p=p.substring(i+1);
    }
    d.add(p);
    disjunction = null;
    if (d.size()>1) disjunction = d;
  }

  /* Return true if the candidate contains one of the options in pattern */
  public synchronized boolean match(String candidate) {
    if (disjunction==null) {
      return candidate.indexOf(pattern)>-1;
    } else {
      int max = disjunction.size();
      for (int i=0;i<max;i++) {
	String p = (String)disjunction.get(i);
	if (candidate.indexOf(p)>-1) return true;
      }
      return false;
    }
  }

  

}

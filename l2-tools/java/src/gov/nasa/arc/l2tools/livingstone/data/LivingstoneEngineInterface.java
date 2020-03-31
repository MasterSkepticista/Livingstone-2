//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * LivingstoneEngineInterface.java
 *
 * Created on February 5, 2001, 4:20 PM
 */

package gov.nasa.arc.l2tools.livingstone.data;

/**
 *
 * @author  bachmann
 * @version 
 */
public interface LivingstoneEngineInterface {
  public int findCandidatesJNI();
  public void progressJNI(String args);
  public void assignJNI(String name, String value);
  public int getTimeStepJNI();
}


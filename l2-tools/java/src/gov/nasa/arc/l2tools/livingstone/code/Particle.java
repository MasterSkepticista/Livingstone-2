//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code;

/**
 * Insert the type's description here.
 * Creation date: (2/10/2000 1:57:24 PM)
 * @author: Andrew Bachmann
 */
public abstract class Particle extends gov.nasa.arc.l2tools.livingstone.DataObject {
  public final static String tagName = "particle";
/**
 * Particle constructor comment.
 */
public Particle() {
	super(tagName);
}
/**
 * Particle constructor comment.
 * @param name java.lang.String
 */
public Particle(String name) {
	super(name);
}
}

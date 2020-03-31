//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.componenttype;

/**
 * Insert the type's description here.
 * Creation date: (3/6/2000 11:51:48 AM)
 * @author: Andrew Bachmann
 */
public abstract class Particle extends gov.nasa.arc.l2tools.livingstone.code.Particle {
	public final static String tagName = "ct:particle";
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

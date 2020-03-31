//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.constraint;

/**
 * Insert the type's description here.
 * Creation date: (2/16/2000 1:20:20 PM)
 * @author: Andrew Bachmann
 */
public abstract class Statement extends gov.nasa.arc.l2tools.livingstone.code.CodeParticle {
	public final static String tagName = "ex:statement";
/**
 * Statement constructor comment.
 */
public Statement() {
	super(tagName);
}
/**
 * Statement constructor comment.
 * @param name java.lang.String
 */
public Statement(String name) {
	super(name);
}
}

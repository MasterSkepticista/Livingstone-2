//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.componenttype;

/**
 * Insert the type's description here.
 * Creation date: (3/6/2000 11:29:50 AM)
 * @author: Andrew Bachmann
 */
public abstract class StateIdentifier extends gov.nasa.arc.l2tools.livingstone.code.Identifier {
	public final static String tagName = "ct:state";
/**
 * StateIdentifier constructor comment.
 */
public StateIdentifier() {
	super(tagName);
}
/**
 * StateIdentifier constructor comment.
 * @param name java.lang.String
 */
public StateIdentifier(String name) {
	super(name);
}
}

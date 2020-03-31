//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.constraint;

/**
 * Insert the type's description here.
 * Creation date: (2/16/2000 1:20:46 PM)
 * @author: Andrew Bachmann
 */
public abstract class Expression extends Statement {
	public final static String tagName = "ex:expr";
/**
 * Expression constructor comment.
 */
public Expression() {
	super(tagName);
}
/**
 * Expression constructor comment.
 * @param name java.lang.String
 */
public Expression(String name) {
	super(name);
}
}

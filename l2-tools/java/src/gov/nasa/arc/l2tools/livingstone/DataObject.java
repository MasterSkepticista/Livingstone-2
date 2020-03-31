//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone;

/**
 * Insert the type's description here.
 * Creation date: (2/10/2000 1:47:46 PM)
 * @author: Andrew Bachmann
 */
public abstract class DataObject extends gov.nasa.arc.l2tools.DataObject {
  public final static String tagName = "object";
/**
 * Object constructor comment.
 */
public DataObject() {
	super(tagName);
}
/**
 * Object constructor comment.
 * @param name java.lang.String
 */
public DataObject(String name) {
	super(name);
}
}

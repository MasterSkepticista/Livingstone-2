//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code;

/**
 * Insert the type's description here.
 * Creation date: (3/23/2000 7:43:57 PM)
 * @author: Andrew Bachmann
 */
public abstract class AbstractValue extends CodeParticle {
  // compilation caches
	private java.lang.String name = null;
	private java.lang.String value = null;
/**
 * AbstractValue constructor comment.
 */
public AbstractValue() {
	super();
}
/**
 * AbstractValue constructor comment.
 * @param name java.lang.String
 */
public AbstractValue(String name) {
	super(name);
}
/**
 *
 * Creation date: (3/22/2000 3:26:12 PM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public java.lang.String getName() {
	return name;
}
/**
 *
 * Creation date: (3/23/2000 11:37:17 AM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public java.lang.String getValue() {
	return value;
}
/**
 * refreshCache method comment.
 */
public void refreshCache() throws gov.nasa.arc.l2tools.Exception {
	try {
		// cache attributes
		if (get("name") != null) {
			setName(get("name"));
		} else {
			String errorString = "illegal anonymous error: each value must have a name";
			throw new gov.nasa.arc.l2tools.livingstone.code.IllegalAnonymousError(errorString);
		}
		if (get("value") != null) {
			setValue(get("value"));
		}
		
		// check for children
		if (!getChildren().isEmpty()) {
			System.out.println("Ignoring children of value");
		}
	} catch (gov.nasa.arc.l2tools.Exception error) {
		error.concatMessage("\n  in a value");
		if (getName()!=null) error.concatMessage(" named '"+getName()+"'");
		if (getValue()!=null) error.concatMessage(" with value '"+getValue()+"'");
		throw error;
	}
}
/**
 * refreshClauses method comment.
 */
public void refreshExpression() throws gov.nasa.arc.l2tools.livingstone.code.CompilerError {
	// nothing interesting to do here
}
/**
 *
 * Creation date: (3/22/2000 3:26:12 PM)
 * @author: Andrew Bachmann
 * 
 * @param newName java.lang.String
 */
protected void setName(java.lang.String newName) {
	name = newName;
}
/**
 *
 * Creation date: (3/23/2000 11:37:17 AM)
 * @author: Andrew Bachmann
 * 
 * @param newValue java.lang.String
 */
protected void setValue(java.lang.String newValue) {
	value = newValue;
}
}

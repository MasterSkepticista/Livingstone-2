//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.attributetype;

/**
 * Insert the type's description here.
 * Creation date: (3/3/2000 3:18:06 PM)
 * @author: Andrew Bachmann
 */
public class Value extends gov.nasa.arc.l2tools.livingstone.code.AbstractValue {
	public final static String tagName = "at:value";
/**
 * AT_Value constructor comment.
 */
public Value() {
	super(tagName);
}
/**
 * AT_Value constructor comment.
 * @param name java.lang.String
 */
public Value(String name) {
	super(name);
}
/**
 * search for a Value with value <i>value</i> for key <i>key</i>
 * Creation date: (10/24/2000 4:05:22 PM)
 * @return gov.nasa.arc.l2tools.livingstone.code.attributetype.Value
 * @param key java.lang.String
 * @param value java.lang.String
 * @param object gov.nasa.arc.l2tools.DataObject
 */
public static Value search(String key, String value, gov.nasa.arc.l2tools.DataObject object) {
	return (Value)object.search(Value.class,key,value);
}
/**
 * search for a Value named <i>name</i>
 * Creation date: (10/24/2000 4:05:22 PM)
 * @return gov.nasa.arc.l2tools.livingstone.code.attributetype.Value
 * @param name java.lang.String
 * @param object gov.nasa.arc.l2tools.DataObject
 */
public static Value searchByName(String name, gov.nasa.arc.l2tools.DataObject object) {
	return (Value)object.searchByName(Value.class,name);
}
}

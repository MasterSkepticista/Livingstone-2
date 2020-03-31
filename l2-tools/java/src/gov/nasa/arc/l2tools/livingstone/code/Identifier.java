//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code;

/**
 * Insert the type's description here.
 * Creation date: (2/24/2000 6:49:12 PM)
 * @author: Andrew Bachmann
 */
public class Identifier extends gov.nasa.arc.l2tools.livingstone.code.Particle {
  public final static String tagName = "identifier";

/**
 * Identifier constructor comment.
 */
public Identifier() {
	super(tagName);
}
/**
 * Identifier constructor comment.
 * @param name java.lang.String
 */
public Identifier(String name) {
	super(name);
}
/**
 * search for a Identifier with value <i>value</i> for key <i>key</i>
 * Creation date: (10/24/2000 4:05:22 PM)
 * @return gov.nasa.arc.l2tools.livingstone.code.Identifier
 * @param key java.lang.String
 * @param value java.lang.String
 * @param object gov.nasa.arc.l2tools.DataObject
 */
public static Identifier search(String key, String value, gov.nasa.arc.l2tools.DataObject object) {
	return (Identifier)object.search(Identifier.class,key,value);
}
/**
 * search for a Identifier named <i>name</i>
 * Creation date: (10/24/2000 4:05:22 PM)
 * @return gov.nasa.arc.l2tools.livingstone.code.Identifier
 * @param name java.lang.String
 * @param object gov.nasa.arc.l2tools.DataObject
 */
public static Identifier searchByName(String name, gov.nasa.arc.l2tools.DataObject object) {
	return (Identifier)object.searchByName(Identifier.class,name);
}
}

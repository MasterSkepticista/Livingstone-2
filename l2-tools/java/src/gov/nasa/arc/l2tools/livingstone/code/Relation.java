//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code;

/**
 * Insert the type's description here.
 * Creation date: (2/11/2000 7:00:12 PM)
 * @author: Andrew Bachmann
 */
public class Relation extends gov.nasa.arc.l2tools.livingstone.code.AbstractRelation {
  public final static String tagName = "relation";
/**
 * Relation constructor comment.
 */
public Relation() {
	super(tagName);
}
/**
 * Relation constructor comment.
 * @param name java.lang.String
 */
public Relation(String name) {
	super(name);
}
/**
 *
 * Creation date: (3/10/2000 7:09:36 PM)
 * @author: Andrew Bachmann
 * 
 */
public void refreshCache() throws gov.nasa.arc.l2tools.Exception {
	try {
		if ((get("name") == null) || (get("name")=="")) {
			String errorString = "illegal anonymous error: each relation must have a name";
			throw new gov.nasa.arc.l2tools.livingstone.code.IllegalAnonymousError(errorString);
		}	
		super.refreshCache();
	} catch (gov.nasa.arc.l2tools.Exception error) {
		error.concatMessage("\n  ");
		if (getLine()!=null) {
			error.concatMessage("line "+getLine()+": ");
		}
		error.concatMessage("in relation");
		if (getName()!=null) { 
			error.concatMessage(": "+getName());
		}
		throw error;
	}
}
/**
 * search for a Relation with value <i>value</i> for key <i>key</i>
 * Creation date: (10/24/2000 4:05:22 PM)
 * @return gov.nasa.arc.l2tools.livingstone.code.Relation
 * @param key java.lang.String
 * @param value java.lang.String
 * @param object gov.nasa.arc.l2tools.DataObject
 */
public static Relation search(String key, String value, gov.nasa.arc.l2tools.DataObject object) {
	return (Relation)object.search(Relation.class,key,value);
}
/**
 * search for a Relation named <i>name</i>
 * Creation date: (10/24/2000 4:05:22 PM)
 * @return gov.nasa.arc.l2tools.livingstone.code.Relation
 * @param name java.lang.String
 * @param object gov.nasa.arc.l2tools.DataObject
 */
public static Relation searchByName(String name, gov.nasa.arc.l2tools.DataObject object) {
	return (Relation)object.searchByName(Relation.class,name);
}
}

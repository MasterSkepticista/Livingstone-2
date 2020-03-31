//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.componenttype;

/**
 * Insert the type's description here.
 * Creation date: (3/6/2000 11:23:31 AM)
 * @author: Andrew Bachmann
 */
public class NominalTransition extends AbstractTransition {
	public final static String tagName = "ct:transition";
	private java.lang.String cost = null;
/**
 * CT_NominalTransition constructor comment.
 */
public NominalTransition() {
	super(tagName);
}
/**
 * CT_NominalTransition constructor comment.
 * @param name java.lang.String
 */
public NominalTransition(String name) {
	super(name);
}
/**
 *
 * Creation date: (3/23/2000 7:05:45 PM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public java.lang.String getCost() {
	return cost;
}
/**
 * Creation date: (4/8/2000 9:55:20 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.Transition
 * @param component gov.nasa.arc.l2tools.livingstone.componentinstance.Component
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
public gov.nasa.arc.l2tools.livingstone.componentinstance.Transition instantiate(gov.nasa.arc.l2tools.livingstone.componentinstance.Component component) 
throws gov.nasa.arc.l2tools.Exception {
	try {
		gov.nasa.arc.l2tools.livingstone.componentinstance.Transition transition
		  = super.instantiate(component);
	  if ((getCost()!=null) && (getCost()!="")) {
		  transition.set("cost",getCost());
	  }
		return transition;
	} catch (gov.nasa.arc.l2tools.Exception e) {
		e.concatMessage("\n  ");
		if (getLine()!=null) {
			e.concatMessage("line "+getLine()+": ");
		}
		e.concatMessage("in a nominal transition");
		if (getName()!=null) {
			e.concatMessage(": "+getName());
		}
		throw e;
	}
}
/**
 * refreshCache method comment.
 */
public void refreshCache() throws gov.nasa.arc.l2tools.Exception {
	try {
		// cache attributes
		if ((get("cost")!=null) && (get("cost")!="")) {
			setCost(get("cost"));
		}
		// do the rest
		super.refreshCache();
	} catch (gov.nasa.arc.l2tools.Exception error) {
		error.concatMessage("\n  ");
		if (getLine()!=null) {
			error.concatMessage("line "+getLine()+": ");
		}
		error.concatMessage("in a nominal transition");
		if (getName()!=null) { 
			error.concatMessage(": "+getName());
		}
		throw error;
	}
}
/**
 * search for a NominalTransition with value <i>value</i> for key <i>key</i>
 * Creation date: (10/24/2000 4:05:22 PM)
 * @return gov.nasa.arc.l2tools.livingstone.code.componenttype.NominalTransition
 * @param key java.lang.String
 * @param value java.lang.String
 * @param object gov.nasa.arc.l2tools.DataObject
 */
public static NominalTransition search(String key, String value, gov.nasa.arc.l2tools.DataObject object) {
	return (NominalTransition)object.search(NominalTransition.class,key,value);
}
/**
 * search for a NominalTransition named <i>name</i>
 * Creation date: (10/24/2000 4:05:22 PM)
 * @return gov.nasa.arc.l2tools.livingstone.code.componenttype.NominalTransition
 * @param name java.lang.String
 * @param object gov.nasa.arc.l2tools.DataObject
 */
public static NominalTransition searchByName(String name, gov.nasa.arc.l2tools.DataObject object) {
	return (NominalTransition)object.searchByName(NominalTransition.class,name);
}
/**
 *
 * Creation date: (3/23/2000 7:05:45 PM)
 * @author: Andrew Bachmann
 * 
 * @param newCost java.lang.String
 */
protected void setCost(java.lang.String newCost) {
	cost = newCost;
}
}

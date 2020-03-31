//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.componenttype;

/**
 * Insert the type's description here.
 * Creation date: (3/6/2000 11:23:41 AM)
 * @author: Andrew Bachmann
 */
public class FailureTransition extends AbstractTransition {
	public final static String tagName = "ct:failure";
	private java.lang.String probability = null;
/**
 * CT_FailureTransition constructor comment.
 */
public FailureTransition() {
	super(tagName);
}
/**
 * CT_FailureTransition constructor comment.
 * @param name java.lang.String
 */
public FailureTransition(String name) {
	super(name);
}
/**
 *
 * Creation date: (3/23/2000 7:06:24 PM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public java.lang.String getProbability() {
	return probability;
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
	  if ((getProbability()!=null) && (getProbability()!="")) {
		  transition.set("probability",getProbability());
	  }
		return transition;
	} catch (gov.nasa.arc.l2tools.Exception e) {
		e.concatMessage("\n  ");
		if (getLine()!=null) {
			e.concatMessage("line "+getLine()+": ");
		}
		e.concatMessage("in a failure transition");
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
		if ((get("probability")!=null) && (get("probability")!="")) {
			setProbability(get("probability"));
		}
		// do the rest
		super.refreshCache();
	} catch (gov.nasa.arc.l2tools.Exception error) {
		error.concatMessage("\n  ");
		if (getLine()!=null) {
			error.concatMessage("line "+getLine()+": ");
		}
		error.concatMessage("in a failure transition");
		if (getName()!=null) { 
			error.concatMessage(": "+getName());
		}
		throw error;
	}
}
/**
 * search for a FailureTransition with value <i>value</i> for key <i>key</i>
 * Creation date: (10/24/2000 4:05:22 PM)
 * @return gov.nasa.arc.l2tools.livingstone.code.componenttype.FailureTransition
 * @param key java.lang.String
 * @param value java.lang.String
 * @param object gov.nasa.arc.l2tools.DataObject
 */
public static FailureTransition search(String key, String value, gov.nasa.arc.l2tools.DataObject object) {
	return (FailureTransition)object.search(FailureTransition.class,key,value);
}
/**
 * search for a FailureTransition named <i>name</i>
 * Creation date: (10/24/2000 4:05:22 PM)
 * @return gov.nasa.arc.l2tools.livingstone.code.componenttype.FailureTransition
 * @param name java.lang.String
 * @param object gov.nasa.arc.l2tools.DataObject
 */
public static FailureTransition searchByName(String name, gov.nasa.arc.l2tools.DataObject object) {
	return (FailureTransition)object.searchByName(FailureTransition.class,name);
}
/**
 *
 * Creation date: (3/23/2000 7:06:24 PM)
 * @author: Andrew Bachmann
 * 
 * @param newProbability java.lang.String
 */
protected void setProbability(java.lang.String newProbability) {
	probability = newProbability;
}
}

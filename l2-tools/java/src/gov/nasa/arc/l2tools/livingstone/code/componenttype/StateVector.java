//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.componenttype;

/**
 * Insert the type's description here.
 * Creation date: (3/6/2000 11:20:28 AM)
 * @author: Andrew Bachmann
 */
public class StateVector extends gov.nasa.arc.l2tools.livingstone.code.CodeParticle 
implements gov.nasa.arc.l2tools.livingstone.code.Instantiable {
	public final static String tagName = "ct:statevector";
	private javax.swing.DefaultListModel attributes = new javax.swing.DefaultListModel();
/**
 * CT_StateVector constructor comment.
 */
public StateVector() {
	super(tagName);
}
/**
 * CT_StateVector constructor comment.
 * @param name java.lang.String
 */
public StateVector(String name) {
	super(name);
}
/**
 *
 * Creation date: (3/23/2000 7:27:09 PM)
 * @author: Andrew Bachmann
 * 
 * @return javax.swing.DefaultListModel
 */
public javax.swing.DefaultListModel getAttributes() {
	return attributes;
}
/**
 * Creation date: (4/8/2000 9:05:23 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.data.componenttype.StateVector
 * @param component gov.nasa.arc.l2tools.livingstone.componentinstance.Component
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
public gov.nasa.arc.l2tools.livingstone.componentinstance.StateVector instantiate(gov.nasa.arc.l2tools.livingstone.componentinstance.Component component) 
throws gov.nasa.arc.l2tools.Exception {
	try {
		// compile ourselves
		compile();
		// bail if there's no attributes
		if (getAttributes().isEmpty()) return null;
	  // make instantiated class
		gov.nasa.arc.l2tools.livingstone.componentinstance.StateVector stateVector
		  = new gov.nasa.arc.l2tools.livingstone.componentinstance.StateVector(component,this);
	  String vars=null;
		java.util.Enumeration attributes=getAttributes().elements();
		while (attributes.hasMoreElements()) {
			String attributeName=(String)attributes.nextElement();
			Name name=new Name(attributeName);
			gov.nasa.arc.l2tools.livingstone.componentinstance.Attribute attribute=name.instantiate(component);
			if (attribute==null || attribute.getAbstractAttributeType()==null) {
				String errorString="syntax error: attribute '"+attributeName+"' not found for state vector of component '"+component.getName()+"'";
				throw new gov.nasa.arc.l2tools.livingstone.SyntaxError(errorString);
			} else {
				if (vars==null) vars=attribute.getName();
				else vars+=" "+attribute.getName();
				stateVector.setLength(stateVector.getLength()+1);
			}
		}
		if ((vars!=null) && (vars!=""))
			stateVector.set("vars",vars);	
		return stateVector;
	}
	catch (gov.nasa.arc.l2tools.Exception error) {
		error.concatMessage("\n  ");
		if (getLine()!=null) {
			error.concatMessage("line "+getLine()+": ");
		}
		error.concatMessage("in a state vector");
		throw error;
	}
}
/**
 * refreshCache method comment.
 */
public void refreshCache() throws gov.nasa.arc.l2tools.Exception {
//	try {
		// cache attributes
		if ((get("vars")!=null) && (get("vars")!="")) {
			java.util.StringTokenizer varsTokenizer=new java.util.StringTokenizer(get("vars"));
			while (varsTokenizer.hasMoreTokens()) {
				getAttributes().addElement(varsTokenizer.nextToken());
			}
		}
		// check for children
		if (!getChildren().isEmpty()) {
			System.out.println("Ignoring children of state vector");
		}
//	}
//	catch (gov.nasa.arc.l2tools.Exception error) {
//		error.concatMessage("\n  ");
//		if (getLine()!=null) {
//			error.concatMessage("line "+getLine()+": ");
//		}
//		error.concatMessage("in a state vector");
//		throw error;
//	}
}
/**
 * refreshClauses method comment.
 */
public void refreshExpression() throws gov.nasa.arc.l2tools.Exception {}
/**
 *
 * Creation date: (3/23/2000 7:27:09 PM)
 * @author: Andrew Bachmann
 * 
 * @param newAttributes javax.swing.DefaultListModel
 */
protected void setAttributes(javax.swing.DefaultListModel newAttributes) {
	attributes = newAttributes;
}
}

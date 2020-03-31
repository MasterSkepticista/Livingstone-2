//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.componentinstance;

/**
 * Insert the type's description here.
 * Creation date: (3/14/2000 11:20:44 AM)
 * @author: Andrew Bachmann
 */
public class EqualityTerm extends gov.nasa.arc.l2tools.livingstone.componentinstance.Particle {
	public final static String tagName = "ci:term";
	// numbers make no presumption of order
	private java.lang.String argument1 = null;
	private java.lang.String argument2 = null;
	// synchronized internal representation
  class XMLContent extends gov.nasa.arc.l2tools.Content {
	  public String getContent() {
		  if (getSign()) return getString();
			else return "!"+getString();
	  }
  }
	private XMLContent xmlContent=new XMLContent();
	private boolean sign = true;
	private Clause clause = null;
/**
 * Literal constructor comment.
 */
public EqualityTerm() {
	super(tagName);
	setupListModelListener();
}
/**
 * Creation date: (4/8/2000 7:47:49 PM)
 * @author: Andrew Bachmann
 * 
 * @param clause gov.nasa.arc.l2tools.livingstone.componentinstance.Clause
 */
public EqualityTerm(Clause clause) {
	this();
	setupContent();
	setClause(clause);
	clause.getChildren().addElement(this);
}
/**
 * Literal constructor comment.
 * @param name java.lang.String
 */
public EqualityTerm(String name) {
	super(name);
	setupContent();
}
/**
 *
 * Creation date: (3/29/2000 9:55:48 AM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public java.lang.String getArgument1() {
	return argument1;
}
/**
 *
 * Creation date: (3/29/2000 9:56:01 AM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public java.lang.String getArgument2() {
	return argument2;
}
/**
 * Creation date: (4/8/2000 7:48:02 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.Clause
 */
public Clause getClause() {
	return clause;
}
/**
 *
 * Creation date: (3/24/2000 3:25:00 PM)
 * @author: Andrew Bachmann
 * 
 * @return boolean
 */
public boolean getSign() {
	return sign;
}
/**
 *
 * Creation date: (3/24/2000 3:32:35 PM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public java.lang.String getString() {
	String left=getArgument1();
	String right=getArgument2();
	if (left==null) left="null";
	if (right==null) right="null";
/*	if (left.compareTo(right) > 0) {
		left=getArgument2();
		right=getArgument1();
	}*/
	return left+"="+right;
}
/**
 * search for a EqualityTerm with value <i>value</i> for key <i>key</i>
 * Creation date: (10/24/2000 4:05:22 PM)
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.EqualityTerm
 * @param key java.lang.String
 * @param value java.lang.String
 * @param object gov.nasa.arc.l2tools.DataObject
 */
public static EqualityTerm search(String key, String value, gov.nasa.arc.l2tools.DataObject object) {
	return (EqualityTerm)object.search(EqualityTerm.class,key,value);
}
/**
 *
 * Creation date: (3/29/2000 9:55:48 AM)
 * @author: Andrew Bachmann
 * 
 * @param newArgument1 java.lang.String
 */
public void setArgument1(java.lang.String newArgument1) {
	argument1 = newArgument1;
	updateString();
}
/**
 *
 * Creation date: (3/29/2000 9:56:01 AM)
 * @author: Andrew Bachmann
 * 
 * @param newArgument2 java.lang.String
 */
public void setArgument2(java.lang.String newArgument2) {
	argument2 = newArgument2;
	updateString();
}
/**
 * Creation date: (4/8/2000 7:48:02 PM)
 * @author: Andrew Bachmann
 * 
 * @param newClause gov.nasa.arc.l2tools.livingstone.componentinstance.Clause
 */
protected void setClause(Clause newClause) {
	clause = newClause;
}
/**
 *
 * Creation date: (3/24/2000 3:25:00 PM)
 * @author: Andrew Bachmann
 * 
 * @param newSign boolean
 */
public void setSign(boolean newSign) {
	sign = newSign;
}
/**
 * Creation date: (4/8/2000 4:35:09 PM)
 * @author: Andrew Bachmann
 * 
 */
protected void setString(String string) {
	xmlContent.setContent(string);
}
/**
 *
 * Creation date: (3/14/2000 12:41:15 PM)
 * @author: Andrew Bachmann
 * 
 */
private void setupContent() {
	getChildren().addElement(xmlContent);
}
/**
 *
 * Creation date: (3/6/2000 1:54:02 PM)
 * @author: Andrew Bachmann
 * 
 */
private void setupListModelListener() {
	javax.swing.event.ListDataListener listener = new javax.swing.event.ListDataListener() {
		public void contentsChanged(javax.swing.event.ListDataEvent event) {
		}
		public void intervalAdded(javax.swing.event.ListDataEvent event) {
			java.lang.Object child=getChildren().elementAt(event.getIndex0());
			if (child instanceof gov.nasa.arc.l2tools.Content) {
				String content=((gov.nasa.arc.l2tools.Content)child).getContent();
				if (content.startsWith("!")) {
					setSign(false);
					content=content.substring(1);
				} else {
					setSign(true);
				}
				int equalsPosition=content.indexOf('=');
				// don't crash
				if (equalsPosition==-1) {
					return;
				}
				setArgument1(content.substring(0,equalsPosition));
				// don't crash
				if (equalsPosition==content.length()) {
					return;
				}
				setArgument2(content.substring(equalsPosition+1));
				getChildren().removeListDataListener(this);
			}
		}
		public void intervalRemoved(javax.swing.event.ListDataEvent event) {
		}
	};
	getChildren().addListDataListener(listener);
}
/**
 *
 * Creation date: (3/29/2000 9:57:08 AM)
 * @author: Andrew Bachmann
 * 
 */
protected void updateString() {
	if (getSign())
		setString(toString());
	else
		setString("!"+toString());
}
}

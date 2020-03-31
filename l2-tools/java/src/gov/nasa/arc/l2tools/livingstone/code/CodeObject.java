//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code;

/**
 * Insert the type's description here.
 * Creation date: (3/10/2000 6:21:17 PM)
 * @author: Andrew Bachmann
 */
public abstract class CodeObject extends gov.nasa.arc.l2tools.livingstone.DataObject 
  implements Cachable, Compilable {
  public final static String tagName = "codeobject";
	// flags to show if the object is cached or compiled
	private boolean cachedFlag = false;
	private boolean compiledFlag = false;
	// code information
	private java.lang.Integer line = null;
	// updates flags when object is modified
	private class FlagObserver implements java.util.Observer {
		public void update(java.util.Observable obsv, java.lang.Object obj) {
			setCachedFlag(false);
			setCompiledFlag(false);
		}
	}
	private FlagObserver flagObserver = new FlagObserver();
	private gov.nasa.arc.l2tools.livingstone.code.logic.LogicExpression expression
	  = new gov.nasa.arc.l2tools.livingstone.code.logic.AndExpression();
/**
 * CodeObject constructor comment.
 */
public CodeObject() {
	super(tagName);
}
/**
 * CodeObject constructor comment.
 * @param name java.lang.String
 */
public CodeObject(String name) {
	super(name);
}
/**
 *
 * Creation date: (3/10/2000 5:31:14 PM)
 * @author: Andrew Bachmann
 * 
 */
public synchronized final void cache() throws gov.nasa.arc.l2tools.Exception {
	if (getCachedFlag()==false) {
		if ((get("line") != null) && (get("line") != "")) {
			setLine(Integer.valueOf(get("line")));
		}
		refreshCache();
		setCachedFlag(true);
		// feedback on progression
		System.out.print("-");
	}
}
/**
 *
 * Creation date: (3/10/2000 5:31:14 PM)
 * @author: Andrew Bachmann
 * 
 */
public synchronized final void compile() throws gov.nasa.arc.l2tools.Exception {
	cache();
	if (getCompiledFlag()==false) {
		refreshExpression();
		if (getExpression()!=null) {
			getExpression().setLine(getLine());
		}
		setCompiledFlag(true);
		// feedback on progression
		System.out.print("+");
	}
}
/**
 *
 * Creation date: (3/10/2000 4:18:27 PM)
 * @author: Andrew Bachmann
 * 
 * @return boolean
 */
public boolean getCachedFlag() {
	return cachedFlag;
}
/**
 *
 * Creation date: (3/10/2000 4:18:27 PM)
 * @author: Andrew Bachmann
 * 
 * @return boolean
 */
public boolean getCompiledFlag() {
	return compiledFlag;
}
/**
 * Creation date: (4/5/2000 12:10:49 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.code.logic.LogicExpression
 */
public gov.nasa.arc.l2tools.livingstone.code.logic.LogicExpression getExpression() {
	return expression;
}
/**
 * Creation date: (6/28/2000 2:34:10 PM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.Integer
 */
public java.lang.Integer getLine() {
	return line;
}
/**
 *
 * Creation date: (3/22/2000 2:57:53 PM)
 * @author: Andrew Bachmann
 * 
 */
public abstract void refreshCache() throws gov.nasa.arc.l2tools.Exception;
/**
 * Creation date: (4/5/2000 12:08:34 PM)
 * @author: Andrew Bachmann
 * 
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
public abstract void refreshExpression() throws gov.nasa.arc.l2tools.Exception;
/**
 *
 * Creation date: (3/10/2000 4:18:27 PM)
 * @author: Andrew Bachmann
 * 
 * @param newCachedFlag boolean
 */
protected void setCachedFlag(boolean newCachedFlag) {
	cachedFlag = newCachedFlag;
}
/**
 *
 * Creation date: (3/10/2000 4:18:27 PM)
 * @author: Andrew Bachmann
 * 
 * @param newCompiledFlag boolean
 */
protected void setCompiledFlag(boolean newCompiledFlag) {
	compiledFlag = newCompiledFlag;
}
/**
 * Creation date: (4/5/2000 12:10:49 PM)
 * @author: Andrew Bachmann
 * 
 * @param newExpression javax.swing.DefaultListModel
 */
protected void setExpression(gov.nasa.arc.l2tools.livingstone.code.logic.LogicExpression newExpression) {
	expression = newExpression;
}
/**
 * Creation date: (6/28/2000 2:34:10 PM)
 * @author: Andrew Bachmann
 * 
 * @param newLine java.lang.Integer
 */
protected void setLine(java.lang.Integer newLine) {
	line = newLine;
}
}

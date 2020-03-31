//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.constraint;

/**
 * Insert the type's description here.
 * Creation date: (3/6/2000 11:40:30 AM)
 * @author: Andrew Bachmann
 */
public class EqualityExpression extends Expression {
	public final static String tagName = "ex:equals";
  // compilation caches
	private javax.swing.DefaultListModel arguments = new javax.swing.DefaultListModel();
/**
 * EqualityExpression constructor comment.
 */
public EqualityExpression() {
	super(tagName);
}
/**
 * EqualityExpression constructor comment.
 * @param name java.lang.String
 */
public EqualityExpression(String name) {
	super(name);
}
/**
 *
 * Creation date: (3/23/2000 11:17:16 AM)
 * @author: Andrew Bachmann
 * 
 * @return javax.swing.DefaultListModel
 */
public javax.swing.DefaultListModel getArguments() {
	return arguments;
}
/**
 *
 * Creation date: (3/10/2000 5:31:14 PM)
 * @author: Andrew Bachmann
 * 
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
public void refreshCache() throws gov.nasa.arc.l2tools.Exception {
	try {
		// cache attributes
		String arguments=get("arguments");
		// make sure the attribute exists
		if (arguments==null) {
			String errorString="syntax error: constraint must have at least two arguments";
			errorString+="\n  a constraint has no arguments";
			throw new gov.nasa.arc.l2tools.livingstone.SyntaxError(errorString);
		}
		java.util.StringTokenizer argumentTokenizer=new java.util.StringTokenizer(arguments);
		// ensure that there is at least one argument
		if (!argumentTokenizer.hasMoreTokens()) {
			String errorString="syntax error: constraint must have at least two arguments";
			errorString+="\n  a constraint has empty arguments";
			throw new gov.nasa.arc.l2tools.livingstone.SyntaxError(errorString);
		}
		// put the first name into the default list model
		getArguments().addElement(argumentTokenizer.nextToken());
		// ensure that there are at least two arguments
		if (!argumentTokenizer.hasMoreTokens()) {
			String errorString="syntax error: constraint must have at least two arguments";
			errorString+="\n  a constraint has only one argument";
			throw new gov.nasa.arc.l2tools.livingstone.SyntaxError(errorString);
		}
		// put each name into the default list model
		while (argumentTokenizer.hasMoreTokens()) {
			getArguments().addElement(argumentTokenizer.nextToken());
		}
		// check for children
		if (!getChildren().isEmpty()) {
			System.out.println("Ignoring children of equality expression");
		}
	} catch (gov.nasa.arc.l2tools.Exception error) {
		error.concatMessage("\n  ");
		if (getLine()!=null) {
			error.concatMessage("line "+getLine()+": ");
		}
		error.concatMessage("in an equality expression");
		throw error;
	}
}
/**
 * refreshClauses method comment.
 */
public void refreshExpression() throws gov.nasa.arc.l2tools.livingstone.code.CompilerError {
	setExpression(new gov.nasa.arc.l2tools.livingstone.code.logic.AndExpression());
	
	java.util.Enumeration arguments=getArguments().elements();
	if (!arguments.hasMoreElements()) return;
	
	java.lang.Object firstArgument=arguments.nextElement();
	if (!(firstArgument instanceof String)) {
		String errorString="compiler error: arguments of equality expression are corrupted";
		throw new gov.nasa.arc.l2tools.livingstone.code.CompilerError(errorString);
	}
	
	java.lang.String leftArgument=(String)firstArgument;
	while (arguments.hasMoreElements()) {
		java.lang.Object argument=arguments.nextElement();
		if (argument instanceof String) {
			gov.nasa.arc.l2tools.livingstone.code.logic.EqualityExpression equality
			  = new gov.nasa.arc.l2tools.livingstone.code.logic.EqualityExpression();
			equality.setArgument1(leftArgument);
			equality.setArgument2((String)argument);
			setExpression(getExpression().destructiveConjunction(equality));
		} else {
			String errorString="compiler error: arguments of equality expression are corrupted";
			throw new gov.nasa.arc.l2tools.livingstone.code.CompilerError(errorString);
		}
	}
}
/**
 *
 * Creation date: (3/23/2000 11:17:16 AM)
 * @author: Andrew Bachmann
 * 
 * @param newArguments javax.swing.DefaultListModel
 */
protected void setArguments(javax.swing.DefaultListModel newArguments) {
	arguments = newArguments;
}
}
